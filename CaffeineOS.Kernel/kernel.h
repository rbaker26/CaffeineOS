#pragma once

void display_banner();
void print_config(const multiboot_info_t* mbd);
void detect_memory(const multiboot_info_t* mbd, multiboot_uint32_t& lo_mem_size, multiboot_uint32_t& hi_mem_size);




int _atoi(char c);
char _itoa(int i);
void _atoi(int i, char* buf, int size);

void _printf(int i);
void _printf(const char* s);
void _printf(char c);

typedef unsigned int uint32_t;