/*!
 * @file sbus.h
 *
 * @brief Library for handling SBUS protocool on AVR DB family MCUs.
*/

#ifndef SBUS_H_
#define SBUS_H_



// ===========================================
// ========== LIBRARY CONFIGURATION ==========
// ===========================================



#define LIBCONFIG_SBUS_F_CLK_PER       24000000
#define LIBCONFIG_SBUS_USART_BAUD_RATE 100000
#define LIBCONFIG_SBUS_FRAME_HEADER    0x0F
#define LIBCONFIG_SBUS_FRAME_FOOTER    0x00

// Choose one option
// #define LIBCONFIG_SBUS_USART_0
// #define LIBCONFIG_SBUS_USART_1
#define LIBCONFIG_SBUS_USART_2
// #define LIBCONFIG_SBUS_USART_3
// #define LIBCONFIG_SBUS_USART_4
// #define LIBCONFIG_SBUS_USART_5

#if defined LIBCONFIG_SBUS_USART_0
    #define LIBCONFIG_SBUS_USART_PERIPH USART0
#elif defined LIBCONFIG_SBUS_USART_1
    #define LIBCONFIG_SBUS_USART_PERIPH USART1
#elif defined LIBCONFIG_SBUS_USART_2
    #define LIBCONFIG_SBUS_USART_PERIPH USART2
#elif defined LIBCONFIG_SBUS_USART_3
    #define LIBCONFIG_SBUS_USART_PERIPH USART4
#elif defined LIBCONFIG_SBUS_USART_4
    #define LIBCONFIG_SBUS_USART_PERIPH USART4
#elif defined LIBCONFIG_SBUS_USART_5
    #define LIBCONFIG_SBUS_USART_PERIPH USART5
#else
    #error Invalid SBUS configuration
#endif



// ======================================
// ========== DEFINITIONS ===============
// ======================================



#define SBUS_CHANNEL_01 0
#define SBUS_CHANNEL_02 1
#define SBUS_CHANNEL_03 2
#define SBUS_CHANNEL_04 3
#define SBUS_CHANNEL_05 4
#define SBUS_CHANNEL_06 5
#define SBUS_CHANNEL_07 6
#define SBUS_CHANNEL_08 7
#define SBUS_CHANNEL_09 8
#define SBUS_CHANNEL_10 9
#define SBUS_CHANNEL_11 10
#define SBUS_CHANNEL_12 11
#define SBUS_CHANNEL_13 12
#define SBUS_CHANNEL_14 13
#define SBUS_CHANNEL_15 14
#define SBUS_CHANNEL_16 15



// ===================================
// ========== HANDLERS ===============
// ===================================



/*!
 * @brief Handles SBUS interrupt
 * 
 * @note This function should be placed in USARTx_RXC interrupt vector.
*/
void sbus_usart_rxc_interrupt_handler();



// =========================================
// ========== INITIALIZATION ===============
// =========================================



/*!
 * @brief Initializes SBUS.
 *
 * This function initializes SBUS communication on specified USART by doing following:
 * 1. Set USART baud rate to 100000 bps
 * 2. Set USART frame format to 8 data, even parity, 2 stop.
 * 3. Enable USART RXC interrupt
 * 4. Enable USART RX
*/
void sbus_init();



// ===================================
// ========== READINGS ===============
// ===================================



/*!
 * @brief Reads value of specified SBUS channel.
 *
 * This function reads value of specified SBUS channel. SBUS channel value is in range 172-1811.
 *
 * @param[in] channel SBUS channel to read value from.
 * 
 * @returns Value of specified SBUS channel.
*/
uint16_t sbus_channel_read(uint8_t channel);



#endif // SBUS_H_
