
// Saved registers for kernel context switches.
struct context {
    uint64 ra;
    uint64 sp;

// callee-saved
    uint64 s0;
    uint64 s1;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;

};

struct proc {
    uint64 KSTACK;

    // swtch() here to run process
    struct context *context;
};

struct cpu {
    struct proc *p;
    struct context *context;
};

