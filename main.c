#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct s_block* t_block;

#define align4(x) (((((x) -1) > >2) < <2)+4)

#define BLOCK_SIZE sizeof(struct s_block)

struct s_block {
    size_t size;
    t_block next;
    int free;
    char data[1];
};



void* dummyMalloc(size_t size) {
    void* p;

    p = sbrk(0);
    if (sbrk(size) == (void*)-1) {
        return NULL;
    }

    return p;
}

t_block find_block(t_block* last, size_t size) {
    t_block b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b
    }
}

t_block extend_heap(t_block last, size_t s) {
    t_block b;
    b = sbrk(0);
    if (sbrk(BLOCK_SIZE + s) == (void*)-1) {
        return NULL;
    }
    b->size = s;
    b->next = NULL;
    if (last) {
        last->next = b;
    }
    b->free = 0;
    return (b);
}

int main() {
    int* arr;
    size_t n = 10;

    arr = (int*)dummyMalloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        arr[i] = i;
    }

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}