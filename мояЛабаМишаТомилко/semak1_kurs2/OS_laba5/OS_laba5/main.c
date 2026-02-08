#include <stdio.h>
#include "blockspace.h"

int main() {
    remove("storage.bin");
    FILE* f = fopen("storage.bin", "wb+");
    if (!f) {
        perror("fopen");
        return 1;
    }

    BlockSpace* bs = block_space_init(f, 1024, 100);
    bs->files = NULL;
    bs->file_count = 0;

    for (int i = 0; i < 80; i++) {
        char name[32];
        sprintf(name, "temp_%d.txt", i);
        create_file(bs, name, 1024);
    }

    for (int i = 0; i < 80; i += 2) {
        char name[32];
        sprintf(name, "temp_%d.txt", i);
        FileEntry* file = find_file(bs, name);
        if (file) {
            BlockList* bl = file->blocks;
            block_t blocks_to_free[100];
            int cnt = 0;
            while (bl) {
                blocks_to_free[cnt++] = bl->block_num;
                BlockList* next = bl->next;
                free(bl);
                bl = next;
            }
            free_blocks(bs, blocks_to_free, cnt);

            for (size_t j = 0; j < bs->file_count; j++) {
                if (strcmp(bs->files[j].name, name) == 0) {
                    bs->files[j] = bs->files[--bs->file_count];
                    break;
                }
            }
        }
    }

    create_file(bs, "big_fragmented.log", 40 * 1024);  // 40 блоков

    find_most_fragmented_file(bs);

    fclose(f);
    return 0;
}
