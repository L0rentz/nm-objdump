/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** Parser
*/

#include "my.h"

jmp_buf s_jumpBuffer = {0, 0, 0};

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

int check_x64(Elf64_Ehdr *elf, off_t fd_size)
{
    if (fd_size < 64)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    if ((int)(elf->e_shnum * elf->e_shentsize + elf->e_shoff) <= 0)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    if (elf->e_shnum * elf->e_shentsize + elf->e_shoff != fd_size)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, TRUNCATED);
    return (0);
}

int check_x86(Elf32_Ehdr *elf, off_t fd_size)
{
    if (fd_size < 52)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    if ((int)(elf->e_shnum * elf->e_shentsize + elf->e_shoff) <= 0)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    if (elf->e_shnum * elf->e_shentsize + elf->e_shoff != fd_size)
        munmap(elf, fd_size), longjmp(s_jumpBuffer, TRUNCATED);
    return (0);
}

void check_dir(char const *filename)
{
    DIR *dir = opendir(filename);
    if (dir != NULL)
        longjmp(s_jumpBuffer, IS_A_DIR);
}

int check_file(char const *filename, int *type)
{
    check_dir(filename);
    if (check_archive(filename)) return (1);
    int fd = open(filename, O_RDONLY);
    if (fd == -1) longjmp(s_jumpBuffer, NO_FILE);
    off_t fd_size = lseek(fd, 0, SEEK_END);
    char *data = mmap(NULL, fd_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (fd_size < 5 && fd_size != 0)
        munmap(data, fd_size), longjmp(s_jumpBuffer, TRUNCATED);
    if (data[EI_MAG0] != 0x7f || data[EI_MAG1] != 'E'
    || data[EI_MAG2] != 'L' || data[EI_MAG3] != 'F')
        munmap(data, fd_size), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    if (data[4] == 2) check_x64((Elf64_Ehdr *)data, fd_size);
    else if (data[4] == 1) check_x86((Elf32_Ehdr *)data, fd_size);
    else munmap(data, fd_size), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    *type = data[4];
    munmap(data, fd_size);
    return (0);
}