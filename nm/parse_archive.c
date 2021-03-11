/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** parse_archive
*/

#include "my.h"

jmp_buf s_jumpBuffer;

void shift_array_left(char *arr, int len)
{
    char tmp = arr[0];
    for (int i = 0; i < len - 1; i++)
        arr[i] = arr[i + 1];
    arr[len - 1] = tmp;
}

int check_format_x64(char *data, int offset,
    size_t fd_size, errors_t *s_errors)
{
    Elf64_Ehdr *elf = (Elf64_Ehdr *)&data[offset];
    if ((elf->e_shnum * elf->e_shentsize + elf->e_shoff
    != s_errors->next_obj - offset || data[s_errors->next_obj + 58] != 0x60
    || data[s_errors->next_obj + 59] != 0x0A) && s_errors->next_obj < fd_size) {
        error_not_recognized(s_errors->binary, s_errors->obj_name);
        return (1);
    }
    return (0);
}

int check_format_common(char *data, int offset,
    size_t fd_size, errors_t *s_errors)
{
    if (fd_size < offset
    || (data[offset - 1] != 0x0A && data[offset - 2] != 0x60)) {
        munmap(data, fd_size);
        error_malformed(s_errors->binary, s_errors->filename);
        exit(84);
    }
    if (fd_size < offset + 3) {
        error_truncated(s_errors->binary, s_errors->obj_name);
        return (1);
    }
    if (data[offset] != 0x7f || data[offset + 1] != 'E'
    || data[offset + 2] != 'L' || data[offset + 3] != 'F') {
        error_not_recognized(s_errors->binary, s_errors->obj_name);
        return (1);
    }
    if (check_format_x64(data, offset, fd_size, s_errors))
        return (1);
    return (0);
}

void archive_offset_loop(struct stat fd_stat, char *data, errors_t *s_errors)
{
    int offset = 8, j = 0, save = 0, k = 0;
    for (char obj_name[255], size[11], i = 0; offset < fd_stat.st_size; i++) {
        obj_name[16] = '\0', size[10] = '\0', k = 0;
        if (offset != 8) offset += atoi(size);
        for (int i = 0; i < 16; i++) obj_name[i] = data[offset + i];
        offset += 60;
        for (int i = 0; i < 10; i++) size[i] = data[offset - 12 + i];
        if (i == 1) save = offset;
        if (strlen(size) > 0 && i > 1) {
            for (j = 0; obj_name[j] != '\0' && obj_name[j] != '/'; j++);
            obj_name[j] = '\0';
            if (j == 0) {
                shift_array_left(obj_name, 16);
                for (int i = save + atoi(obj_name); data[i] != '/'; i++, k++)
                    obj_name[k] = data[i];
                obj_name[k] = '\0';
            }
            s_errors->obj_name = (char *)&obj_name;
            //printf("----- %x %x %x %c %c %c\n", data[offset - 2], data[offset - 1], data[offset], data[offset + 1], data[offset + 2], data[offset + 3]);
            s_errors->next_obj = offset + atoi(size);
            if (check_format_common(data, offset, fd_stat.st_size, s_errors))
                continue;
            printf("\n%s:\n", obj_name);
            nm_x64(&data[offset], s_errors);
        }
    }
}

void parse_archive(errors_t *s_errors)
{
    int fd = open(s_errors->filename, O_RDONLY);
    struct stat fd_stat;
    stat(s_errors->filename, &fd_stat);
    if (fd_stat.st_size < 68) close(fd), longjmp(s_jumpBuffer, NOT_RECOGNIZED);
    char *data = mmap(NULL, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    archive_offset_loop(fd_stat, data, s_errors);
    munmap(data, fd_stat.st_size);
    close(fd);
}