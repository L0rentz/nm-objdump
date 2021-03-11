/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** main
*/

#include "my.h"

void nm_selector(errors_t *s_errors, int ac)
{
    int fd = open(s_errors->filename, O_RDONLY);
    if (fd == -1) longjmp(s_jumpBuffer, NO_FILE);
    struct stat fd_stat;
    stat(s_errors->filename, &fd_stat);
    void *data =
        mmap(NULL, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (check_file(s_errors->filename)) {
        parse_archive(s_errors);
    } else {
        if (ac > 2) printf("\n%s:\n", s_errors->filename);
        nm_x64(data);
    }
    munmap(data, fd_stat.st_size);
    close(fd);
}

int main(int ac, char **av)
{
    void (*p[5])(char const *binary, char const *filename);
    p[0] = NULL;
    p[1] = error_no_file;
    p[2] = error_not_recognized;
    p[3] = error_truncated;
    p[4] = error_malformed;
    int val = 0, err = 0;
    errors_t s_errors;
    s_errors.binary = av[0];
    for (int i = 1; i < ac; i++) {
        if (val = setjmp(s_jumpBuffer)) p[val](av[0], av[i]);
        else {
            s_errors.filename = av[i];
            nm_selector(&s_errors, ac);
        }
    }
    if (ac == 1) {
        if (val = setjmp(s_jumpBuffer)) p[val](av[0], "a.out");
        else nm_selector(&s_errors, ac);
    }
    return (0);
}