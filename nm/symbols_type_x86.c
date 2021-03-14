/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** symbols_type_x86
*/

#include "my.h"

char get_symbol_type_next_x86(Elf32_Sym sym, Elf32_Shdr *shdr, char c)
{
    if (shdr[sym.st_shndx].sh_type == SHT_NOBITS && c == '?'
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) c = 'B';
    if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && c == '?'
        && shdr[sym.st_shndx].sh_flags == SHF_ALLOC) c = 'R';
    if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && c == '?'
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) c = 'D';
    if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && c == '?'
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) c = 'T';
    if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC && c == '?') c = 'D';
    if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && c != '?') c += 32;
    if (c == '?' && ELF32_ST_BIND(sym.st_info) == 0
    && ELF32_ST_TYPE(sym.st_info) == 0 && shdr[sym.st_shndx].sh_type == 17
    && shdr[sym.st_shndx].sh_flags == 0) c = 'n';
    return (c);
}

char get_symbol_type_x86(Elf32_Sym sym, Elf32_Shdr *shdr)
{
    char c = '?';
    if (ELF32_ST_BIND(sym.st_info) == STB_GNU_UNIQUE) c = 'u';
    if (ELF32_ST_BIND(sym.st_info) == STB_WEAK && c == '?') {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF) c = 'w';
    }
    if (ELF32_ST_BIND(sym.st_info) == STB_WEAK
        && ELF32_ST_TYPE(sym.st_info) == STT_OBJECT && c == '?') {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF) c = 'v';
    }
    if (sym.st_shndx == SHN_UNDEF && c == '?') c = 'U';
    if (sym.st_shndx == SHN_ABS && c == '?') c = 'A';
    if (sym.st_shndx == SHN_COMMON && c == '?') c = 'C';
    if (c == '?') c = get_symbol_type_next_x86(sym, shdr, c);
    if (c == '?') c = 't';
    return (c);
}