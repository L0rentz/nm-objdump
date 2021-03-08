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

int check_archive(char const *filename)
{
    char *line = NULL;
    size_t len = 0;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) fclose(fp), longjmp(s_jumpBuffer, NO_FILE);
    if (getline(&line, &len, fp) == -1)
        fclose(fp), longjmp(s_jumpBuffer, NO_FILE);
    fclose(fp);
    if (strlen(line) == 8 && strcmp("!<arch>\n", line) == 0) return (1);
    return (0);
}

int check_file(char const *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) close(fd), longjmp(s_jumpBuffer, NO_FILE);
    if (check_archive(filename)) {
        close(fd);
        return (1);
    }
    struct stat fd_stat;
    Elf64_Ehdr *elf;
    stat(filename, &fd_stat);
    elf = mmap(NULL, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (fd_stat.st_size > 3) {
        if (elf->e_ident[EI_MAG0] != 0x7f || elf->e_ident[EI_MAG1] != 'E'
        || elf->e_ident[EI_MAG2] != 'L' || elf->e_ident[EI_MAG3] != 'F') {
            munmap(elf, fd_stat.st_size), close(fd);
            longjmp(s_jumpBuffer, NOT_RECOGNIZED);
        }
    }
    munmap(elf, fd_stat.st_size);
    if (fd_stat.st_size < 4 && fd_stat.st_size != 0)
        longjmp(s_jumpBuffer, TRUNCATED);
    return (0);
}