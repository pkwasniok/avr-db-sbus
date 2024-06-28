#include <avr/io.h>
#include "./sbus.h"

#define F_CLK_PER 24000000

/* SBUS specification:
 * Baud rate: 100000
 * Data bits: 8
 * Parity bit: even
 * Stop bits: 2
*/
 
#define LIBCONFIG_SBUS_USART_BAUD_RATE 100000

void sbus_init(USART_t *usart)
{
    // Set baud rate
    usart->BAUD = (64 * F_CLK_PER) / (16 * LIBCONFIG_SBUS_USART_BAUD_RATE);

    // Configure USART according to SBUS specs
    usart->CTRLC = USART_PMODE_EVEN_gc | USART_SBMODE_2BIT_gc | USART_CHSIZE_8BIT_gc;

    // Enable receiver interrupt
    usart->CTRLA = USART_RXCIE_bm;

    // Enable receiver
    usart->CTRLB = USART_RXEN_bm;
}
