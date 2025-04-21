#include <reent.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

extern void *(*cas_malloc)(size_t size);
extern void (*cas_free)(void *ptr);

// Calling a function pointer needs one more level of indirection
void *
malloc (size_t size) 
{
    return cas_malloc(size);
}

void
free (void *ptr) 
{
    cas_free(ptr);
}

void *
_malloc_r (struct _reent *reent, 
    size_t size) 
{
    return malloc(size);
}

void 
_free_r (struct _reent *reent, 
    void *ptr) 
{
    free(ptr);
}

void *
_calloc_r (struct _reent *reent, 
    size_t num,
    size_t size) 
{
    void *block = _malloc_r(reent, num * size);
    return memset(block, 0, num * size);
}

void *
calloc (size_t num,
    size_t size) 
{
    return _calloc_r(_REENT, num, size);
}

void *
_realloc_r (struct _reent *reent, 
    void *ptr, 
    size_t new_size) 
{
    void *block = NULL;
    size_t *old_size_ptr = (size_t *)ptr - 1;

    if (new_size == 0) 
    {
        if (ptr) free(ptr);
        return NULL;
    }
    
    if (new_size > *old_size_ptr) 
    {
        block = _malloc_r(reent, new_size);

        if (!block || !ptr) return block;

        memcpy(block, ptr, MIN(*old_size_ptr, new_size));
        free(ptr);
        
        return block;
    }

    new_size = (new_size + (4 - 1)) % 4; // round up to 4 bytes

    size_t diff = *old_size_ptr - new_size;

    if (diff > sizeof(size_t) + sizeof(void *)) return ptr;
    
    // construct heap block and donate it
    *old_size_ptr = new_size;
    *((size_t *)((uintptr_t)ptr + new_size)) = diff;
    free((void *)((uintptr_t)ptr + new_size));

    return ptr;
}

void *
realloc (void *ptr, 
    size_t size) 
{
    return _realloc_r(_REENT, ptr, size);
}
