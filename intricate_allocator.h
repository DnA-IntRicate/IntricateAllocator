#ifndef INTRICATE_ALLOCATOR_H
    #define INTRICATE_ALLOCATOR_H

    #ifdef _MSC_VER
        #pragma once
    #endif // _MSC_VER

    #define IA_VERSION_MAJOR 1
    #define IA_VERSION_MINOR 0
    #define IA_VERSION_PATCH 0
    #define IA_VERSION_REVISION 1
    #define IA_VERSION_STRING "1.0.0.1"
    
    #if defined(IA_DYNAMIC) && defined(IA_STATIC)
        #error "Both IA_DYNAMIC and IA_STATIC cannot be defined at the same time!"
    #endif // defined(IA_DYNAMIC) && defined(IA_STATIC)
    
    #ifdef IA_DYNAMIC
        #ifdef _MSC_VER
            #define IA_IMPORT __declspec(dllimport)
            #define IA_EXPORT __declspec(dllexport)
            
            // Disable Visual Studio dll-interface warnings
            #pragma warning(disable: 4251)
            #pragma warning(disable: 4275)
        #else
            #define IA_IMPORT __attribute__ ((visibility("default")))
            #define IA_EXPORT __attribute__ ((visibility("default")))
        #endif // _MSC_VER
    
        #ifdef IA_BUILD
            #define IA_API IA_EXPORT
        #else
            #define IA_API IA_IMPORT
        #endif // IA_BUILD
    #else
        #ifndef IA_STATIC
            #define IA_STATIC
        #endif // !IA_STATIC
    #endif // IA_DYNAMIC
    
    #ifdef IA_STATIC
        #define IA_IMPORT
        #define IA_EXPORT
        #define IA_API
    #endif // IA_STATIC

    #include <stdint.h>

    #ifdef __cplusplus
        #define IA_BEGIN_EXTERN_CPP extern "C" {
        #define IA_END_EXTERN_CPP }
    #else
        #define IA_BEGIN_EXTERN_CPP
        #define IA_END_EXTERN_CPP
    #endif // __cplusplus

IA_BEGIN_EXTERN_CPP

    // Allocates memory of size and returns a pointer to it in memory.
    // TODO: Make proper comments here.
    IA_API void* ia_alloc(size_t size);

    IA_API void ia_free(void* block);

IA_END_EXTERN_CPP

    #ifdef __cplusplus
        #define IA_RELEASE(ptr) ia_free(ptr); ptr = nullptr
    #else
        #define IA_RELEASE(ptr) ia_free(ptr); ptr = ((void*)0)
    #endif // __cplusplus
#endif // !INTRICATE_ALLOCATOR_H
