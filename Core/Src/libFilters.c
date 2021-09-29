#include "libFilters.h"
uint8_t EndOfFilterFlag = 0;
uint8_t EndOfCombinedFilter = 0;
uint16_t SampleSize = 7;
float RunningAverageCoeff = 0.7;

/**
 * @brief Простое осреднение по SampleSize точкам
 * 
 * @param Value - новое значение в выборку
 * @return uint16_t - среднее значение (пока набирается новая выборка - высылает старое среднее)
 */
uint16_t SimpleAverageFilter(uint16_t Value)
{
    static uint8_t SamplePackCount = 0;
    static uint8_t SizeCount = 0;
    static uint32_t SimpleSum = 0;
    static uint16_t MeanValue = 0;

    SamplePackCount += 1;
    if (SamplePackCount >= SampleFreq)
    {
        SamplePackCount = 0;
        SizeCount += 1;
        SimpleSum = SimpleSum + Value;
        if (SizeCount >= SampleSize)
        {
            MeanValue = SimpleSum / SizeCount;
            EndOfFilterFlag = 1;
            SizeCount = 0;
            SimpleSum = 0;
        }
    }
    return MeanValue;
}

/**
 * @brief Осреднение с исключением максимального и минимального значения. 
 * Внимание! Минимальное значение выборки в этом случае равно 3!
 * 
 * @param Value - новое значение в выборку
 * @return uint16_t - среднее значение (пока набирается новая выборка - высылает старое среднее)
 */
uint16_t ExcludeAverageFilter(uint16_t Value)
{
    static uint8_t SamplePackCount = 0;
    static uint8_t SizeCount = 0;
    static uint32_t SimpleSum = 0;
    static uint16_t MeanValue = 0;
    static uint16_t MaxValue = 0;
    static uint16_t MinValue = 65535;

    SamplePackCount += 1;
    if (SamplePackCount >= SampleFreq)
    {
        SamplePackCount = 0;
        SizeCount += 1;
        SimpleSum = SimpleSum + Value;
        if (MaxValue < Value)
            MaxValue = Value;
        if (MinValue > Value)
            MinValue = Value;

        if (SizeCount >= SampleSize)
        {
            MeanValue = (SimpleSum - MaxValue - MinValue) / (SizeCount - 2);
            EndOfFilterFlag = 1;
            SizeCount = 0;
            SimpleSum = 0;
            MaxValue = 0;
            MinValue = 65535;
        }
    }
    return MeanValue;
}

/**
 * @brief Медианный фильтр по трем точкам. Выдает среднее из трех записанных точек.
 * 
 * @param Value - новое значение в выборку
 * @return uint16_t - отфильтрованное значение (пока набирается новая выборка - высылает предыдущее отфильтрованнон)
 */
uint16_t MidleOf3Filter(uint16_t Value)
{
    static uint8_t SamplePackCount = 0;
    static uint8_t SizeCount = 0;
    static uint16_t Midle = 0;
    static uint16_t Sample[3] = {0, 0, 0};
    static uint16_t MaxValueInd = 0;
    static uint16_t MinValueInd = 0;

    SamplePackCount += 1;
    if (SamplePackCount >= SampleFreq)
    {
        SamplePackCount = 0;
        SizeCount += 1;
        Sample[SizeCount - 1] = Value;
        if (Sample[MaxValueInd] < Value)
            MaxValueInd = SizeCount - 1;
        if (Sample[MinValueInd] > Value)
            MinValueInd = SizeCount - 1;

        if (SizeCount >= 3)
        {
            switch (MaxValueInd + MinValueInd)
            {
            case 1:
                Midle = Sample[2];
                break;
            case 2:
                Midle = Sample[1];
                break;
            case 3:
                Midle = Sample[0];
                break;
            default:
                Midle = Sample[0];
                break;
            }
            EndOfFilterFlag = 1;
            SizeCount = 0;
            MaxValueInd = 0;
            MinValueInd = 0;
        }
    }
    return Midle;
}
/**
 * @brief Бегущее среднее (вроде по двум точкам). Настраиваемый параметр RunningAverageCoeff -
 * резкость фильтрации. На результаты сильно влияет частота опроса.
 * 
 * @param Value - новое значение в выборку
 * @return uint16_t - отфильтрованное значение (пока набирается новая выборка - высылает предыдущее отфильтрованнон)
 */
uint16_t RunningAverage(uint16_t Value)
{
    static uint8_t SamplePackCount = 0;
    static uint16_t FilteredValue = 0;

    SamplePackCount += 1;
    if (SamplePackCount >= SampleFreq)
    {
        SamplePackCount = 0;
        FilteredValue = (Value - FilteredValue) * RunningAverageCoeff + FilteredValue;
    }
    return FilteredValue;
}

/**
 * @brief Медианная фильтрация (предфильтрация) + фильтрация бегущим средним.
 * Если чатота выборок больше 1, то стоит помнить, что в данной реализации чатота выборок будет
 * больше в SampleFreq^3 раз.
 * @param Value - новое значение в выборку
 * @return uint16_t - отфильтрованное значение (пока набирается новая выборка - высылает предыдущее отфильтрованнон)
 */
uint16_t CombinedFilterSample = 1;
uint16_t MidleOf3ANDRunningAverage(uint16_t Value)
{
    static uint8_t SamplePackCount = 0;
    static uint8_t SizeCount = 0;
    static uint16_t PreFilteredValue = 0;
    static uint16_t FilteredValue = 0;

    SamplePackCount += 1;
    if (SamplePackCount >= CombinedFilterSample)
    {
        SamplePackCount = 0;
        SizeCount += 1;
        PreFilteredValue = MidleOf3Filter(Value);
        if (SizeCount == 3)
        {
            SizeCount = 0;
            FilteredValue = RunningAverage(PreFilteredValue);
            EndOfCombinedFilter = 1;
        }
    }
    return FilteredValue;
}