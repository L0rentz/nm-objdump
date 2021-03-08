/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** main
*/

#include "my.h"

int main(int ac, char **av)
{
    void (*p[4])(char const *binary, char const *filename);
    p[0] = NULL;
    p[1] = error_no_file;
    p[2] = error_not_recognized;
    p[3] = error_truncated;
    for (int i = 1, val = 0; i < ac; i++) {
        if (val = setjmp(s_jumpBuffer)) p[val](av[0], av[i]);
        else {
            check_file(av[i]);
            nm_x64(av[i]);
        }
    }
    return (0);
}