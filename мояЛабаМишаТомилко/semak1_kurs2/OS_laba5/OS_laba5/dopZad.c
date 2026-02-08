#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifndef F_LOG2PHYS
#define F_LOG2PHYS 49
#endif


long long max_span = 0;
char most_fragmented_file[4096] = "";

void process_file(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        return;
    }

    struct stat st;
    if (fstat(fd, &st) != 0 || st.st_size == 0 || !S_ISREG(st.st_mode)) {
        close(fd);
        return;
    }

    long long min_offset = -1;
    long long max_end = -1;
    off_t offset = 0;

    while (offset < st.st_size) {
        struct log2phys l2p;
        if (lseek(fd, offset, SEEK_SET) == -1) break;

        if (fcntl(fd, F_LOG2PHYS, &l2p) == -1) {
            break;
        }

        if (l2p.l2p_contigbytes <= 0) {
            break;
        }

        long long start = l2p.l2p_devoffset;
        long long end = start + l2p.l2p_contigbytes;

        if (min_offset == -1 || start < min_offset) {
            min_offset = start;
        }
        if (end > max_end) {
            max_end = end;
        }

        offset += l2p.l2p_contigbytes;
    }

    close(fd);

    if (min_offset != -1 && max_end > min_offset) {
        long long span = max_end - min_offset;

        if (span > (long long)st.st_size * 2 && span > max_span) {
            max_span = span;
            strncpy(most_fragmented_file, path, sizeof(most_fragmented_file) - 1);
            most_fragmented_file[sizeof(most_fragmented_file) - 1] = '\0';
        }
    }
}

void scan_directory(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char path[4096];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (lstat(path, &st) == -1) continue;

        if (S_ISDIR(st.st_mode)) {
            scan_directory(path);
        } else if (S_ISREG(st.st_mode)) {
            process_file(path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s /путь/к/директории\n", argv[0]);
        return 1;
    }

    const char *root = argv[1];

    printf("Сканирование директории: %s\n", root);
    scan_directory(root);

    if (max_span > 0 && most_fragmented_file[0] != '\0') {
        printf("\nСамый раздробленный файл:\n%s\n", most_fragmented_file);
        printf("Расстояние между первым и последним блоком: %lld байт\n", max_span);
    } else {
        printf("\nНе найдено сильно фрагментированных файлов.\n");
    }

    return 0;
}
