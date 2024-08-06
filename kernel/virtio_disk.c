#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "virtio.h"
#include "fs.h"
#include "buf.h"

#define R(r) ((volatile uint32 *)(VIRTIO0 + (r)))

static struct disk {
    char pages[2*PGSIZE];
    struct VRingDesc *desc;
    uint16 *avail;
    struct UsedArea *used;

    // 用于标记每个描述符是否空闲
    char free[NUM];

    // we've looked this far in used[2..NUM].
    // 跟踪已处理到的'used'环的索引位置。（比如上次处理到索引2，那么这个值就变成3）
    uint16 used_idx;

    // 用于记录飞行中的I/O操作的相关信息.通过描述连接的
    struct {
        struct buf *b;
        char status;
    } info[NUM];

    struct spinlock vdisk_lock;
} __attribute__ ((aligned (PGSIZE))) disk; // 让这个结构体16字节对齐

void virtio_disk_init(void) {

    // 验证设备是否正确
    if (*R(VIRTIO_MMIO_MAGIC_VALUE) != 0x74726976 ||
        *R(VIRTIO_MMIO_VERSION) != 1 ||
        *R(VIRTIO_MMIO_DEVICE_ID) != 2 ||
        *R(VIRTIO_MMIO_VENDOR_ID) != 0x554d4551
    ) {
        panic("could not find virtio disk!");
    }

    uint32 status =0;

    // 1：重置设备，设置状态寄存器为0即可。
    // 2：设置状态寄存器Acknowladge状态位，OS识别到设备
    // 3：设置状态寄存器Driver状态位，OS知道如何驱动设备
    // 4：读取设备features，做设备状态协商。
    // 5：设置Features_OK状态位，驱动不再接收新的工作特性
    // 6：再次读取设备状态，确保已经设置Features_OK状态位。
    // 7：执行特定设备的设置，包括虚拟队列等(虚拟队列配置流程后方详细展开)
    // 8：设置DRIVER_OK 状态位，此时设备就活起来了，是的，非常优雅，不愧是标准。(原话：At this point the device is “live")
                        
    status |= VIRTIO_CONFIG_S_ACKNOWLEDGE;

    *R(VIRTIO_MMIO_STATUS) = status;

    status |= VIRTIO_CONFIG_S_DRIVER;

    *R(VIRTIO_MMIO_STATUS) = status;

    // 下面这几位被禁掉
    // 1：VIRTIO_BLK_F_RO (5-->第5位) 表示块设备只读
    // 2：VIRTIO_BLK_F_SCSI (7) 表示虚拟块设备支持SCSI（Small Computer System Interface）包命令
    // 3：VIRTIO_BLK_F_CONFIG_WCE(11) 表示允许虚拟块设备在写回和写穿模式之间切换其缓存
    // 4：VIRTIO_BLK_F_MQ(12) 表示设备支持多个虚拟队列，xv6中只使用了一个，默认的队列0
    // 5：VIRTIO_F_ANY_LAYOUT(27) 出于对旧版的兼容，表示设备和驱动之间对消息帧不做任何协商
    // 6：VIRTIO_RING_F_INDIRECT_DESC(28) 用于扩展descriptor数组到更大容量，由原本的一个队列
    uint64 features = *R(VIRTIO_MMIO_DEVICE_FEATURES);

    features &= ~(1 << VIRTIO_BLK_F_RO);
    features &= ~(1 << VIRTIO_BLK_F_SCSI);
    features &= ~(1 << VIRTIO_BLK_F_CONFIG_WCE);
    features &= ~(1 << VIRTIO_BLK_F_MQ);
    features &= ~(1 << VIRTIO_BLK_F_LAYOUT);
    features &= ~(1 << VIRTIO_BLK_F_EVENT_IDX);
    features &= ~(1 << VIRTIO_BLK_F_INDIRECT_DESC);

    *R(VIRTIO_MMIO_DRIVER_FEATURES) = features;

    status |= VIRTIO_CONFIG_S_FEATURES_OK;
    *R(VIRTIO_MMIO_STATUS) = status;

    // tell device we're completely ready
    status |= VIRTIO_CONFIG_S_DRIVER_OK;
    *R(VIRTIO_MMIO_STATUS) = status;

    status = *R(VIRTIO_MMIO_STATUS);

    if (!(status & VIRTIO_CONFIG_S_FEATURES_OK)) {
        panic("virtio disk FEATURES_OK unset");
    }

    *R(VIRTIO_MMIO_GUEST_PAGE_SIZE) = PGSIZE;
    

    // 设备启动中虚拟队列基本配置流程
    // 1：选择要使用的队列，将它的下标写入QueueSel，xv6使用默认的0号队列
    // 2：检查QueueReady寄存器判断队列是否就绪
    // 3：读取QueueNumMax寄存器，得到设备支持的最大队列大小
    // 4：给队列分配内存空间，确保物理上连续
    // 5：通过写入QueueNum通知设备驱动使用的队列大小
    // 6：写寄存器组：QueueDescLow/QueueDescHigh, QueueDriverLow/QueueDriverHigh and QueueDeviceLow/QueueDeviceHigh分别写入Descriptor Table Available Ring和Used Ring的64位地址。
    // 7：向QueueReady寄存器写1。准备完毕。

    *R(VIRTIO_MMIO_QUEUE_SEL) = 0;

    uint32 max = *R(VIRTIO_MMIO_QUEUE_NUM_MAX);
    if (max <= 0) {
        panic("virtio disk has no queue 0");
    }

    if (max < NUM) {
        panic("virtio disk max queue too short.");
    }

    // 在文档中等同于queue size概念
    *R(VIRTIO_MMIO_QUEUE_NUM) = NUM;
    memset(disk.pages, 0, sizeof(disk.pages));

    // page frame number。用来告诉device virtio队列的物理帧号。
    // virtio通常用环形缓冲区来进行数据传输，这个就是来告诉设备这个环形区域在物理内存哪儿
    // 须要配合GUEST_PAGE_SIZE一起使用
    // 最新的文档已经采用更新的技术替换pfn了，所以找不到详细的描述（怪不得我不知道为什么）
    // 可以去找老版本的文档了
    *R(VIRTIO_MMIO_QUEUE_PFN) = ((uint64)disk.pages) >> PGSHIFT;

    disk.desc = (struct VRingDesc *) disk.pages;
    disk.avail = (uint16 *)(disk.pages + NUM*sizeof(struct VRingDesc));
    disk.used = (struct UsedArea *)(disk.pages + PGSIZE);

    for (int i = 0; i < NUM; i++) {
        disk.free[i] = 1;
    }

    printf("disk init\n");
}


