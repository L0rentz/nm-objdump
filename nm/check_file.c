/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** Parser
*/

#include "my.h"

jmp_buf s_jumpBuffer;

int check_archive(char const *filename)
{
    char *line = NULL;
    size_t len = 0;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) longjmp(s_jumpBuffer, NO_FILE);
    if (getline(&line, &len, fp) == -1)
        fclose(fp), longjmp(s_jumpBuffer, NO_FILE);
    fclose(fp);
    if (strlen(line) == 8 && strcmp("!<arch>\n", line) == 0) {
        free(line);
        return (1);
    }
    return (0);
}

int check_file(char const *filename)
{
    if (check_archive(filename)) return (1);
    int fd = open(filename, O_RDONLY);
    if (fd == -1) longjmp(s_jumpBuffer, NO_FILE);
    Elf64_Ehdr *elf;
    off_t fd_size = lseek(fd, 0, SEEK_END);
    elf = mmap(NULL, fd_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (fd_size > 3)
        if (elf->e_ident[EI_MAG0] != 0x7f || elf->e_ident[EI_MAG1] != 'E'
        || elf->e_ident[EI_MAG2] != 'L' || elf->e_ident[EI_MAG3] != 'F') {
            munmap(elf, fd_size), close(fd);
            longjmp(s_jumpBuffer, NOT_RECOGNIZED);
        }
    if ((int)(elf->e_shnum * elf->e_shentsize + elf->e_shoff) <= 0)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    if ((fd_size < 4 && fd_size != 0)
        || elf->e_shnum * elf->e_shentsize + elf->e_shoff != fd_size)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, TRUNCATED);
    munmap(elf, fd_size);
    return (0);
}