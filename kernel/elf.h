// Format of an ELF executable file
#define ELF_MAGIC 0x464C457FU

// File header
struct elfhdr {
    uint magic; // must equal ELF_MAGIC
};