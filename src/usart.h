#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdbool.h>

void usart_init(USART_t *usart, uint32_t baud_rate);

bool usart_is_bdf(USART_t *usart);
bool usart_is_isfif(USART_t *usart);
bool usart_is_rxsif(USART_t *usart);
bool usart_is_dreif(USART_t *usart);
bool usart_is_txcif(USART_t *usart);
bool usart_is_rxcif(USART_t *usart);

void usart_write(USART_t *usart, uint8_t data);
uint8_t usart_read(USART_t *usart);

#endif // USART_H_
