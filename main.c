#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct s_block* t_block;

#define align4(x) (((((x) - 1) >> 2) << 2) + 4)

#define BLOCK_SIZE 12

void* base = NULL;

struct s_block {
    size_t size;
    t_block next;
    t_block prev;
    int free;
    void* ptr;
    char data[1];
};

t_block get_block(void* p) {
    char* tmp;
    tmp = p;
    return (p = tmp -= BLOCK_SIZE);
}

/* Valid adress for free */
int valid_addr(void* p) {
    if (base) {
        if (p > base && p < sbrk(0)) {
            return (p == (get_block(p))->ptr);
        }
    }
    return (0);
}

void split_block(t_block b, size_t s) {
    t_block new;
    new = (t_block)(b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->free = 1;
    b->size = s;
    b->next = new;
    return NULL;
}

t_block find_block(t_block* last, size_t size) {
    t_block b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
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

void* dummyMalloc(size_t size) {
    void* p;

    p = sbrk(0);
    if (sbrk(size) == (void*)-1) {
        return NULL;
    }

    return p;
}

void* true_malloc(size_t size) {
    t_block b, last;
    size_t s;
    s = align4(size);
    if (base) {
        // First find a block
        last = base;
        b = find_block(&last, s);
        if (b) {
            // Can we split
            if ((b->size - s) >= (BLOCK_SIZE + 4)) {
                split_block(b, s);
            }
            b->free = 0;
        }
        else {
            // No fitting block, extend the heap
            b = extend_heap(last, s);
            if (!b) {
                return (NULL);
            }
        }
    }
    else {
        // First use of malloc
        b = extend_heap(NULL, s);
        if (!b) {
            return (NULL);
        }
        base = b;
    }
    return (b->data);
}

void* calloc(size_t number, size_t size) {
    size_t* new;
    size_t s4, i;
    new = malloc(number * size);
    if (new) {
        s4 = align4(number * size) << 2;
        for (int i = 0; i < s4; i++) {
            new[i] = 0;
        }
    }
    return (new);
}

t_block fusion(t_block b) {
    if (b->next && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next) {
            b->next->prev = b;
        }
    }
    return (b);
}

void free(void* p) {
    t_block b;
    if (valid_addr(p)) {
        b = get_block(p);
        b->free = 1;
        if (b->prev && b->prev->free) {
            b = fusion(b->prev);
        }
        if (b->next) {
            fusion(b);
        }
        else {
            if (b->prev) {
                b->prev->next = NULL;
            }
            else {
                base = NULL;
            }
            brk(b);
        }
    }
}

int main() {
    int* arr;
    size_t n = 10;

    arr = true_malloc(n * sizeof(int));
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

    // Using calloc
    int* arr_calloc = calloc(n, sizeof(int));
    if (arr_calloc == NULL) {
        printf("Calloc failed\n");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr_calloc[i]);
    }
    printf("\n");

    // Freeing memory (dummy implementation)
    // Normally, you would implement a free function to handle this
    // Here we just demonstrate the fusion function
    t_block block = (t_block)((char*)arr - sizeof(struct s_block));
    block->free = 1;
    fusion(block);

    return 0;
}