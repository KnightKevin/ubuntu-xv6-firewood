#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "virtio.h"

#define R(r) ((volatile uint32 *)(VIRTIO0 + (r)))

static struct disk {
    char pages[2*PGSIZE];
    struct VRingDesc *desc;
    uint16 *avail;
    struct UsedArea *used;
} __attribute__ ((aligned (PGSIZE))) disk; // 让这个结构体16字节对齐

void virtio_disk_init(void) {

    // 验证设备是否正确
    if (*R(VIRTIO_MMIO_MAGIC_VALUE) != 0x74726976 ||
        *R(VIRTIO_MMIO_VERSION) != 0x1 ||
        *R(VIRTIO_MMIO_DEVICE_ID) != 0x0 ||
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
    uint32 features = *R(VIRTIO_MMIO_DEVICE_FEATURES);

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

    *R(VIRTIO_MMIO_QUEUE_SEL) = 0x0;

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

                        

    printf("disk init\n");
}