#include "intricate_allocator.h"
#include <stdio.h>
#include <stdlib.h>


struct my_data_t
{
    int num1;
    float num2;
};


int main(int argc, char** argv)
{
    struct my_data_t* data = (struct my_data_t*)ia_alloc(sizeof(struct my_data_t));
    data->num1 = 54;
    data->num2 = (22.0f / 7.0f);

    printf("Num 1: %d\n", data->num1);
    printf("Num 2: %f\n", data->num2);

    size_t arrSize = 32;
    int* arr = (int*)ia_alloc(sizeof(int) * arrSize);

    for (size_t i = 0; i < arrSize; ++i)
        arr[i] = i;

    for (size_t i = 0; i < arrSize; ++i)
        printf("%d\n", arr[i]);

    ia_free(data);
    printf("Freed: %p\n", data);
    printf("Num 1: %d\n", data->num1);

    // This code only throws a read access violation when i hits 1017 (1018 + 1). This is the value of i for the pointer
    // to exceed the bounds of the heap. This means the block returned from alloc isn't being properly truncated
//    size_t i = 0;
//    while (1)
//    {
//        printf("i is now : (%zu) at %d\n", i, *arr);
//
//        arr++;
//        i++;
//    }

    // Experiment:
    int* number = malloc(sizeof(int));
    *number = 91;
    printf("Before free: %d\n", *number);
    free(number);
    printf("After free: %d\n", *number);


    (void)getchar();

    return 0;
}
