#ifndef BLOCKSPACE_H
#define BLOCKSPACE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t block_t;

typedef struct BlockList {
    block_t block_num;
    struct BlockList* next;
} BlockList;

typedef struct {
    char name[32];
    size_t size;
    BlockList* blocks;
} FileEntry;

typedef struct {
    FILE* file;
    size_t block_size;
    size_t num_blocks;
    size_t free_blocks;
    BlockList* free_list;
    size_t data_offset;
    int in_transaction;
    
    FileEntry* files;
    size_t file_count;
} BlockSpace;

BlockSpace* block_space_init(FILE* file, size_t block_size, size_t num_blocks);
int allocate_blocks(BlockSpace* bs, size_t count, block_t* result);
int free_blocks(BlockSpace* bs, block_t* blocks, size_t count);
int write_blocks(BlockSpace* bs, block_t* blocks, size_t count, const void* data);
int read_blocks(BlockSpace* bs, block_t* blocks, size_t count, void* data);

int create_file(BlockSpace* bs, const char* name, size_t size);
int write_file(BlockSpace* bs, const char* name, const void* data, size_t size);
FileEntry* find_file(BlockSpace* bs, const char* name);

void find_most_fragmented_file(BlockSpace* bs);

#endif
