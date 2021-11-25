#ifndef INTERRUPT_H
#define INTERRUPT_H

void print_registers(uint32_t reg[16]);

void print_pcr(uint32_t psr[2]);

void print_status(unsigned int psr);

void print_dfsr(unsigned int sr);

void print_ifsr(unsigned int sr);

#endif