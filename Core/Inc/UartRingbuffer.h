/*
 * UartRingbuffer.h
 *
 *  Created on: Apr 24, 2024
 *      Author: KARAN
 */

#ifndef INC_UARTRINGBUFFER_H_
#define INC_UARTRINGBUFFER_H_


#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "string.h"


#define Rx_Buffer_Size 20
#define BUFFER_SIZE 64


typedef struct Rbuffer{
	char buffer[BUFFER_SIZE];
	unsigned int head;
	unsigned int tail;
}RingBuffer;


//RingBuffer tx_data = {.head = 0, .tail = 0};


void RingBufferInit(UART_HandleTypeDef *huart);

void WriteBuffer(RingBuffer *rx_data,uint16_t Size);

uint16_t isdataAvaliable(RingBuffer *rx_data);

void ReadBuffer(uint8_t * DesBuffer,RingBuffer *SourceBuffer,uint16_t Size);

void copyafter(uint8_t * DesBuffer,RingBuffer *SourceBuffer,char *str);
#endif /* INC_UARTRINGBUFFER_H_ */
