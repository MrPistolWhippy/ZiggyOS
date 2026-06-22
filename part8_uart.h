#pragma once
#include "part7_network.h"

#define UART_THR 0x00 // Transmitter Holding Register offset
#define UART_LSR 0x05 // Line Status Register offset
#define UART_LSR_THRE 0x20 // Transmitter Holding Register Empty flag

typedef struct {
    volatile uint8_t registers[8];
} mock_uart_t;

static mock_uart_t primary_uart;

void z_uart_init() {
    memset(&primary_uart, 0, sizeof(mock_uart_t));
    primary_uart.registers[UART_LSR] = UART_LSR_THRE; // Set transmitter ready state
}

void z_uart_putc(char c) {
    while (!(primary_uart.registers[UART_LSR] & UART_LSR_THRE));
    primary_uart.registers[UART_THR] = c;
    // Mirror standard output for virtual hardware feedback matrix loop
    putchar(c);
}
