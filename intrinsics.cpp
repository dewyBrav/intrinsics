// for int32_t
#include <stdint.h>
// for SSE2 intrinsics
#include <emmintrin.h>
#include <iostream>

/*Описание void bar()
## В этом примере содержимое массива array_a загружается в один векторный регистр, а массива array_b — в другой.
Затем соответствующие 32-битные элементы регистров складываются, и результат записывается в третий регистр,
а затем копируется в массив array_c. В этом примере можно отметить ещё одну особенность интринсиков:
_mm_add_epi32 принимает два аргумента-регистра и возвращает одно значение-регистр. Но в действительности инструкция
paddd, которой соответствует _mm_add_epi32, имеет только два регистра-операнда, в один из которых записывается
результат операции с потерей первоначального содержимого регистра. Чтобы сохранить содержимое регистров при
компиляции c = _mm_add_epi32(a, b), компилятор добавляет дополнительные операции копирования из регистра в регистр*/
/*_____Более детальное описание_____
Инструкция _mm_loadu_si128(__m128i* addr) извлекает из оперативной памяти непрерывный массив целочисленных данных
длиной 128 бит с начальным адресом addr и записывает в выбранный векторный регистр.
В противоположность ей, инструкция _mm_storeu_si128(__m128i* addr, __m128i a) копирует в оперативную память,
начиная с адреса addr, непрерывный массив данных длиной 128 бит, которые содержатся в регистре a. При использовании
этих инструкций адрес addr может быть произвольным (естественно, при чтении и записи не должен происходить выход
за пределы массива).
Инструкции _mm_load_si128 и _mm_store_si128 аналогичны приведённым выше и потенциально более производительны.
Но они требуют, чтобы addr был кратен 16 байтам (иначе говоря, выровнен по границе 16 байт), иначе при их
выполнении произойдёт аппаратное исключение.*/
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

void sum_float(float src0[], float src1[], float dst[], size_t len);
void sum_double(double src0[], double src1[], double dst[], size_t len);

int main()
{
    {
        std::cout << "bar()\t";
        bar();
        std::cout << "\n";
    }
    {
        std::cout << "sum_float()\t";
        const size_t len{4};
        float src0[len]{1.15, 2.16, 1.00, 2.00};
        float src1[len]{7.88, 2.12, 0.00, 0.00};
        float dst[len]{}; //
        sum_float(src0, src1, dst, len);
        std::cout << "\n";
    }
    {
        std::cout << "sum_double()\t";
        const size_t len{2};
        double src0[len]{3.35, 2.11};
        double src1[len]{11.12, 2.12};
        double dst[len]{}; // 14.47, 4.23
        sum_double(src0, src1, dst, len);
        std::cout << "\n";
    }

    return 0;
}
/*Приведём простой пример использования арифметических инструкций с плавающей запятой. Здесь, аналогично примеру
в разделе 2 суммируются элементы двух массивов src0 и src1, а результат записывается в массив dst. При этом
количество элементов, которые необходимо просуммировать, задано параметром len. В том случае, если len не кратно
количеству элементов, которые вмещает векторный регистр (в нашем случае это будет четыре и два),
часть элементов обрабатывается обычным способом, без векторизации.*/
void sum_float(float src0[], float src1[], float dst[], size_t len)
{
    //__m128, «массив» из четырёх чисел с плавающей запятой с одинарной точностью;
    __m128 x0, x1; // floating-point, single precision

    size_t len4 = len & ~0x03;

    for (size_t i = 0; i < len4; i += 4)
    {
        x0 = _mm_loadu_ps(src0 + i); // loading of four float values
        x1 = _mm_loadu_ps(src1 + i);
        x0 = _mm_add_ps(x0, x1);
        _mm_storeu_ps(dst + i, x0); // store
    }
    for (size_t i = len4; i < len; i++)
    {
        dst[i] = src0[i] + src1[i];
    }

    // print
    for (size_t i {}; i < len4; ++i)
    {
        std::cout << dst[i] << ' ';
    }
}
void sum_double(double src0[], double src1[], double dst[], size_t len)
{
    //__m128d, «массив» из двух чисел с плавающей запятой двойной точности
    __m128d x0, x1; // floating-point, double precision

    size_t len2 = len & ~0x01;

    for (size_t i = 0; i < len2; i += 2)
    {
        x0 = _mm_loadu_pd(src0 + i); // loading of two double values
        x1 = _mm_loadu_pd(src1 + i);
        x0 = _mm_add_pd(x0, x1);
        _mm_storeu_pd(dst + i, x0);
    }
    if (len2 != len)
    {
        dst[len2] = src0[len2] + src1[len2];
    }

    for (size_t i{}; i < len2; ++i)
        std::cout << dst[i] << ' ';
}