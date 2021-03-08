/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** Parser
*/

#include "my.h"

jmp_buf s_jumpBuffer;

void errors_prefix(char const *binary, char const *filename, int select)
{
    struct stat stats;
    fprintf(stderr, binary + 2);
    if (select == QUOTES) {
        fprintf(stderr, ": '");
        fprintf(stderr, filename);
        fprintf(stderr, "': ");
    }
    else {
        fprintf(stderr, ": ");
        fprintf(stderr, filename);
        fprintf(stderr, ": ");
    }
}

void error_no_file(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, QUOTES);
    fprintf(stderr, "No such file\n");
}

void error_not_recognized(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, NO_QUOTES);
    fprintf(stderr, "file format not recognized\n");
}

void error_truncated(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, NO_QUOTES);
    fprintf(stderr, "file truncated\n");
}

void check_file(char const *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) longjmp(s_jumpBuffer, NO_FILE);
    else {
        struct stat stats;
        Elf64_Ehdr *elf;
        stat(filename, &stats);
        elf = mmap(NULL, stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (stats.st_size > 3) {
            if (elf->e_ident[EI_MAG0] != 0x7f || elf->e_ident[EI_MAG1] != 'E'
            || elf->e_ident[EI_MAG2] != 'L' || elf->e_ident[EI_MAG0] != 'F')
                longjmp(s_jumpBuffer, NOT_RECOGNIZED);
        } else if (stats.st_size < 4 && stats.st_size != 0)
            longjmp(s_jumpBuffer, TRUNCATED);
        munmap(elf, stats.st_size);
        close(fd);
    }
}