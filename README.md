## Intrinsics

### Contents
* `Intrinsics`: встроенные объекты компилятора
## https://www.elecard.com/ru/page/article_vector_instructions_part1?utm_source=reddit&utm_campaign=vector#:~:text=__m128i%2C%20128-%D0%B1%D0%B8%D1%82%D0%BD%D1%8B%D0%B9%20%D1%80%D0%B5%D0%B3%D0%B8%D1%81%D1%82%D1%80,-%20%D0%B8%2064-%D0%B1%D0%B8%D1%82%D0%BD%D1%8B%D1%85%20%D1%87%D0%B8%D1%81%D0%B5%D0%BB.
## Векторные вычисления — такие вычисления, когда при выполнении одной инструкции процессора производится  одновременно несколько однотипных операций

## Векторные инструкции реализуют те же операции, что и скалярные (обычные) инструкции, но благодаря большому объёму обрабатываемых данных производительность этих инструкций выше. Если для регистра общего назначения при выполнении некоторой инструкции предполагается, что в нём находится только одна порция данных определённого типа (целое число определённой разрядности, число с плавающей запятой), то в векторном регистре одновременно находится столько независимых порций данных определённого типа, сколько позволяет разместить ёмкость регистра. И такое же количество одновременных независимых операций может быть произведено над этими данными при выполнении векторной инструкции — и во столько же раз повышается производительность вычислений. Повысить производительность процессора, выполняя несколько одинаковых операций одновременно, — основная задача векторных расширений.

## Можно получить доступ к векторным инструкциям и в программе на языке высокого уровня (в частности, C/C++) без ассемблерных вставок. Для этого используются так называемые интринсики (intrinsics) — встроенные объекты компилятора. В заголовочном файле объявлен один или несколько типов данных (с точки зрения программиста, это массив фиксированной длины, но без возможности доступа к элементам этого массива), переменной одного из этих типов соответствует векторный регистр

## векторным регистрам xmm (SSE) соответствуют три типа данных [1]:  
## __m128, «массив» из четырёх чисел с плавающей запятой с одинарной точностью; 
## __m128d, «массив» из двух чисел с плавающей запятой двойной точности;
## __m128i, 128-битный регистр, который можно рассматривать как «массив» 8-, 16-,32- и 64-битных чисел. 
## Поскольку конкретная векторная инструкция работает, как правило, с одним из трёх типов данных (число с плавающей запятой одинарной точности, двойной точности, целочисленный), то и аргументы функций-векторных инструкций имеют один из трёх указанных типов. Подобным образом устроена и система типов AVX2: в ней имеются типы __m256 (с плавающей запятой), __m256d (двойной точности) и __m256i (целочисленный).

## Описание intrinsics.cpp
## В этом примере содержимое массива array_a загружается в один векторный регистр, а массива array_b — в другой. Затем соответствующие 32-битные элементы регистров складываются, и результат записывается в третий регистр, а затем копируется в массив array_c. В этом примере можно отметить ещё одну особенность интринсиков: _mm_add_epi32 принимает два аргумента-регистра и возвращает одно значение-регистр. Но в действительности инструкция paddd, которой соответствует _mm_add_epi32, имеет только два регистра-операнда, в один из которых записывается результат операции с потерей первоначального содержимого регистра. Чтобы сохранить содержимое регистров при компиляции c = _mm_add_epi32(a, b), компилятор добавляет дополнительные операции копирования из регистра в регистр
## Имена интринсиков выбраны так, чтобы облегчить чтение исходного текста программ. В архитектуре x86 имя состоит из трёх частей: префикса, обозначений операции и типа скалярных данных(пример _mm_add_epi32). Префикс обозначает разрядность векторного регистра: _mm_ для 128 бит, _mm256_ для 256 бит и _mm512_ для 512 бит соответственно. Обозначения некоторых типов данных приведены в таблице 1. 

## Таблица 1: Обозначение типов данных интринсиков архитектуры х86 (b)
## Обозначение 	Описание
## ps	        Число с плавающей запятой одинарной точности
## pd	        Число с плавающей запятой двойной точности
## epi8	        Целое число, 8 бит, со знаком
## epu8	        Целое число, 8 бит, без знака
## epi16	    Целое число, 16 бит, со знаком
## epi32	    Целое число, 32 бит, со знаком
## epi64	    Целое число, 64 бит, со знаком
## si128	    Целое число, 128 бит
## si256	    Целое число, 256 бит
## Имена типов данных (__m128i и другие) и имена функций-интринсиков фактически стали стандартными в разных компиляторах

## Чтобы процессор произвел какие-либо действия с данными, которые находятся в оперативной памяти, необходимо сначала загрузить эти данные в регистр процессора. А после завершения обработки данные необходимо записать обратно в оперативную память.

## Большинство векторных инструкций работает в формате «регистр — регистр», то есть операндами инструкций являются векторные регистры, и в них же записывается результат работы инструкций

## Логические операции и операции сравнения. В наборе SSE2 представлены инструкции, выполняющие следующие логические операции: «И», «ИЛИ», «исключающее ИЛИ», «И-НЕ». Имена инструкций, соответственно, следующие: _mm_and_si128, _mm_or_si128, _mm_xor_si128 и _mm_andnot_si128. Инструкции полностью аналогичны побитовым операциям, выполняемым над целыми числами, только разрядность числа равна не 32 или 64, а 128 битам.

## Арифметические операции и сдвиги. Инструкции, принадлежащие этой группе, несомненно, самые востребованные. Для вычислений с плавающей точкой и в x86, и в ARM есть инструкции, которые реализуют все четыре арифметические операции и вычисляют квадратный корень для чисел однократной и двойной точности. В х86 для чисел однократной точности эти инструкции следующие: _mm_add_ps, _mm_sub_ps, _mm_mul_ps, _mm_div_ps и _mm_srqt_ps.

## Перестановка и перемешивание. Для группы инструкций, о которых далее пойдёт речь, не существует аналогичных скалярных инструкций процессора. При их выполнении не образуются новые значения: либо производится перестановка данных внутри регистра, либо данные, взятые из нескольких регистров-источников, записываются в определённом порядке в регистр-приёмник. На первый взгляд, эти инструкции мало полезны, но в действительности их значение очень велико. Без них многие алгоритмы невозможно реализовать эффективно

## Инструкции AVX и AXV2. Дальнейшее развитие векторных инструкций архитектуры x86 связано с появлением 256-битных инструкций AVX и AVX2. Что эти инструкции предоставляют разработчикам? Прежде всего, вместо 8 (16) 128-битных регистров XMM имеется 16 256-битных регистров YMM0 – YMM15, младшие 128 бит которых являются векторными регистрами XMM. Инструкции, в отличие от SSE, принимают не два, а три регистра-операнда: два регистра-источника и один регистр-приёмник. При этом после выполнения инструкции первоначальное содержимое регистров-источников не теряется.Почти все операции, реализованные в более ранних наборах инструкций SSE-SSE4.2, есть и в AVX/AVX2, прежде всего — арифметические. Для инструкций вроде _mm_add_epi32, _mm_madd_epi16, _mm_sub_ps, _mm_slli_epi16 и многих других имеются полностью аналогичные инструкции, но которые вдвое их производительнее.