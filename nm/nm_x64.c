/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** nm_x64
*/

#include "my.h"

char get_symbol_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
    char c;
    if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE) c = 'u';
    else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF) c = 'w';
    } else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK
        && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF) c = 'v';
    } else if (sym.st_shndx == SHN_UNDEF) c = 'U';
    else if (sym.st_shndx == SHN_ABS) c = 'A';
    else if (sym.st_shndx == SHN_COMMON) c = 'C';
    else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) c = 'B';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == SHF_ALLOC) c = 'R';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) c = 'D';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) c = 'T';
    else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC) c = 'D';
    else c = '?';
    if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?') c += 32;
    if (c == '?') c = 't';
    return (c);
}

char get_other_symbol_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
    char c;
    if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE) c = 'u';
    else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF) c = 'w';
    } else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK
        && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF) c = 'v';
    } else if (sym.st_shndx == SHN_UNDEF) c = 'U';
    else if (sym.st_shndx == SHN_ABS) c = 'A';
    else if (sym.st_shndx == SHN_COMMON) c = 'C';
    else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) c = 'B';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == SHF_ALLOC) c = 'R';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) c = 'D';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) c = 'T';
    else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC) c = 'D';
    else c = '?';
    if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?') c += 32;
    if (c == '?') c = 't';
    return (c);
}

void fill_symbol_list(void *data, Elf64_Shdr *symtab, Elf64_Shdr *strtab)
{
    Elf64_Ehdr *elf = (Elf64_Ehdr *)data;
    Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)data + elf->e_shoff);
    Elf64_Sym *sym = (Elf64_Sym *)(data + symtab->sh_offset);
    char *str = (char *)(data + strtab->sh_offset);
    symbolsInfos_t list[symtab->sh_size / symtab->sh_entsize];
    size_t len = 0;
    for (size_t i = 0; i < symtab->sh_size / symtab->sh_entsize; i++) {
        if ((str + sym[i].st_name)[0] == '\0'
            || ELF64_ST_TYPE(sym[i].st_info) == 4) continue;
        list[len].type = get_symbol_type(sym[i], shdr);
        list[len].name = str + sym[i].st_name;
        list[len].adr = sym[i].st_value;
        len++;
    }
    sort_symbol_list(&list, len);
    print_symbol_list(list, len);
}

void get_symtab(void *data, Elf64_Shdr **symtab, Elf64_Shdr **strtab)
{
    Elf64_Ehdr *elf = (Elf64_Ehdr *)data;
    Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)data + elf->e_shoff);
    char *str = (char *)(data + shdr[elf->e_shstrndx].sh_offset);
    for (int i = 0; i < elf->e_shnum; i++) {
        if (strcmp(&str[shdr[i].sh_name], ".symtab") == 0)
            (*symtab) = (Elf64_Shdr *)&shdr[i];
        if (strcmp(&str[shdr[i].sh_name], ".strtab") == 0)
            (*strtab) = (Elf64_Shdr *)&shdr[i];
    }
}

void nm_x64(void *data, errors_t *s_errors)
{
    Elf64_Shdr *symtab = NULL;
    Elf64_Shdr *strtab = NULL;
    get_symtab(data, &symtab, &strtab);
    if (symtab == NULL || strtab == NULL) {
        error_no_symbols(s_errors->binary, s_errors->filename);
        return;
    }
    fill_symbol_list(data, symtab, strtab);
}