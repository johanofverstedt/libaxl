
#include "stdio.h"

// Primitive type definitions
typedef signed char        i8;
typedef unsigned char      u8;
typedef short              i16;
typedef unsigned short     u16;
typedef int                i32;
typedef unsigned int       u32;
typedef long long          i64;
typedef unsigned long long u64;

// array_double
typedef struct {
    double* elem;
    i32     count;
} array_double;


double sum(array_double v) {
    double s = 0.0;
    for(i32 i = 0; i < v.count; ++i) {
        s += v.elem[i];
    } //for (i)
    return s;
} // sum


double sum(double* a, i32 count) {
    double s = 0.0;
    for(i32 i = 0; i < count; ++i) {
        s += a[i];
    } //for (i)
    return s;
} // sum

// 
// Program entry point
// 
int main(int argc, char** argv) {
    double a[4] = { 1.0, 2.0, 3.0, 4.0 };
    array_double aa;
    aa.count = sizeof(a) / sizeof(*a);
    aa.elem = a;
    double result = sum(aa);
    printf("%f", result);
    return 0;
} // main

