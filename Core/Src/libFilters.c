// Copyright © 2022 Samsonenko Dmitry. All rights reserved.
// License: http://opensource.org/licenses/MIT

#include "libFilters.h"

void InitAverFilter(AverFilterStruct *Signal, uint8_t SampleSize, uint8_t SampleFreq)
{
    Signal->SampleSize = SampleSize;
    Signal->SampleFreq = SampleFreq;
    Signal->Value = 0;
    Signal->Out = 0;
    Signal->Sum = 0;
    Signal->SampleCount = 0;
    Signal->SampleFreqCount = 0;
    Signal->Filtered = 0;
}
void InitExclAverFilter(ExclAverFilterStruct *Signal, uint8_t SampleSize, uint8_t SampleFreq)
{
    Signal->SampleSize = SampleSize;
    Signal->SampleFreq = SampleFreq;
    Signal->Value = 0;
    Signal->Out = 0;
    Signal->Sum = 0;
    Signal->MaxValue = 0;
    Signal->MinValue = UINT16_MAX;
    Signal->SampleCount = 0;
    Signal->SampleFreqCount = 0;
    Signal->Filtered = 0;
}
void InitMidleOf3Filter(MidleOf3FilterStruct *Signal, uint8_t SampleFreq)
{
    Signal->SampleFreq = SampleFreq;
    Signal->Value = 0;
    Signal->Out = 0;
    Signal->MaxValueInd = 0;
    Signal->MinValueInd = 0;
    Signal->SampleCount = 0;
    Signal->SampleFreqCount = 0;
    Signal->Filtered = 0;
}
void InitRuningAverFilterr(RuningAverFilterStruct *Signal, float RunAverCoef, uint8_t SampleFreq)
{
    Signal->RunAverCoef = RunAverCoef;
    Signal->SampleFreq = SampleFreq;
    Signal->Value = 0;
    Signal->Out = 0;
    Signal->SampleFreqCount = 0;
    Signal->Filtered = 0;
}

/**
 * @brief Простое осреднение по SampleSize точкам
 * 
 * @param Signal - ссылка на структуру фильтра с осреднением 
 */
void AverFilterHandler(AverFilterStruct *Signal)
{
    Signal->SampleFreqCount++;
    if (Signal->SampleFreqCount >= Signal->SampleFreq)
    {
        Signal->SampleFreqCount = 0;
        Signal->SampleCount++;
        Signal->Sum += Signal->Value;
        if (Signal->SampleCount >= Signal->SampleSize)
        {
            Signal->Out = Signal->Sum / Signal->SampleCount;
            Signal->SampleCount = 0;
            Signal->Sum = 0;
            Signal->Filtered = 1;
        }
    }
}

/**
 * @brief Осреднение с исключением максимального и минимального значения. 
 * Внимание! Минимальное значение выборки в этом случае равно 3!
 * 
 * @param Signal - ссылка на структуру фильтра с осреднением и исключеним двух экстремумов выборки
 */
void ExclAverFilterHandler(ExclAverFilterStruct *Signal)
{
    Signal->SampleFreqCount++;
    if (Signal->SampleFreqCount >= Signal->SampleFreq)
    {
        Signal->SampleFreqCount = 0;
        Signal->SampleCount++;
        Signal->Sum += Signal->Value;
        /*Поиск экстремумов, которые будут выкидываться из расчета*/
        if (Signal->MaxValue < Signal->Value)
        {
            Signal->MaxValue = Signal->Value;
        }
        if (Signal->MinValue > Signal->Value)
        {
            Signal->MinValue = Signal->Value;
        }
        if (Signal->SampleCount >= Signal->SampleSize)
        {

            Signal->Out = (Signal->Sum - Signal->MaxValue - Signal->MinValue) / (Signal->SampleCount - 2);
            Signal->SampleCount = 0;
            Signal->Sum = 0;
            Signal->MinValue = UINT16_MAX;
            Signal->MaxValue = 0;
            Signal->Filtered = 1;
        }
    }
}

/**
 * @brief Медианный фильтр по трем точкам. Выдает среднее из трех записанных точек.
 * 
 * @param Signal - ссылка на структуру медианного фильтра
 */
void MidleOf3FilterHandler(MidleOf3FilterStruct *Signal)
{
    Signal->SampleFreqCount++;
    if (Signal->SampleFreqCount >= Signal->SampleFreq)
    {
        Signal->SampleFreqCount = 0;
        Signal->Sample[Signal->SampleCount] = Signal->Value;
        Signal->SampleCount++;

        if (Signal->SampleCount >= 3)
        {
            /*Поиск среднего из трех*/
            if (Signal->Sample[0] >= Signal->Sample[1])
            {
                Signal->MaxValueInd = 0;
                Signal->MinValueInd = 1;
            }
            else
            {
                Signal->MaxValueInd = 1;
                Signal->MinValueInd = 0;
            }
            if (Signal->Sample[2] > Signal->Sample[Signal->MaxValueInd])
            {
                Signal->Out = Signal->Sample[Signal->MaxValueInd];
            }
            else
            {
                if (Signal->Sample[2] < Signal->Sample[Signal->MinValueInd])
                {
                    Signal->Out = Signal->Sample[Signal->MinValueInd];
                }
                else
                {
                    Signal->Out = Signal->Sample[2];
                }
            }
            Signal->SampleCount = 0;
            Signal->Filtered = 1;
        }
    }
}

/**
 * @brief Бегущее среднее (вроде по двум точкам). Настраиваемый параметр RunningAverageCoeff -
 * резкость фильтрации. На результаты сильно влияет частота опроса.
 * 
 * @param Signal - ссылка на структуру фильтра бегущим средним
 */
void RuningAverFilterHandler(RuningAverFilterStruct *Signal)
{
    Signal->SampleFreqCount++;
    if (Signal->SampleFreqCount >= Signal->SampleFreq)
    {
        Signal->SampleFreqCount = 0;
        Signal->Out = (Signal->Value - Signal->Out) * Signal->RunAverCoef + Signal->Out;
    }
    Signal->Filtered = 1;
}

/*Интересные результаты дает фильтрация мединным фильтром, а затем бегущим средним*/