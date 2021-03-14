/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** nm_x86
*/

#include "my.h"

void fill_symbol_list_x86(void *data, Elf32_Shdr *symtab, Elf32_Shdr *strtab)
{
    Elf32_Ehdr *elf = (Elf32_Ehdr *)data;
    Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)data + elf->e_shoff);
    Elf32_Sym *sym = (Elf32_Sym *)(data + symtab->sh_offset);
    char *str = (char *)(data + strtab->sh_offset);
    symbolsInfos_t list[symtab->sh_size / symtab->sh_entsize];
    size_t len = 0;
    for (size_t i = 0; i < symtab->sh_size / symtab->sh_entsize; i++) {
        if ((str + sym[i].st_name)[0] == '\0'
            || ELF32_ST_TYPE(sym[i].st_info) == 4) continue;
        list[len].type = get_symbol_type_x86(sym[i], shdr);
        list[len].name = str + sym[i].st_name;
        list[len].adr = sym[i].st_value;
        len++;
    }
    sort_symbol_list(&list, len);
    print_symbol_list(list, len);
}

void get_symtab_x86(void *data, Elf32_Shdr **symtab, Elf32_Shdr **strtab)
{
    Elf32_Ehdr *elf = (Elf32_Ehdr *)data;
    Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)data + elf->e_shoff);
    char *str = (char *)(data + shdr[elf->e_shstrndx].sh_offset);
    for (int i = 0; i < elf->e_shnum; i++) {
        if (strcmp(&str[shdr[i].sh_name], ".dynsym") == 0)
            (*symtab) = (Elf32_Shdr *)&shdr[i];
        if (strcmp(&str[shdr[i].sh_name], ".strtab") == 0)
            (*strtab) = (Elf32_Shdr *)&shdr[i];
    }
}

void nm_x86(void *data, errors_t *s_errors)
{
    Elf32_Shdr *symtab = NULL;
    Elf32_Shdr *strtab = NULL;
    get_symtab_x86(data, &symtab, &strtab);
    if (symtab == NULL || strtab == NULL) {
        error_no_symbols(s_errors->binary, s_errors->filename);
        return;
    }
    fill_symbol_list_x86(data, symtab, strtab);
}