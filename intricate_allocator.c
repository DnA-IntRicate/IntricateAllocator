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
    #endif // !WIN32_LEAN_AND_MEAN
#endif // IA_PLATFORM_WINDOWS

#define NULLPTR ((void*)0)

typedef struct heap_chunk_t
{
    size_t size;
    size_t prev_size;
    bool in_use;
    struct heap_chunk_t* next;
} heap_chunk_t;

struct heap_info_t
{
    struct heap_chunk_t* start;
    size_t avail_size;
} g_heap_info;


// TODO: Return type for this function?
static void ia_heap_init(size_t size)
{
    printf("Initializing...\n");
    printf("Heap size: %zu bytes\n", size);

    HANDLE heap_mapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, NULL);
    if (heap_mapping == NULL)
    {
        fprintf(stderr, "Could not create file mapping object: %lu\n", GetLastError());
        return;
    }
    
    void* heap_start = MapViewOfFile(heap_mapping, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if (heap_start == NULL)
    {
        fprintf(stderr, "Could not map view of file: %lu\n", GetLastError());
        CloseHandle(heap_mapping);
        return;
    }

    heap_chunk_t* first = (heap_chunk_t*)heap_start;
    first->size = size - sizeof(heap_chunk_t*);
    first->in_use = false;
    first->next = first;

    memset(&g_heap_info, 0, sizeof(struct heap_info_t));
    g_heap_info.start = first;
    g_heap_info.avail_size = first->size;
}

void* ia_alloc(size_t size)
{
    // If the avail size is 0, initialize the heap, (this should be changed to request for more memory)
    if (g_heap_info.avail_size == 0)
        ia_heap_init(4096);

    // Returns null if the requested allocation size exceeds the heaps available size.
    if (size > g_heap_info.avail_size)
    {
        printf("No available heap space!");
        return NULLPTR;
    }

    // Traverse the free-list for the first valid large-enough chunk from the head of the heap
    heap_chunk_t* chunk = g_heap_info.start;
    while ((chunk != NULLPTR) && (size >= chunk->size) && chunk->in_use)
        chunk = chunk->next;

    if (chunk == NULLPTR)
    {
        printf("No suitable chunk found!");
        return NULLPTR;
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

    // The current chunk should be marked as in use so that it doesn't get unintentionally overwritten
    // in future calls of this function or before the chunk is freed.
    chunk->in_use = true;
    printf("Found heap chunk %p of size: %zu bytes\n", chunk, chunk->size);

    // Set the head of the heap equal to the next available chunk found previously
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
    if (block == NULLPTR)
        return;

    heap_chunk_t* chunk = &((heap_chunk_t*)block)[-1];
    heap_chunk_t* old_first = g_heap_info.start;

    g_heap_info.start = chunk;
    chunk->next = old_first;
    chunk->in_use = false;
    chunk->prev_size = chunk->size;
    chunk->size = 0;
}
