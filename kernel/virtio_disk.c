#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "virtio.h"

#define R(r) ((volatile uint32 *)(VIRTIO0 + (r)))

void virtio_disk_init(void) {

    // 验证设备是否正确
    if (*R(VIRTIO_MMIO_MAGIC_VALUE) != 0x74726976 ||
        *R(VIRTIO_MMIO_VERSION) != 0x1 ||
        *R(VIRTIO_MMIO_DEVICE_ID) != 0x0 ||
        *R(VIRTIO_MMIO_VENDOR_ID) != 0x554d4551
    ) {
        panic("could not find virtio disk!");
    }

    printf("disk init\n");
}