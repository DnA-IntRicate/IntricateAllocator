#include "intricate_allocator.h"
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef NOMINMAX
    #define NOMINMAX    
#endif // !NOMINMAX

#ifdef IA_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
        #include <Windows.h>
        #pragma comment(lib, "onecore.lib")
    #endif // !WIN32_LEAN_AND_MEAN
#endif // IA_PLATFORM_WINDOWS

#define IA_HEAP_PAGE_SIZE 4096ull

#ifdef IA_DEBUG
    #define IA_DEBUG_PRINT(...) printf(__VA_ARGS__)
    #define IA_DEBUG_ERROR(...) fprintf(stderr, __VA_ARGS__)
#else
    #define IA_DEBUG_PRINT(...)
    #define IA_DEBUG_ERROR(...)
#endif // IA_DEBUG

typedef struct heap_chunk_t
{
    size_t size;
    //size_t prev_size;
    bool in_use;
    struct heap_chunk_t* next;
} heap_chunk_t;

// Ehhhh?
heap_chunk_t* g_free_list = NULL;

// Do we need this?
struct heap_info_t
{
//    heap_chunk_t* start;
    heap_chunk_t* head;
    size_t avail_size;
} g_heap_info;


static heap_chunk_t* ia_os_alloc(size_t size)
{
    size_t chunk_size = size + sizeof(heap_chunk_t);

    heap_chunk_t* chunk = (heap_chunk_t*)VirtualAlloc2(GetCurrentProcess(), NULL, chunk_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL, 0);
    if (chunk == NULL)
    {
        IA_DEBUG_ERROR("Failed to allocate new chunk: %#x\n", GetLastError());
        return NULL;
    }

    chunk->size = size;
    chunk->in_use = false;
    chunk->next = NULL;

    return chunk;
}

static bool ia_heap_init(size_t init_size)
{
    IA_DEBUG_PRINT("Initializing...\n");
    IA_DEBUG_PRINT("Heap size: %zu bytes\n", init_size);

    void* heap_start = VirtualAlloc2(GetCurrentProcess(), NULL, init_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL, 0);
    if (heap_start == NULL)
    {
        IA_DEBUG_ERROR("Failed to initialize heap: %#x\n", GetLastError());
        return false;
    }
    
    heap_chunk_t* first = (heap_chunk_t*)heap_start;
    first->size = init_size - sizeof(heap_chunk_t*);
    first->in_use = false;
    first->next = NULL;

    memset(&g_heap_info, 0, sizeof(struct heap_info_t));
    g_heap_info.head = first;
    g_heap_info.avail_size = first->size;

    return true;
}

static bool ia_heap_extend(size_t extension_size)
{
    printf("Extending heap by: %zu bytes\n", extension_size);

    void* extension_start = VirtualAlloc2(GetCurrentProcess(), NULL, extension_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL, 0);
    if (extension_start == NULL)
    {
        IA_DEBUG_ERROR("Failed to extend heap: %#x\n", GetLastError());
        return false;
    }

    // Initialize the new chunk
    heap_chunk_t* extension = (heap_chunk_t*)extension_start;
    extension->size = extension_size - sizeof(heap_chunk_t);
    extension->in_use = false;

    // Add the new chunk to the end of the free list
    heap_chunk_t* last_chunk = g_heap_info.head;    // Use head or start here?
    while (last_chunk->next) 
        last_chunk = last_chunk->next;
    
    last_chunk->next = extension;
    extension->next = NULL;
    g_heap_info.avail_size += extension_size;

    return true;
}

