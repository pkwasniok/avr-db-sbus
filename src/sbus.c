#include <avr/io.h>
#include "./sbus.h"



#define USART LIBCONFIG_SBUS_USART_PERIPH



// ===========================================
// ========== GLOBAL VARIABLES ===============
// ===========================================



int8_t SBUS_COUNTER = -1;
uint8_t SBUS_BUFFER[2];
uint8_t SBUS_BUFFER_FRAME[25];
uint8_t SBUS_SHADOW_BUFFER_FRAME[25];

uint8_t SBUS_FRAME_BUFFER[25];



// ===================================
// ========== HANDLERS ===============
// ===================================



void sbus_interrupt_handler()
{
    // Read data from USART
    uint8_t data_h = USART.RXDATAH;
    uint8_t data_l = USART.RXDATAL;

    // Save data into buffer
    SBUS_BUFFER[0] = SBUS_BUFFER[1];
    SBUS_BUFFER[1] = data_l;

    // Reset SBUS counter when detected frame header
    if (SBUS_COUNTER == -1 && SBUS_BUFFER[0] == LIBCONFIG_SBUS_FRAME_FOOTER && SBUS_BUFFER[1] == LIBCONFIG_SBUS_FRAME_HEADER)
    {
        SBUS_COUNTER = 0;
    }

    // If SBUS counter is initialized, save incoming bytes into shadow frame buffer
    if (SBUS_COUNTER >= 0 && SBUS_COUNTER <= 24)
    {
        SBUS_SHADOW_BUFFER_FRAME[SBUS_COUNTER] = SBUS_BUFFER[1];
        SBUS_COUNTER += 1;
    }

    // When whole frame is received copy data from shadow frame buffer into frame buffer and deinitialize SBUS counter
    if (SBUS_COUNTER == 25)
    {
        SBUS_COUNTER = -1;

        for (int i=0; i<25; i++)
            SBUS_BUFFER_FRAME[i] = SBUS_SHADOW_BUFFER_FRAME[i];
    }
}



// =========================================
// ========== INITIALIZATION ===============
// =========================================



void sbus_init()
{
    // Set baud rate
    USART.BAUD = (64 * LIBCONFIG_SBUS_F_CLK_PER) / (16 * LIBCONFIG_SBUS_USART_BAUD_RATE);

    // Configure USART according to SBUS specs
    USART.CTRLC = USART_PMODE_EVEN_gc | USART_SBMODE_2BIT_gc | USART_CHSIZE_8BIT_gc;

    // Enable receiver interrupt
    USART.CTRLA = USART_RXCIE_bm;

    // Enable receiver
    USART.CTRLB = USART_RXEN_bm;
}



// ===================================
// ========== READINGS ===============
// ===================================



uint16_t sbus_channel_read(uint8_t channel)
{
    switch (channel)
    {
        case SBUS_CHANNEL_01:
            return (SBUS_BUFFER_FRAME[2]<<8 | SBUS_BUFFER_FRAME[1]) & 0x7FF;
        case SBUS_CHANNEL_02:
            return (SBUS_BUFFER_FRAME[3]<<5 | SBUS_BUFFER_FRAME[2]>>3) & 0x7FF;
        case SBUS_CHANNEL_03:
            return (SBUS_BUFFER_FRAME[5]<<10 | SBUS_BUFFER_FRAME[4]<<2 | SBUS_BUFFER_FRAME[3]>>6) & 0x7FF;
        case SBUS_CHANNEL_04:
            return (SBUS_BUFFER_FRAME[5]>>1 | SBUS_BUFFER_FRAME[6]<<7) & 0x7FF;
        case SBUS_CHANNEL_05:
            return (SBUS_BUFFER_FRAME[6]>>4 | SBUS_BUFFER_FRAME[7]<<4) & 0x7FF;
        case SBUS_CHANNEL_06:
            return (SBUS_BUFFER_FRAME[7]>>7 | SBUS_BUFFER_FRAME[8]<<1 | SBUS_BUFFER_FRAME[9]<<9) & 0x7FF;
        case SBUS_CHANNEL_07:
            return (SBUS_BUFFER_FRAME[9]>>2 | SBUS_BUFFER_FRAME[10]<<6) & 0x7FF;
        case SBUS_CHANNEL_08:
            return (SBUS_BUFFER_FRAME[10]>>5 | SBUS_BUFFER_FRAME[11]<<3) & 0x7FF;
        case SBUS_CHANNEL_09:
            return (SBUS_BUFFER_FRAME[12] | SBUS_BUFFER_FRAME[13]<<8) & 0x7FF;
        case SBUS_CHANNEL_10:
            return (SBUS_BUFFER_FRAME[13]>>3 | SBUS_BUFFER_FRAME[14]<<5) & 0x7FF;
        case SBUS_CHANNEL_11:
            return (SBUS_BUFFER_FRAME[14]>>6 | SBUS_BUFFER_FRAME[15]<<2 | SBUS_BUFFER_FRAME[16]<<10) & 0x7FF;
        case SBUS_CHANNEL_12:
            return (SBUS_BUFFER_FRAME[16]>>1 | SBUS_BUFFER_FRAME[17]<<7) & 0x7FF;
        case SBUS_CHANNEL_13:
            return (SBUS_BUFFER_FRAME[17]>>4 | SBUS_BUFFER_FRAME[18]<<4) & 0x7FF;
        case SBUS_CHANNEL_14:
            return (SBUS_BUFFER_FRAME[18]>>7 | SBUS_BUFFER_FRAME[19]<<1 | SBUS_BUFFER_FRAME[20]<<9) & 0x7FF;
        case SBUS_CHANNEL_15:
            return (SBUS_BUFFER_FRAME[20]>>2 | SBUS_BUFFER_FRAME[21]<<6) & 0x7FF;
        case SBUS_CHANNEL_16:
            return (SBUS_BUFFER_FRAME[21]>>5 | SBUS_BUFFER_FRAME[22]<<3) & 0x7FF;
    }
}



#undef USART
