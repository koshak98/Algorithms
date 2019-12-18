/*
 * =====================================================================================
 *
 *       Filename:  phy08-0.c
 *
 *    Description:  all substring appearings 
 *
 *        Version:  1.0
 *        Created:  19.11.2019 22:07:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Maxim Klechshev
 *   Organization:  MIPT
 *
 * =====================================================================================
 */
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char** argv) {
        char* fn = argv[1];
        char* pattern = argv[2];
        int fd = open(fn, O_RDONLY);
        if (fd == -1) {
            perror("failed open");
            return 2;
        }
        int size = lseek(fd, 0, SEEK_END);
        char* text = mmap(NULL, size + 1, PROT_READ, MAP_SHARED, fd, 0);

        if ((int64_t)(pattern - text) < 0) return 1;

        char* cur_found_substring = strstr(text, pattern);
        if (cur_found_substring == NULL) {
                return 0;
        }
        if ((int64_t)text == -1) {
                return 1;
        }
        while (cur_found_substring != NULL && (uint64_t)(cur_found_substring - text + 1) < size && cur_found_substring + 1 < text + size) {
                printf("%llu\n", (uint64_t)(cur_found_substring - text));
                cur_found_substring = strstr(cur_found_substring + 1, pattern);
                if (cur_found_substring == NULL) {
                        return 0;
                }
        }
        munmap(text, size);
        close(fd);
        return 0;
}