void* ia_alloc(size_t size)
{
    if (!g_heap_info.head)
        ia_heap_init(IA_HEAP_PAGE_SIZE);
    
    if ((g_heap_info.avail_size == 0) || (g_heap_info.avail_size < size))
    {
        size_t extension = IA_HEAP_PAGE_SIZE;
        while (extension < size)
            extension += IA_HEAP_PAGE_SIZE;

        if (!ia_heap_extend(extension))
            return NULL;
    }
 
    // Returns null if the requested allocation size exceeds the heaps available size.
    if (size > g_heap_info.avail_size)
    {
        IA_DEBUG_ERROR("No available heap space!");
        return NULL;
    }

    // Traverse the free-list for the first valid large-enough chunk from the head of the heap
    heap_chunk_t* chunk = g_heap_info.head;
    while ((chunk != NULL) && (size > chunk->size) && chunk->in_use)
        chunk = chunk->next;

    if (chunk == NULL)
    {
        IA_DEBUG_ERROR("No suitable chunk found!");
        return NULL;
    }

    // When the chunk is found, split it if there is excess space
    if (chunk->size > (size + sizeof(heap_chunk_t)))
    {
        // Find the next chunk by advancing the chunk pointer by size
        heap_chunk_t* next_chunk = (heap_chunk_t*)(((char*)chunk) + sizeof(heap_chunk_t) + size);
        next_chunk->size = chunk->size - size - sizeof(heap_chunk_t);
        next_chunk->in_use = false; // The next chunk should not be in use
        next_chunk->next = chunk->next; // Set the next chunk pointer of the linked list

        chunk->size = size;
        chunk->next = next_chunk; // Set the next pointer equal to the address of the next chunk.
    }
    else
    {
        // Find the next chunk by advancing the chunk pointer by size
        heap_chunk_t* next_chunk = (heap_chunk_t*)(((char*)chunk) + sizeof(heap_chunk_t) + size);
        next_chunk->size = g_heap_info.avail_size - sizeof(heap_chunk_t) - size;
        next_chunk->in_use = false; // The next chunk should not be in use
        next_chunk->next = chunk->next; // Set the next chunk pointer of the linked list

        chunk->size = size;
        chunk->next = next_chunk; // Set the next pointer equal to the address of the next chunk.
    }

    // The current chunk should be marked as in use so that it doesn't get unintentionally overwritten
    // in future calls of this function or before the chunk is freed.
    chunk->in_use = true;
    IA_DEBUG_PRINT("Found heap chunk %p of size: %zu bytes\n", chunk, chunk->size);

    // Set the head of the heap equal to the next available chunk found previously
    g_heap_info.head = chunk->next;
    g_heap_info.avail_size -= chunk->size + sizeof(heap_chunk_t);

    // Return a pointer to the allocated block (just after the chunk metadata)
    return (void*)(((char*)chunk) + sizeof(heap_chunk_t));
}

//static void ia_merge_free_chunks(heap_chunk_t* chunk) {
//    while (chunk->next != NULLPTR && !chunk->next->in_use) {
//        chunk->size += sizeof(heap_chunk_t) + chunk->next->size;
//        chunk->next = chunk->next->next;
//    }
//}

// This is going to end up causing a very fragmented heap.
void ia_free(void* block)
{
    if (block == NULL)
        return;

    heap_chunk_t* chunk = (heap_chunk_t*)((char*)block - sizeof(heap_chunk_t));
    chunk->in_use = false;
    chunk->next = g_heap_info.head; // Inserting the chunk into the free list
    // Don't modify chunk sizes yet, eventually this should be done correctly to handle coalescing of chunks
    // chunk->prev_size = chunk->size;
    // chunk->size = 0;
    g_heap_info.head = chunk;
    g_heap_info.avail_size += chunk->size + sizeof(heap_chunk_t);

    IA_DEBUG_PRINT("Freed heap chunk %p of size: %zu bytes\n", chunk, chunk->size);
}

void* ia_memset(void* block, int val, size_t range)
{
    if (block == NULL)
        return NULL;

    if (range == 0)
        return block;

    for (uint8_t* ptr = (uint8_t*)block; range > 0; --range)
        *(ptr++) = (uint8_t)val;

    return block;
}
