#define VIRTIO_MMIO_MAGIC_VALUE 0x000
#define VIRTIO_MMIO_VERSION 0x004
#define VIRTIO_MMIO_DEVICE_ID 0x008
#define VIRTIO_MMIO_VENDOR_ID 0x00C
#define VIRTIO_MMIO_DEVICE_FEATURES 0x010
#define VIRTIO_MMIO_DEVICE_FEATURES_SEL 0x014
#define VIRTIO_MMIO_DRIVER_FEATURES 0x020
#define VIRTIO_MMIO_GUEST_PAGE_SIZE 0x028 // page size of PFN, write-only
#define VIRTIO_MMIO_DRIVER_FEATURES_SEL 0x024
#define VIRTIO_MMIO_QUEUE_SEL 0x030
#define VIRTIO_MMIO_QUEUE_NUM_MAX 0x034 // max size of current queue, read-only
#define VIRTIO_MMIO_QUEUE_NUM 0x038 // size of current queue, write-only
#define VIRTIO_MMIO_QUEUE_PFN 0x040 // physical page number for queue, read/write

#define VIRTIO_MMIO_STATUS 0x070


#define VIRTIO_CONFIG_S_ACKNOWLEDGE 1
#define VIRTIO_CONFIG_S_DRIVER 2
#define VIRTIO_CONFIG_S_FEATURES_OK 8
#define VIRTIO_CONFIG_S_DRIVER_OK 4


// device feature bits
#define VIRTIO_BLK_F_RO 5
#define VIRTIO_BLK_F_SCSI 7
#define VIRTIO_BLK_F_CONFIG_WCE 11
#define VIRTIO_BLK_F_MQ 12
#define VIRTIO_BLK_F_LAYOUT 27
#define VIRTIO_BLK_F_INDIRECT_DESC 28
#define VIRTIO_BLK_F_EVENT_IDX 29

// this many virtio descriptors.
// must be a power of two.
#define NUM 8
