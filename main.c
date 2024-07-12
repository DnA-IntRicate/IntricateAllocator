#include "intricate_allocator.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct my_data_t
{
    int num1;
    float num2;
} my_data_t;

typedef struct vec2_t
{
    float x;
    float y;
} vec2_t;

int main(int argc, char** argv)
{
    my_data_t* data1 = (my_data_t*)ia_alloc(sizeof(my_data_t));
    data1->num1 = 54;
    data1->num2 = (22.0f / 7.0f);

    printf("(1) - Num 1: %d\n", data1->num1);
    printf("(1) - Num 2: %f\n", data1->num2);

    ia_free(data1);
    printf("Num1: %d\n", data1->num1);

    vec2_t* vec = (my_data_t*)ia_alloc(sizeof(vec2_t));
    vec->x = 2.0f;
    vec->y = 1.5f;

    printf("(2) - Vec.x: %f\n", vec->x);
    printf("(2) - Vec.y: %f\n", vec->y);

    my_data_t* data2 = (my_data_t*)ia_alloc(sizeof(my_data_t)); // This is infinite looping here, must be fixed!
    data2->num1 = 21;
    data2->num2 = 456.00453213f;

    printf("(2) - Num 1: %d\n", data2->num1);
    printf("(2) - Num 2: %f\n", data2->num2);
    ia_memset(data2, 0, sizeof(my_data_t));
    printf("(2)[After ia_memset] - Num 1: %d\n", data1->num1);
    printf("(2)[After ia_memset] - Num 2: %f\n", data1->num2);

    // This causes a use-after-free issue since vec is assigned the exact same allocation that data1 was in.
    // When trying to read data1->num1 we just get garbage back from it.
    printf("(1)[After Free] - Num1: %d\n", data1->num1);

    // Using the IA_RELEASE macro it automatically sets the pointer to null to prevent this
    IA_RELEASE(vec);
    printf("Vec after IA_RELEASE: %p\n", vec);

    (void)getchar();
    return 0;
}
