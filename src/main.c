#define F_CPU 24000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "./usart.h"
#include "./sbus.h"

#define SBUS_HEADER 0x0F
#define SBUS_FOOTER 0x81



int usart0_putchar(char c, __attribute__((unused)) FILE *_stream)
{
    usart_write(&USART0, c);
    return 0;
}

FILE usart_stream = FDEV_SETUP_STREAM(usart0_putchar, NULL, _FDEV_SETUP_WRITE);



#define SBUS_BUFFER_SIZE 1024

uint8_t BUFFER_H[SBUS_BUFFER_SIZE];
uint8_t BUFFER_L[SBUS_BUFFER_SIZE];
uint16_t pointer_head = 0;
uint16_t pointer_tail = 0;



ISR(USART2_RXC_vect)
{
    // Push data onto USART ring buffer
    pointer_head += 1;
    if (pointer_head >= SBUS_BUFFER_SIZE) pointer_head = 0;
    BUFFER_H[pointer_head] = USART2.RXDATAH;
    BUFFER_L[pointer_head] = USART2.RXDATAL;
}


int main(void)
{
    // Set internal clock freq to 24 MHz
    _PROTECTED_WRITE(CLKCTRL.OSCHFCTRLA, 0x09 << 2);

    _delay_ms(1000);

    // Initialize STDIOs
    stdout = &usart_stream;
    stderr = &usart_stream;

    // Initialize ports
    PORTA_DIRSET |= PIN0_bm;
    PORTF_PIN1CTRL = PORT_INVEN_bm; // Required for SBUS

    // Initialize peripherals
    usart_init(&USART0, 115200);
    sbus_init(&USART2);

    sei();

    _delay_ms(100);

    uint8_t current_byte = 0;
    uint8_t prev_byte = 0;

    uint8_t frame_buffer[100];
    uint8_t frame_pointer = 0;

    uint16_t channels[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    while (1)
    {
        while (pointer_tail != pointer_head)
        {
            // Pop data from USART ring buffer
            pointer_tail += 1;
            if (pointer_tail > SBUS_BUFFER_SIZE) pointer_tail = 0;
            uint8_t data_l = BUFFER_L[pointer_tail];
            uint8_t data_h = BUFFER_H[pointer_tail];

            prev_byte = current_byte;
            current_byte = data_l;

            // Detect start of new frame
            if (prev_byte == 0x00 && current_byte == 0x0F)
            {
                frame_pointer = 0;

                channels[0] = (frame_buffer[2]<<8 | frame_buffer[1]) & 0x7FF;
                channels[1] = (frame_buffer[3]<<5 | frame_buffer[2]>>3) & 0x7FF;
                channels[2] = (frame_buffer[5]<<10 | frame_buffer[4]<<2 | frame_buffer[3]>>6) & 0x7FF;
                channels[3] = (frame_buffer[5]>>1 | frame_buffer[6]<<7) & 0x7FF;
                channels[4] = (frame_buffer[6]>>4 | frame_buffer[7]<<4) & 0x7FF;
                channels[5] = (frame_buffer[7]>>7 | frame_buffer[8]<<1 | frame_buffer[9]<<9) & 0x7FF;
                channels[6] = (frame_buffer[9]>>2 | frame_buffer[10]<<6) & 0x7FF;
                channels[7] = (frame_buffer[10]>>5 | frame_buffer[11]<<3) & 0x7FF;
                channels[8] = (frame_buffer[12] | frame_buffer[13]<<8) & 0x7FF;
                channels[9] = (frame_buffer[13]>>3 | frame_buffer[14]<<5) & 0x7FF;
                channels[10] = (frame_buffer[14]>>6 | frame_buffer[15]<<2 | frame_buffer[16]<<10) & 0x7FF;
                channels[11] = (frame_buffer[16]>>1 | frame_buffer[17]<<7) & 0x7FF;
                channels[12] = (frame_buffer[17]>>4 | frame_buffer[18]<<4) & 0x7FF;
                channels[13] = (frame_buffer[18]>>7 | frame_buffer[19]<<1 | frame_buffer[20]<<9) & 0x7FF;
                channels[14] = (frame_buffer[20]>>2 | frame_buffer[21]<<6) & 0x7FF;
                channels[15] = (frame_buffer[21]>>5 | frame_buffer[22]<<3) & 0x7FF;
            }

            // Push data into frame buffer
            frame_buffer[frame_pointer++] = current_byte;
        }

        printf("\033[2J");

        for (int i=0; i<16; i++)
        {
            printf("CH%d: %d\n", i+1, channels[i]);
        }

        _delay_ms(5);
    }
}
