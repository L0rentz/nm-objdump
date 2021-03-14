/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** objdump_x86
*/

#include "my.h"

unsigned int get_flags_x86(Elf32_Ehdr *elf, Elf32_Shdr *shdr)
{
    unsigned int flags = 0;
    if (elf->e_type == ET_EXEC) flags += D_PAGED + EXEC_P;
    else if (elf->e_type == ET_DYN) flags += D_PAGED + DYNAMIC;
    else if (elf->e_type == ET_REL) flags += HAS_RELOC;
    for (int i = 0; i < elf->e_shnum; i++)
        if (SHT_DYNSYM == shdr[i].sh_type) flags += HAS_SYMS;
    return (flags);
}


int skip_sections_x86(Elf32_Ehdr *elf, Elf32_Shdr	*shdr, int index)
{
    static char *to_skip[] = { "", ".bss", ".shstrtab", ".symtab", ".strtab",
        ".rela.text", ".rela.debug_info", ".rela.debug_aranges",
        ".rela.debug_line", ".rela.eh_frame", ".tbss", "__libc_freeres_ptrs",
        NULL };
    char *shdr_str = NULL;
    if (shdr[index].sh_size == 0)
        return (1);
    for (int i = 0; to_skip[i] != NULL; i++) {
        shdr_str = (char *)elf + shdr[elf->e_shstrndx].sh_offset;
        if (!strcmp(to_skip[i], &shdr_str[shdr[index].sh_name]))
            return (1);
    }
    return (0);
}

void dump_inner_loop_x86(Elf32_Ehdr *elf, Elf32_Shdr *shdr, int i)
{
    for (unsigned int j = shdr[i].sh_offset;
    j < (unsigned int)(shdr[i].sh_offset + shdr[i].sh_size); j += 16) {
        printf(" %04x ",
            (int)(shdr[i].sh_addr + j - shdr[i].sh_offset));
        hexa_display((unsigned char *)elf + j,
            shdr[i].sh_offset + shdr[i].sh_size - j);
        printf("\n");
    }
}

void dump_data_x86(void *data)
{
    Elf32_Ehdr *elf = (Elf32_Ehdr *)data;
    Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)data + elf->e_shoff);
    char *shdr_str = NULL;
    for (int i = 0; i < elf->e_shnum; i++) {
        if (skip_sections_x86(elf, shdr, i) == 0) {
            shdr_str = (char *)data + shdr[elf->e_shstrndx].sh_offset;
            printf("Contents of section %s:\n", &shdr_str[shdr[i].sh_name]);
            dump_inner_loop_x86(elf, shdr, i);
        }
    }
}

void objdump_x86(void *data, errors_t *s_errors, int archive)
{
    Elf32_Ehdr *elf = (Elf32_Ehdr *)data;
    if (archive == 1)
        printf("     file format elf32-i386\n", s_errors->filename);
    else printf("\n%s:     file format elf32-i386\n", s_errors->filename);
    unsigned int flags =
        get_flags_x86(elf, (Elf32_Shdr *)((char *)data + elf->e_shoff));
    printf("architecture: i386, flags 0x%08x:\n", flags);
    flags_writer(flags);
    printf("start address 0x%08lx\n\n", elf->e_entry);
    dump_data_x86(data);
}