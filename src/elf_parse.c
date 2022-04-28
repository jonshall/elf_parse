#include <elf_parse.h>

/**
 * @file   elf_parse.c
 * @author Jon S Hall
 * @brief  parse 32-bit elf executables
 * @date   April 2022
 * @ref    Learning Linux Binary Analysis by Ryan "Elfmaster" O'Neil
 */

int
main(int argc, char **argv)
{
    int fd     = 0;
    int status = BAD_EXIT;

    uint8_t *   p_mem       = NULL;
    char *      p_interp    = NULL;
    const char *p_str_table = NULL;

    struct stat st = { 0 };

    Elf32_Ehdr *ehdr = NULL;
    Elf32_Phdr *phdr = NULL;
    Elf32_Shdr *shdr = NULL;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
        goto EXIT;
    }

    if (0 > (fd = open(argv[1], O_RDONLY)))
    {
        perror("[-] open");
        goto EXIT;
    }

    if (0 > fstat(fd, &st))
    {
        perror("[-] fstat");
        goto EXIT;
    }

    // map into memory
    p_mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (p_mem == MAP_FAILED)
    {
        perror("[-] mmap");
        goto EXIT;
    }

    // initial ELF header starts at offset 0 of mapped memory
    ehdr = (Elf32_Ehdr *)p_mem;

    // phdr table offset given by e_phoff in Elf32_Ehdr
    phdr = (Elf32_Phdr *)&p_mem[ehdr->e_phoff];

    // shdr table offset given by e_shoff in Elf32_Ehdr
    shdr = (Elf32_Shdr *)&p_mem[ehdr->e_shoff];

    // Ensure ELF magic bytes match (0x7f E L F)
    if ((ELF_MAGIC != p_mem[0]) && (strcmp(&p_mem[1], ELF_STR)))
    {
        fprintf(stderr, "[-] %s is not an ELF file\n", argv[1]);
        goto EXIT;
    }

    printf("[+] Program entry point:\n\t0x%x\n", ehdr->e_entry);

    // find str table indexes for section header names e_shstrndx
    p_str_table = &p_mem[shdr[ehdr->e_shstrndx].sh_offset];

    // print each section header name and addr using indexes of str table
    printf("[+] Section header list:\n");
    for (int i = 0; i < ehdr->e_shnum; i++)
    {
        printf("\t%s: 0x%x\n", &p_str_table[shdr[i].sh_name], shdr[i].sh_addr);
    }

    // print segment name, addr. For PT_INTERP, print path to dynamic linker
    // (interpreter)
    printf("[+] Program header list:\n");
    for (int i = 0; i < ehdr->e_phnum; i++)
    {
        switch (phdr[i].p_type)
        {
            case PT_LOAD:
                // text segment offset starts at 0, only other possible loadable
                // segment is data
                if (0 == phdr[i].p_offset)
                {
                    printf("\tText segment:    0x%x\n", phdr[i].p_vaddr);
                }
                else
                {
                    printf("\tData segment:    0x%x\n", phdr[i].p_vaddr);
                }
                break;
            case PT_INTERP:
                p_interp = strdup((char *)&p_mem[phdr[i].p_offset]);
                printf("\tInterpreter:     %s\n", p_interp);
                free(p_interp);
                p_interp = NULL;
                break;
            case PT_NOTE:
                printf("\tNote segment:    0x%x\n", phdr[i].p_vaddr);
                break;
            case PT_DYNAMIC:
                printf("\tDynamic segment: 0x%x\n", phdr[i].p_vaddr);
                break;
            case PT_PHDR:
                printf("\tPhdr segment:    0x%x\n", phdr[i].p_vaddr);
                break;
        }
    }
    status = GOOD_EXIT;

EXIT:
    return (status);
}