#ifndef MALLOC_TUTORIAL_H
#define MALLOC_TUTORIAL_H

typedef struct s_block* t_block;

#define align4(x) (((((x) - 1) >> 2) << 2) + 4)

#define BLOCK_SIZE 12

struct s_block {
    size_t size;
    t_block next;
    t_block prev;
    int free;
    void* ptr;
    char data[1];
};

void* true_malloc(size_t size);
void* calloc(size_t number, size_t size);
void free(void* p);

#endif