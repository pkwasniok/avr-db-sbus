#define F_CLK_PER 24000000

#include <avr/io.h>
#include "./usart.h"



void usart_init(USART_t *usart, uint32_t baud_rate)
{
    // Set baud rate
    usart->BAUD = (64 * F_CLK_PER) / (16 * baud_rate);

    // Set frame format and mode of operation (8 data bit, 1 stop bit, asynchronous, no parity)
    usart->CTRLC = USART_CHSIZE_8BIT_gc;

    // Enable transmitter and receiver
    usart->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
}



bool usart_is_bdf(USART_t *usart)
{
    return (usart->STATUS & USART_BDF_bm);
}

bool usart_is_isfif(USART_t *usart)
{
    return (usart->STATUS & USART_ISFIF_bm);
}

bool usart_is_rxsif(USART_t *usart)
{
    return (usart->STATUS & USART_RXSIF_bm);
}

bool usart_is_dreif(USART_t *usart)
{
    return (usart->STATUS & USART_DREIF_bm);
}

bool usart_is_txcif(USART_t *usart)
{
    return (usart->STATUS & USART_TXCIF_bm);
}

bool usart_is_rxcif(USART_t *usart)
{
    return (usart->STATUS & USART_RXCIF_bm);
}



void usart_write(USART_t *usart, uint8_t data)
{
    // Transmitter data register can only be written when there is no data in it (datasheet p. 377)
    // Because of that you need to wait for DREIF flag to be 0
    while(!usart_is_dreif(usart));

    // This library configures USART to 8-bit word length
    // Because of that only low byte data register is used
    usart->TXDATAL = data;
}

uint8_t usart_read(USART_t *usart)
{
    // Read data from receiver data register (datasheet p. 377)
    // It is important to first read RXDATAH because read from RXDATAL
    // shifts data registers, thus making it impossible to read complete frame
    uint8_t data_h = usart->RXDATAH;
    uint8_t data_l = usart->RXDATAL;

    // Decode flags from high byte (datasheet p. 391)
    bool is_bufovf = (data_h & USART_BUFOVF_bm);
    bool is_ferr = (data_h & USART_FERR_bm);
    bool is_perr = (data_h & USART_PERR_bm);
    bool is_rxcif = (data_h & USART_RXCIF_bm);
  
    if (is_rxcif || is_bufovf || is_ferr || is_perr) 
    {
        // No data received or error occured
        return 0;
    }

    // This library configures USART to 8-bit word length
    // Because of that only low byte contains data
    return data_l;
}
