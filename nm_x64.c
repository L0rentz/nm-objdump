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

void swap(symbolsInfos_t* a, symbolsInfos_t* b)
{
    symbolsInfos_t tmp = *a;
    *a = *b;
    *b = tmp;
}

int is_sorted(char *a, char *b)
{
    int i = 0, j = 0;
    for (; a[i] != '\0' && b[j] != '\0'; i++, j++) {
        for (; !isalpha(a[i]); i++);
        for (; !isalpha(b[j]); j++);
        if (toupper(a[i]) > toupper(b[j])) return (0);
        else if (toupper(a[i]) == toupper(b[j])) continue;
        else return (1);
    }
    if (a[i] == '\0' && b[j] == '\0' && j > i) return (0);
    else if (a[i] != '\0' && b[j] == '\0') return (0);
    return (1);
}

void sort_symbol_list(symbolsInfos_t (*list)[], size_t len)
{
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (!is_sorted((*list)[j].name, (*list)[j + 1].name)) {
                swap((*list) + j, (*list) + j + 1);
            }
        }
    }
}

void print_symbol_list(symbolsInfos_t *list, size_t len)
{
    char adr_tmp[len][24];
    int max_adr_len = 16;
    for (size_t i = 0; i < len; i++)
        sprintf(adr_tmp[i], "%x", list[i].adr);
    for (size_t i = 0, spacer = 0; i < len; i++) {
        spacer = max_adr_len - strlen(adr_tmp[i]);
        if (strlen(adr_tmp[i]) > 0 && adr_tmp[i][0] != '0')
            printf("%0*d%s %c %s\n",
                spacer, 0, adr_tmp[i], list[i].type, list[i].name);
        else
            printf("%*s %c %s\n", spacer + 1, "", list[i].type, list[i].name);
    }
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

void nm_x64(char *filename)
{
    int fd = open(filename, O_RDONLY);
    struct stat fd_stat;
    stat(filename, &fd_stat);
    void *data =
        mmap(NULL, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    Elf64_Shdr *symtab = NULL;
    Elf64_Shdr *strtab = NULL;
    get_symtab(data, &symtab, &strtab);
    fill_symbol_list(data, symtab, strtab);
    munmap(data, fd_stat.st_size);
    close(fd);
}