#include "stm32f1xx_hal.h"
#define SampleFreq 1 ///< Расчет фильтра ведется по каждой SampleFreq выборке
//#define SampleSize 7
//#define RunningAverageCoeff 0.05 //При 0.03 шумов на Шпинделе не т вообще

uint16_t SimpleAverageFilter(uint16_t Value);
uint16_t ExcludeAverageFilter(uint16_t Value);
uint16_t MidleOf3Filter(uint16_t Value);
uint16_t RunningAverage(uint16_t Value);
uint16_t MidleOf3ANDRunningAverage(uint16_t Value);