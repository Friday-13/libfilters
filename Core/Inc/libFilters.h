// Copyright © 2022 Samsonenko Dmitry. All rights reserved.
// License: http://opensource.org/licenses/MIT

#include "stm32f1xx_hal.h"
#ifndef LIBFILTERS_H_
#define LIBFILTERS_H_

typedef struct
{
    uint16_t Value;          //Значение, подаваемое на вход фильтра
    uint16_t Out;            //Значение с выхода фильтра
    uint8_t SampleSize;      //Размер выборки
    uint32_t Sum;            //Сумма выборки
    uint8_t SampleCount;     //Счетчик значения в выборке
    uint8_t SampleFreq;      //Частота обновления входной выборки (каждое FilterFreq-ое значение)
    uint8_t SampleFreqCount; //Счетчик для изменения частоты отбора выборки
    uint8_t Filtered;        //Флаг окончания фильтрации
} AverFilterStruct;

typedef struct
{
    uint16_t Value;          //Значение, подаваемое на вход фильтра
    uint16_t Out;            //Значение с выхода фильтра
    uint8_t SampleSize;      //Размер выборки
    uint32_t Sum;            //Сумма выборки
    uint16_t MaxValue;       //Макс значение в выборке
    uint16_t MinValue;       //Мин значение в выборке
    uint8_t SampleCount;     //Счетчик значения в выборке
    uint8_t SampleFreq;      //Частота обновления входной выборки (каждое FilterFreq-ое значение)
    uint8_t SampleFreqCount; //Счетчик для изменения частоты отбора выборки
    uint8_t Filtered;        //Флаг окончания фильтрации
} ExclAverFilterStruct;

typedef struct
{
    uint16_t Value;          //Значение, подаваемое на вход фильтра
    uint16_t Out;            //Значение с выхода фильтра
    uint16_t Sample[3];      //Выборка
    uint8_t MaxValueInd;     //Макс значение в выборке
    uint8_t MinValueInd;     //Мин значение в выборке
    uint8_t SampleCount;     //Счетчик значения в выборке
    uint8_t SampleFreq;      //Частота обновления входной выборки (каждое FilterFreq-ое значение)
    uint8_t SampleFreqCount; //Счетчик для изменения частоты отбора выборки
    uint8_t Filtered;        //Флаг окончания фильтрации
} MidleOf3FilterStruct;

typedef struct
{
    uint16_t Value;          //Значение, подаваемое на вход фильтра
    uint16_t Out;            //Значение с выхода фильтра
    float RunAverCoef;       //Коэффициент фильтрации
    uint8_t SampleFreq;      //Частота обновления входной выборки (каждое FilterFreq-ое значение)
    uint8_t SampleFreqCount; //Счетчик для изменения частоты отбора выборки
    uint8_t Filtered;        //Флаг окончания фильтрации
} RuningAverFilterStruct;

/**
 * @brief FilterHandler(X) - дженерик макрос для перегрузки функций
 * В зависимости от типа аргумента, вызывается определенная функция
 * обработки фильтра
 */
#define FilterHandler(X)              \
    _Generic((X),                     \
             AverFilterStruct*         \
             : AverFilterHandler,     \
               ExclAverFilterStruct*   \
             : ExclAverFilterHandler, \
               MidleOf3FilterStruct*   \
             : MidleOf3FilterHandler, \
               RuningAverFilterStruct*   \
             : RuningAverFilterHandler, \
               default                \
             : AverFilterHandler)(X)

void InitAverFilter(AverFilterStruct *Signal, uint8_t SampleSize, uint8_t SampleFreq);
void InitExclAverFilter(ExclAverFilterStruct *Signal, uint8_t SampleSize, uint8_t SampleFreq);
void InitMidleOf3Filter(MidleOf3FilterStruct *Signal, uint8_t SampleFreq);
void InitRuningAverFilterr(RuningAverFilterStruct *Signal, float RunAverCoef, uint8_t SampleFreq);

void AverFilterHandler(AverFilterStruct *Signal);
void ExclAverFilterHandler(ExclAverFilterStruct *Signal);
void MidleOf3FilterHandler(MidleOf3FilterStruct *Signal);
void RuningAverFilterHandler(RuningAverFilterStruct *Signal);


#endif /*LIBFILTERS_H_*/