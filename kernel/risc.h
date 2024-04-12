
// m mode
#define MSTATUS_MPP_MASK (3L << 11)
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)

// 向上取整
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))

// 向下取整
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

#define PGSIZE 4096 

#define MAXVA (1L<<(9+9+9+12-1))

#define SATP_SV39 (0x8L << 60)
#define MAKE_SATP(pagetable) (SATP_SV39 | (((uint64)pagetable) >> 12))

#define PTE_V (1<<0)
#define PTE_R (1<<1)
#define PTE_W (1<<2)
#define PTE_X (1<<3)
#define PTE_U (1L << 4) // 1 -> user can access


typedef uint64 pte_t;

typedef uint64 *pagetable_t;

static inline void 
w_mepc(uint64 x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}

static inline void
w_satp(uint64 satp) {
    asm volatile("csrw satp, %0" : : "r" (satp));
}

static inline void
sfence_vma() {
    asm volatile("sfence.vma zero, zero");
}

static inline uint64
r_tp() {
    uint64 x;
    asm volatile("mv %0,tp" : "=r" (x));

    return x;
}

static inline uint64
r_mstatus() {
    uint64 x;
    asm volatile("csrr %0,mstatus" : "=r" (x));

    return x;
}

static inline void
w_mstatus(uint64 x) {
    asm volatile("csrw mstatus, %0" : : "r" (x));
}