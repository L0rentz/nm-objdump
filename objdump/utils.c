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

void flags_writer(unsigned int flags)
{
    char *comma = "";
    int flags_array[9] = { HAS_RELOC, EXEC_P, HAS_LINENO, HAS_DEBUG, HAS_SYMS,
        HAS_LOCALS, DYNAMIC, WP_TEXT, D_PAGED };
    char *strings_array[] = { "HAS_RELOC", "EXEC_P",
        "HAS_LINENO", "HAS_DEBUG", "HAS_SYMS", "HAS_LOCALS",
        "DYNAMIC", "WP_TEXT", "D_PAGED" };
    for (int i = 0; i < 9; i++) {
        if (flags & flags_array[i]) {
            printf("%s%s", comma, strings_array[i]);
            comma = ", ";
        }
    }
    printf("\n");
}

void hexa_display(unsigned char *data, int size)
{
    for (int i = 0; i < 16; i++) {
        if (i < size) printf("%02x", data[i]);
        else printf("  ");
        if (!((i + 1) % 4)) printf(" ");
    }
    printf(" ");
    for (int i = 0; i < 16; i++) {
        if (i < size) printf("%c", PRINTABLE(data[i]) ? data[i] : '.');
        else printf(" ");
    }
}