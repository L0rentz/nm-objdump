/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** main
*/

#include "my.h"

void nm_selector(char *filename)
{
    int fd = open(filename, O_RDONLY);
    struct stat fd_stat;
    stat(filename, &fd_stat);
    void *data =
        mmap(NULL, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (check_file(filename)) {
        printf("!<arch>\n");
    } else
        nm_x64(data);
    munmap(data, fd_stat.st_size);
    close(fd);
}

int main(int ac, char **av)
{
    void (*p[4])(char const *binary, char const *filename);
    p[0] = NULL;
    p[1] = error_no_file;
    p[2] = error_not_recognized;
    p[3] = error_truncated;
    int val = 0;
    for (int i = 1; i < ac; i++) {
        if (val = setjmp(s_jumpBuffer)) p[val](av[0], av[i]);
        else {
            if (ac > 2) printf("\n%s:\n", av[i]);
            nm_selector(av[i]);
        }
    }
    if (ac == 1) {
        if (val = setjmp(s_jumpBuffer)) p[val](av[0], "a.out");
        else nm_selector("a.out");
    }
    return (0);
}