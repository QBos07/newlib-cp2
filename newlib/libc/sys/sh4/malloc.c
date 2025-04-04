#include <reent.h>
#include <stdint.h>
#include <string.h>

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
    
    if (new_size > 0) 
    {
        block = _malloc_r(reent, new_size);

        if (!block) 
        {
            return NULL;
        }
    }

    if (!ptr)
    {
        return block;
    }

    /* Right before the data there is a long keeping track of the size */
    memcpy(block, ptr, MIN(*(((unsigned long *)ptr)-1), new_size));
    free(ptr);
    
    return block;
}

void *
realloc (void *ptr, 
    size_t size) 
{
    return _realloc_r(_REENT, ptr, size);
}
