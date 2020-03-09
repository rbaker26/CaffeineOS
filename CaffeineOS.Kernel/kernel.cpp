//#include <cstdint>
//void printf(const char* str)
//{
//	//The monitor will print anything that is inserted into 0xb8000 memory location
//	static unsigned short* VideoMemory = (unsigned short*)0xb8000;
//
//	for (int i = 0; str[i] != '\0'; ++i)
//	{
//		VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
//	}
//}
//
//typedef void (*constructor)();
//extern "C" constructor * start_ctors;
//extern "C" constructor * end_ctors;
//extern "C" void callConstructors()
//{
//	for (constructor* i = start_ctors; i != end_ctors; i++) {
//		(*i)();
//	}
//}
//
//
//extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber)
//{
//	printf("Hello World");
//	while (1);
//#include "types.h"

#include <cstdint>
#include <cstddef>
#include "multiboot.h"
#include "kernel.h"

//*************************************************************************
// Console Output
//*************************************************************************

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t)uc | (uint16_t)color << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*)0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	//*******************************************************
	// Special Charecters Decorder
	//*******************************************************
	// This will handle all escape charecters like \n and \t
	if (c == '\n')
	{
		++terminal_row;
		terminal_column = -1;
		c = '\0';
	}
	else if (c == '\t')
	{
		terminal_column += 5;
		c = '\0';

	}
	//*******************************************************


	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
			//TODO clear the grid
	}

	
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}


//*************************************************************************
// printf() 
//*************************************************************************
void _printf(const char* s)
{
	terminal_writestring(s);
}
void _printf(char c)
{
	char buf[2] = { 0 };
	buf[0] = c;
	_printf(buf);
	buf[0] = 0;
}


void _printf(int i)
{
	if (i > 0)
	{
		int count = 0;
		int temp = i;
		while (i >> 10)
			count++;

		
		char buf[11];
		_atoi(i, buf, count);
	}
	else if (i < 0)
	{
		
	}
	else if (i == 0)
	{
		_printf('0');
	}
	else
	{
		_printf("NaN");
	}
}
//*************************************************************************

char _atoi(int i)
{
	char map[10] = { '0', '1','2','3','4','5','6','7','8','9' };
	if (i < 0 || i > 9)
		return '\0';
	return map[i];
}
int _atoi(char c) 
{
	int i = (char)c - 48;
	if (i < 0 || i > 9)
		return '\0';
	return i;
}

void _atoi(int i, char* buf, int size)
{
	if (size > 11)
		return;
	char buf_rev[11] = { 0 };
	int temp = 0;
	for (int index = 0; index < size; index++)
	{
		temp = i % 10;
		buf_rev[index] = _atoi(temp);

		i -= temp;
		i /= 10;
	}

	int rev_index = 0;
	for (int index = size - 1; index >= 0; index--)
	{
		buf[rev_index] = buf_rev[index];
		rev_index++;
	}
	buf[rev_index] = '\0';

}
//*************************************************************************


typedef void (*constructor)();
extern "C" constructor * start_ctors;
extern "C" constructor * end_ctors;



extern "C" void callConstructors()
{
    for (constructor* i = start_ctors; i != end_ctors; i++) {
        (*i)();
    }
}



void display_banner()
{
	terminal_writestring("************************************************************ \n");
	terminal_writestring("*    ______      ________     _            ____  _____     * \n");
	terminal_writestring("*   / ____/___ _/ __/ __/__  (_)___  ___  / __ \\/ ___/     * \n");
	terminal_writestring("*  / /   / __ `/ /_/ /_/ _ \\/ / __ \\/ _ \\/ / / /\\__ \\\t* \n");
	terminal_writestring("* / /___/ /_/ / __/ __/  __/ / / / /  __/ /_/ /___/ /      * \n");
	terminal_writestring("* \\____/\\__,_/_/ /_/  \\___/_/_/ /_/\\___/\\____//____/ \t* \n");
	terminal_writestring("************************************************************ \n");
	terminal_writestring("* CaffeineOS 1.0                                           * \n");
	terminal_writestring("* cc Bob Baker 2020                                        * \n");
	terminal_writestring("************************************************************ \n");
}

//char getchar()
//{
//	uint16_t* terminal_ibuffer = (uint16_t*)0xB8000;// todo find correct irq number
//}

//extern "C" void kernelMain(multiboot_info_t* mbd, uint32_t magicnumber)
void detect_memory(const multiboot_info_t * mbd, multiboot_uint32_t& lo_mem_size, multiboot_uint32_t & hi_mem_size)
{
	// saftey check always not zero???
	if (true/*mbd->flags == 0*/)
	{
		multiboot_uint32_t lo_mem_size = mbd->mem_lower;
		multiboot_uint32_t hi_mem_size = mbd->mem_upper;
	}

}


void print_config(const multiboot_info_t * mbd)
{
	terminal_writestring("\n");
	terminal_writestring("************************************************************ \n");
	terminal_writestring("* Config                                                   * \n");
	terminal_writestring("************************************************************ \n");
	terminal_writestring("* Boot Loader: ");
	terminal_writestring((const char*)mbd->boot_loader_name);
	terminal_writestring("                       *\n");
	terminal_writestring("*                                                          *\n");
	multiboot_uint32_t lo_mem_size, hi_mem_size;
	detect_memory(mbd, lo_mem_size, hi_mem_size);


	terminal_writestring("* RAM Detected:                                            *\n");
	char buf[11];
	terminal_writestring("*   Lower Memory: ");
	_atoi(lo_mem_size, buf, 8);
	terminal_writestring(buf);
	terminal_writestring(" kibibytes                       *\n");

	char buf2[11];
	terminal_writestring("*   Upper Memory: ");
	_atoi(hi_mem_size, buf2, 8);
	terminal_writestring(buf2);
	terminal_writestring(" kibibytes                       *\n");


	char buf3[11];
	multiboot_uint32_t total_mem_size = lo_mem_size + hi_mem_size;
	terminal_writestring("*   Total Memory: ");
	_atoi(total_mem_size, buf3, 8);
	terminal_writestring(buf3);
	terminal_writestring(" kibibytes                       *\n");
	terminal_writestring("************************************************************ \n");


}


extern "C"
void kernelMain(multiboot_info_t* mbd, uint32_t magicnumber)
{
	/* Initialize terminal interface */
	terminal_initialize();
	terminal_setcolor(vga_color::VGA_COLOR_WHITE);
	display_banner();

	//terminal_writestring((const char*)mbd->mmap_length);
	//_printf(251);


	//multiboot_uint32_t lo_mem_size, hi_mem_size;
	//detect_memory(mbd, lo_mem_size,hi_mem_size);


	print_config(mbd);



    while (1);
}

