/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** utils
*/

#include "my.h"

void shift_array_left(char *arr, int len)
{
    char tmp = arr[0];
    for (int i = 0; i < len - 1; i++)
        arr[i] = arr[i + 1];
    arr[len - 1] = tmp;
}

void get_obj_name(char *obj_name, int save, int k, char *data)
{
    shift_array_left(obj_name, 16);
    for (int i = save + atoi(obj_name); data[i] != '/'; i++, k++)
        obj_name[k] = data[i];
    obj_name[k] = '\0';
}

int find_archive_type(char *data, off_t fd_size)
{
    for (int i = 0; i < fd_size; i++) {
        if (i + 4 <= fd_size && data[i] == 0x7f && data[i + 1] == 'E'
        && data[i + 2] == 'L' && data[i + 3] == 'F')
            return (data[i + 4]);
    }
    longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    munmap(data, fd_size);
    return (0);
}