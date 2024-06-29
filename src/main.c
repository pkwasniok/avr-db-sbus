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



ISR(USART2_RXC_vect)
{
    sbus_interrupt_handler();
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
    sbus_init();

    sei();

    _delay_ms(100);

    while(1)
    {
        uint16_t sbus_channel_01 = sbus_channel_read(SBUS_CHANNEL_06);
        printf("%d\n", sbus_channel_01);
    }
}
