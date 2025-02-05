#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct s_block* t_block;

struct s_block {
    size_t size;
    t_block next;
    int free;
};


void* dummyMalloc(size_t size) {
    void* p;

    p = sbrk(0);
    if (sbrk(size) == (void*)-1) {
        return NULL;
    }

    return p;
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