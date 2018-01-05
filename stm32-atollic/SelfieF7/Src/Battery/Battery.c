/*
 * Battery.c
 *
 *  Created on: 05.01.2018
 *      Author: mice
 */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "Battery.h"
#include "Filtering.h"

#include "cmsis_os.h"
#include "math.h"
#include "adc.h"

extern osThreadId BatteryManagerHandle;

uint16_t adc_raw[3];
float Amps_raw;
float Amps_f;

float Volts_raw;
float Volts_f;

float dT = 0.001f;

filter_t amps_filter;

void StartBatteryManager(void const * argument){
	MX_ADC1_Init();
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_raw, 3);
	lpf_filter_init(&amps_filter, 100, 1000);
	while(1)
	{
		Amps_raw = adc_raw[1] / 4096.f * 3.3f *20.f;
		Amps_f = filter_apply(&amps_filter, Amps_raw);
		mAhs_drawn +=Amps_f*1000*dT;

		Volts_f = adc_raw[2] / 4096.f * 3.3f *5.7f;
		osDelay(1);
	}
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (hadc->Instance == hadc1.Instance) {

	}
}
