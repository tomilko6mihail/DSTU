#include "blockspace.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

BlockSpace* block_space_init(FILE* file, size_t block_size, size_t num_blocks) {
    BlockSpace* bs = malloc(sizeof(BlockSpace));
    if (!bs) return NULL;

    bs->file = file;
    bs->block_size = block_size;
    bs->num_blocks = num_blocks;
    bs->free_blocks = num_blocks;
    bs->free_list = NULL;
    bs->in_transaction = 0;
    bs->data_offset = 0;
    bs->files = NULL;
    bs->file_count = 0;

    for (size_t i = 0; i < num_blocks; i++) {
        BlockList* b = malloc(sizeof(BlockList));
        b->block_num = i;
        b->next = bs->free_list;
        bs->free_list = b;
    }
    return bs;
}

int allocate_blocks(BlockSpace* bs, size_t count, block_t* result) {
    if (bs->free_blocks < count) return -1;
    for (size_t i = 0; i < count; i++) {
        BlockList* b = bs->free_list;
        result[i] = b->block_num;
        bs->free_list = b->next;
        free(b);
        bs->free_blocks--;
    }
    return 0;
}

int free_blocks(BlockSpace* bs, block_t* blocks, size_t count) {
    for (size_t i = 0; i < count; i++) {
        BlockList* b = malloc(sizeof(BlockList));
        b->block_num = blocks[i];
        b->next = bs->free_list;
        bs->free_list = b;
        bs->free_blocks++;
    }
    return 0;
}
FileEntry* find_file(BlockSpace* bs, const char* name) {
    for (size_t i = 0; i < bs->file_count; i++) {
        if (strcmp(bs->files[i].name, name) == 0) {
            return &bs->files[i];
        }
    }
    return NULL;
}

int write_blocks(BlockSpace* bs, block_t* blocks, size_t count, const void* data) {
    const char* ptr = (const char*)data;
    for (size_t i = 0; i < count; i++) {
        fseek(bs->file, bs->data_offset + blocks[i] * bs->block_size, SEEK_SET);
        fwrite(ptr + i * bs->block_size, bs->block_size, 1, bs->file);
    }
    return 0;
}

int read_blocks(BlockSpace* bs, block_t* blocks, size_t count, void* data) {
    char* ptr = (char*)data;
    for (size_t i = 0; i < count; i++) {
        fseek(bs->file, bs->data_offset + blocks[i] * bs->block_size, SEEK_SET);
        fread(ptr + i * bs->block_size, bs->block_size, 1, bs->file);
    }
    return 0;
}

int create_file(BlockSpace* bs, const char* name, size_t size) {
    if (strlen(name) >= 32) return -1;
    for (size_t i = 0; i < bs->file_count; i++) {
        if (strcmp(bs->files[i].name, name) == 0) return -1;
    }

    size_t needed = (size + bs->block_size - 1) / bs->block_size;
    block_t* blocks = malloc(needed * sizeof(block_t));
    if (allocate_blocks(bs, needed, blocks) != 0) {
        free(blocks);
        return -1;
    }

    bs->files = realloc(bs->files, (bs->file_count + 1) * sizeof(FileEntry));
    FileEntry* f = &bs->files[bs->file_count++];
    strncpy(f->name, name, 31);
    f->name[31] = '\0';
    f->size = size;
    f->blocks = NULL;

    for (size_t i = 0; i < needed; i++) {
        BlockList* bl = malloc(sizeof(BlockList));
        bl->block_num = blocks[i];
        bl->next = f->blocks;
        f->blocks = bl;
    }
    free(blocks);
    return 0;
}

void find_most_fragmented_file(BlockSpace* bs) {
    if (bs->file_count == 0) {
        printf("Нет файлов.\n");
        return;
    }

    const char* worst = NULL;
    long long max_span = 0;

    for (size_t i = 0; i < bs->file_count; i++) {
        FileEntry* f = &bs->files[i];
        if (!f->blocks || !f->blocks->next) continue;

        block_t min_b = f->blocks->block_num;
        block_t max_b = f->blocks->block_num;
        BlockList* cur = f->blocks;
        while (cur) {
            if (cur->block_num < min_b) min_b = cur->block_num;
            if (cur->block_num > max_b) max_b = cur->block_num;
            cur = cur->next;
        }

        long long span = (long long)(max_b - min_b) * bs->block_size;

        if (span >= (long long)f->size && span > max_span) {
            max_span = span;
            worst = f->name;
        }
    }

    if (worst) {
        printf("Самый фрагментированный файл: %s\n", worst);
        printf("Расстояние: %lld байт\n", max_span);
    } else {
        printf("Не найдено сильно фрагментированных файлов.\n");
    }
}
