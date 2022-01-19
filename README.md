# Project Description [Eng]

Project contains digital filters library and example of its application. To use the library, the ADC (or another sample source) must first be configured.

## Brief description of the library
The library includes the following filters:

* Averaging;
* Averaging with the exception of the maximum and minimum values ​​in the sample;
* Median filter by three points;
* Running average.
 
 Each of the filters must first be initialized with the corresponding function. The names of the required initialization functions contain the *Init-* prefix.
 
 Each structure contains several general parameters:

 * Value - value supplied to the filter input;
 * Out - filtering result;
 * SampleFreq - sample rate. Each SampleFreq-th passed value gets into the sample;
 * Filtered - flag about the end of filtering.

## Brief application (on the example of a running average)

1. Initialization of the structure corresponding to the desired filter \
   Example:
   ```
   #include "libFilters.h"
   RuningAverFilterStruct testRunAverFilter;
   ```
2. Initial setup of the structure by the corresponding function with the *Init-* prefix
   Example:
   ```
   InitRuningAverFilterr(&testRunAverFilter, 0.7, 1);
   ```
3. Getting a new sample value \
   Example:
   ```
   testRunAverFilter.Value = adc[0];
   ```
   *adc[0]* - ADC value (should be received beforehand)

4. Passing a new value to the handler and processing the filter \
   Example:
   ```
   FilterHandler(&testRunAverFilter);
   ```
Items 3 and 4 are performed as needed (for example, periodically). \
Items 1 and 2 - once, during configuration.

The *FilterHandler* function is universal and is called for any of the filters. The required handler is selected automatically, depending on the passed parameter.



# Описание проекта [Rus]

Проект содержит библиотеку цифровых фильтров и пример её применения. Для использования библиотеки требуется предварительно нстроить АЦП (или другой источник выборки).

## Краткое описание библиотеки
В состав библиотеки входят следующие фильтры:

*   Осреднение;
*   Осреднение с исключением максимального и минимального значения в выборке;
*   Медианный фильтр по трем точкам ;
*   Бегущее среднее.
 
 Каждый из фильтров требуется предварительно инициализировать соответствующей функцией. Имена нужных функций инициализаций содержат префикс *Init-*. 
 
 Каждая структура сожержит несколько общих параметров:

 *  Value - значение, подаваемое на вход фильтра;
 *  Out - результат фильтрации;
 *  SampleFreq - частота выборки. В выборку попадает каждое SampleFreq-ое переданное значение;
 *  Filtered - флаг об окончании фильтрации.

## Краткое применение (на примере бегущего среднего)

1. Инициализация структуры, соответствующей нужному фильтру \
   Пример:
   ```
   #include "libFilters.h"
   RuningAverFilterStruct testRunAverFilter;
   ```
2. Первоначальная настройка структуры соответствующей функцией с префиксом *Init-*
   Пример:
   ```
   InitRuningAverFilterr(&testRunAverFilter, 0.7, 1);
   ```
3. Получение нового значения выборки \
   Пример:
   ```
   testRunAverFilter.Value = adc[0];
   ```
   *adc[0]* - значение с АЦП (должно быть предварительно получено)

4. Передача нового значения в обработчик и обработка фильтра \
   Пример:
   ```
   FilterHandler(&testRunAverFilter);
   ```
Пункты 3 и 4 выполняются по мере необходимости (например, периодически). \
Пункты 1 и 2 - единожды, при конфигурации. 

Функция *FilterHandler* универсальная и вызывается для любого из фильтров. Нужный обработчик выбирается автоматически, в зависимости от переданного параметра.