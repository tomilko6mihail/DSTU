#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

void copy_file(const char *src, const char *dst) {
    FILE *src_file = fopen(src, "rb");
    if (!src_file) {
        perror("Ошибка открытия исходного файла");
        return;
    }
    
    FILE *dst_file = fopen(dst, "wb");
    if (!dst_file) {
        perror("Ошибка открытия целевого файла");
        fclose(src_file);
        return;
    }
    
    char buffer[4096];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        fwrite(buffer, 1, bytes, dst_file);
    }
    
    fclose(src_file);
    fclose(dst_file);
    
    // Копируем права доступа
    struct stat st;
    if (stat(src, &st) == 0) {
        chmod(dst, st.st_mode);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <исходная_директория> <целевая_директория>\n", argv[0]);
        return 1;
    }
    
    const char *src_dir = argv[1];
    const char *dst_dir = argv[2];
    
    DIR *dir = opendir(src_dir);
    if (dir == NULL) {
        perror("Ошибка открытия исходной директории");
        return 1;
    }
    
    // Проверяем существование целевой директории, создаем если нет
    struct stat st;
    if (stat(dst_dir, &st) == -1) {
        if (mkdir(dst_dir, 0755) == -1) {
            perror("Ошибка создания целевой директории");
            closedir(dir);
            return 1;
        }
    }
    
    struct dirent *entry;
    int file_count = 0;
    pid_t pids[100]; // Максимум 100 файлов
    
    printf("Копирование из %s в %s\n", src_dir, dst_dir);
    
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем . и ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        char src_path[1024];
        char dst_path[1024];
        
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst_dir, entry->d_name);
        
        // Проверяем, что это файл, а не директория
        if (stat(src_path, &st) == 0 && S_ISREG(st.st_mode)) {
            pid_t pid = fork();
            
            if (pid == 0) {
                // Дочерний процесс - копируем файл
                printf("Процесс %d копирует: %s\n", getpid(), entry->d_name);
                copy_file(src_path, dst_path);
                exit(0);
            } else if (pid > 0) {
                // Родительский процесс - сохраняем PID
                if (file_count < 100) {
                    pids[file_count++] = pid;
                }
            } else {
                perror("Ошибка создания процесса");
            }
        }
    }
    
    closedir(dir);
    
    // Ждем завершения всех дочерних процессов
    for (int i = 0; i < file_count; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    printf("Копирование завершено. Скопировано файлов: %d\n", file_count);
    
    return 0;
}
