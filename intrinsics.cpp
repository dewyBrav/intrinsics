// Intrinsics

/*Векторным регистрам xmm (SSE) соответствуют три типа данных [1]:
__m128,  «массив» из четырёх чисел с плавающей запятой с одинарной точностью;
__m128d, «массив» из двух чисел с плавающей запятой двойной точности;
__m128i, 128-битный регистр, который можно рассматривать как «массив» 8-, 16-,32- и 64-битных чисел*/

// 1.2.1: Example of SSE2 intrinsics
// for int32_t
#include <stdint.h>
// for SSE2 intrinsics
#include <emmintrin.h>
#include <iostream>

//
void bar()
{
    int32_t array_a[4] = {0, 2, 1, 2}; // 128 bit
    int32_t array_b[4] = {8, 5, 0, 6};
    int32_t array_c[4];

    __m128i a, b, c;
    a = _mm_loadu_si128((__m128i *)array_a); // loading array_a into register a
    b = _mm_loadu_si128((__m128i *)array_b);

    c = _mm_add_epi32(a, b); // must be { 8,7,1,8 }
    _mm_storeu_si128((__m128i *)array_c, c);

    for (int i = 0; i < 4; ++i)
        std::cout << array_c[i] << ' ';
}

int main()
{
    bar();

    return 0;
}