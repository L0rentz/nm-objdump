/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** nm_x64
*/

#include "my.h"

void fill_symbol_list_x64(void *data, Elf64_Shdr *symtab, Elf64_Shdr *strtab)
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
        list[len].type = get_symbol_type_x64(sym[i], shdr);
        list[len].name = str + sym[i].st_name;
        list[len].adr = sym[i].st_value;
        len++;
    }
    sort_symbol_list(&list, len);
    print_symbol_list(list, len, 0);
}

void get_symtab_x64(void *data, Elf64_Shdr **symtab, Elf64_Shdr **strtab)
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
    get_symtab_x64(data, &symtab, &strtab);
    if (symtab == NULL || strtab == NULL) {
        error_no_symbols(s_errors->binary, s_errors->filename);
        return;
    }
    fill_symbol_list_x64(data, symtab, strtab);
}