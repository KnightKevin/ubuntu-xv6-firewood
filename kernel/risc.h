// 向上取整
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))

// 向下取整
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

#define PGSIZE 4096 

#define MAXVA (2<<((9+9+9+12-1)))

#define PTE_V (1<<0)
#define PTE_R (1<<1)
#define PTE_W (1<<2)
#define PTE_X (1<<3)
#define PTE_U (1L << 4) // 1 -> user can access


typedef uint64 pte;

typedef uint64 *pagetable_t;