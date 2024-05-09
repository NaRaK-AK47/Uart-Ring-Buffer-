/*
 * UartRingbuffer.c
 *
 *  Created on: Apr 24, 2024
 *      Author: KARAN
 */


#include "UartRingbuffer.h"
extern RingBuffer rx_data;

uint8_t Rx_DirectReceive[Rx_Buffer_Size];
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

#define UART huart2
#define DMA hdma_usart2_rx

void RingBufferInit(UART_HandleTypeDef *huart){

	HAL_UARTEx_ReceiveToIdle_DMA(huart, Rx_DirectReceive, Rx_Buffer_Size);
	 __HAL_DMA_DISABLE_IT(&DMA, DMA_IT_HT);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){

	if(huart == &UART ){
	WriteBuffer(&rx_data,Size);
	HAL_UARTEx_ReceiveToIdle_DMA(&UART, Rx_DirectReceive, Rx_Buffer_Size);
	 __HAL_DMA_DISABLE_IT(&DMA, DMA_IT_HT);
	}
}

void WriteBuffer(RingBuffer *SourceBuffer,uint16_t Size){

	if(SourceBuffer->head + Size > BUFFER_SIZE ){
		if(SourceBuffer->tail>=(SourceBuffer->head+Size)%BUFFER_SIZE){
		memcpy((uint8_t*)SourceBuffer->buffer + SourceBuffer->head,Rx_DirectReceive,BUFFER_SIZE - SourceBuffer->head);
		memcpy((uint8_t*)SourceBuffer->buffer,Rx_DirectReceive+ (BUFFER_SIZE - SourceBuffer->head),Size - (BUFFER_SIZE - SourceBuffer->head));
		SourceBuffer->head = Size -  (BUFFER_SIZE - SourceBuffer->head);
	}}
	else {
		memcpy((uint8_t*)SourceBuffer->buffer + SourceBuffer->head,Rx_DirectReceive,Size);
	    SourceBuffer->head = SourceBuffer->head + Size;

	}

}

void ReadBuffer(uint8_t * DesBuffer,RingBuffer *SourceBuffer,uint16_t Size){

	if(SourceBuffer->tail + Size > BUFFER_SIZE ){

		memcpy(DesBuffer,(uint8_t*)SourceBuffer->buffer + SourceBuffer->tail,BUFFER_SIZE - SourceBuffer->tail);
		memcpy(DesBuffer+(BUFFER_SIZE - SourceBuffer->tail),(uint8_t*)SourceBuffer->buffer,Size - (BUFFER_SIZE - SourceBuffer->tail));
		SourceBuffer->tail = Size -  (BUFFER_SIZE - SourceBuffer->tail);
	}
	else {
		memcpy(DesBuffer,(uint8_t*)SourceBuffer->buffer + SourceBuffer->tail,Size);
	    SourceBuffer->tail = SourceBuffer->tail + Size;

	}

}

uint16_t isdataAvaliable(RingBuffer *SourceBuffer){
	return (uint16_t)(BUFFER_SIZE+(SourceBuffer->head - SourceBuffer->tail))%BUFFER_SIZE;
}

void copyafter(uint8_t *DesBuffer, RingBuffer *SourceBuffer, char *str) {
    // Calculate the length of data in SourceBuffer
    uint16_t len = (uint16_t)(BUFFER_SIZE + (SourceBuffer->head - SourceBuffer->tail)) % BUFFER_SIZE;

    // Declare a temporary buffer to hold data from SourceBuffer
    uint8_t tempbuff[Rx_Buffer_Size];

    // Read data from SourceBuffer into tempbuff
    ReadBuffer(tempbuff, SourceBuffer, len);

    // Find the occurrence of str within tempbuff
    char *res = strstr((char *)tempbuff, str);

    if (res != NULL) {
        // Calculate the length of data after the occurrence of str
        uint16_t strlength = strlen(str);
        uint16_t datalen = res - (char *)tempbuff + strlength;

        // Copy the data after the occurrence of str into DesBuffer
        memcpy(DesBuffer, tempbuff + datalen, len - datalen);
    } else {
        // Handle the case when str is not found in tempbuff
        // For example, you can set DesBuffer to zero or do any other appropriate action
        // For now, let's set DesBuffer to zero
        memset(DesBuffer, 0, len);
    }
}