static int alloc_desc()
{
    for (int i=0;i<NUM;i++) {
        if (disk.free[i]) {
            disk.free[i] = 0;
            return i;
        }
    }

    return -1;
}

static void free_desc(int i)
{
    if (i >= NUM) {
        panic("virtio_disk_intr 1");
    }

    if (disk.free[i]) {
        panic("virtio_disk_intr 2");
    }

    disk.desc[i].addr = 0;
    disk.free[i] = 1;
// todo  wakeup(&disk.free[0]);
}

static int alloc3_desc(int *idx)
{
    for (int i=0;i<3;i++) {
        idx[i] = alloc_desc();
        if (idx[i] < 0) {
           for (int j=0;j<i;j++) {
                free_desc(idx[j]);
           }

           return -1;
        }
    }

    return 0;
}

/**
 * b: buf
 * write: 0-read, 1-write
 */
void virtio_disk_rw(struct buf *b, int write) {

    // todo 记得改
    intr_off();

    uint64 sector = b->blockno * (BSIZE/512);

    int idx[3];
    while(1) {
        if (alloc3_desc(idx) == 0) {
            break;
        }

        // sleep(&disk.free[0], &disk.vdisk_lock);
    }


    struct virtio_blk_outhdr {
        uint32 type;
        uint32 reserved;
        uint64 sector;
    } buf0;

    if (write) {
        buf0.type = VIRTIO_BLK_T_OUT; // write
    } else {
        buf0.type = VIRTIO_BLK_T_IN; // read
    }
    buf0.reserved = 0;
    buf0.sector = sector;

    // 现在运行在kernel stack，但是device需要直接的物理地址所以需要转一下
    disk.desc[idx[0]].addr = (uint64) kvmpa((uint64) &buf0);
    disk.desc[idx[0]].len = sizeof(buf0);
    disk.desc[idx[0]].flags = VRING_DESC_F_NEXT;
    disk.desc[idx[0]].next = idx[1];

    disk.desc[idx[1]].addr = (uint64) b->data;
    disk.desc[idx[1]].len = BSIZE;
    if (write) {
        disk.desc[idx[1]].flags = 0; // device read b->data
    } else {
        disk.desc[idx[1]].flags = VRING_DESC_F_WRITE; // device write  b->data
    }
    disk.desc[idx[1]].flags |= VRING_DESC_F_NEXT;
    disk.desc[idx[1]].next = idx[2];

    disk.info[idx[0]].status = 0;
    disk.desc[idx[2]].addr = (uint64) &disk.info[idx[0]].status;
    disk.desc[idx[2]].len = 1;
    disk.desc[idx[2]].flags = VRING_DESC_F_WRITE;
    disk.desc[idx[2]].next = 0;

    // 做一个标记，为1表示还没有操作完，等到virtio_disk_intr()处理完就是变成了0
    b->disk = 1;
    disk.info[idx[0]].b = b;

    //struct virtq_avail {
    //  #define VIRTQ_AVAIL_F_NO_INTERRUPT 1
    //  le16 flags;
    //  le16 idx;
    //  le16 ring[ /* Queue Size */ ];
    //  le16 used_event; /* Only if VIRTIO_F_EVENT_IDX */
    // };
    // 下面这三步就是在操作环形队列，disk.avail[1]是一个计数器（默认为0），表示当前有几个有用的ring entry,
    // 由于disk.avail[2]是一个rang[queue size]的数组，如果要用让他形成环那么在存值的时候就需要用(已用的size % 数组size)
    // 来操作索引。比如当前已经用了1个entry，那么第二个entry的索引在哪里呢，就是（1%8）= 1
    disk.avail[2 + (disk.avail[1] % NUM)] = idx[0];
    __sync_synchronize(); // 告诉编译器前后后两个指令不要代码顺序优化，我这里要保持顺序
    disk.avail[1] = disk.avail[1] + 1;

    // write a queue index to this register notifies the device 
    // that there are new buffers to process in the queue
    *R(VIRTIO_MMIO_QUEUE_NOTIFY) = 0;


    // todo 暂时先这样写，后面记得删掉
    intr_on();

    printf("virtio process:");
    while (b->disk == 1) {
        // todo sleep(b, &disk.vdisk_lock);
        printf(".");
    }
    printf("\n");

    printf("virtio end!\n");

    // todo 执行到这表示磁盘读写已经完成了，该释放一些资源了
    


}

void virtio_disk_intr()
{
      // todo acquire(&disk.vdisk_lock);


    // todo think
    while ((disk.used_idx%NUM) != (disk.used->idx % NUM)) {
        int id = disk.used->elems[disk.used_idx].id;

        if (disk.info[id].status != 0) {
            panic("virtio_disk_intr status");
        }

        disk.info[id].b->disk = 0;

        // todo wakeup(disk.info[id].b);

        disk.used_idx = (disk.used_idx+1) % NUM;

    }

    *R(VIRTIO_MMIO_INTERRUPT_ACK) = *R(VIRTIO_MMIO_INTERRUPT_STATUS) & 0x3;
        // todo release(&disk.vdisk_lock);

}