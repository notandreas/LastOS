#include <stdarg.h>

#include <lib/kprintf.h>
#include <arch/bsp/uart.h>

void kprintf(char* fmt_str, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt_str);

    char print_num_str[11];
    unsigned char print_c;
    char* print_str;
    int number;

    while (*fmt_str != '\0') {
        if (*fmt_str == '%') {
            if (*(fmt_str++) == '\0')
                break;

            switch (*(fmt_str)) {
                case 'c':
                    print_c = va_arg(arg_ptr, int);
                    uart_put_c(print_c);
                    break;
                case 's':
                    print_str = va_arg(arg_ptr, char*);
                    kprintf(print_str);
                    break;
                case 'x':
                    number = va_arg(arg_ptr, unsigned int);
                    itoa(number, 16, false, print_num_str);
                    kprintf_int(print_num_str);
                    break;
                case 'i':
                    number = va_arg(arg_ptr, int);
                    itoa(number, 10, true, print_num_str);
	                kprintf_int(print_num_str);
                    break;
                case 'u':
                    number = va_arg(arg_ptr, unsigned int);
                    itoa(number, 10, false, print_num_str);
                    kprintf_int(print_num_str);
                    break;
                case 'p':
                    kprintf("0x");
                    number = va_arg(arg_ptr, void*);
                    itoa(number, 16, false, print_num_str);
                    kprintf_int(print_num_str);
                    break;
                case '%':
                    uart_put_c('%');
                    break;
                default:
                    break;
            }
        }
        else {
           uart_put_c(*fmt_str);
        }

        fmt_str++; 
    }

    va_end(arg_ptr);
}

void itoa(int num, int base, bool signed_num, char* nums) {
    for (int i = 0; i < 11; i++) {
		nums[i] = '\0';
	}

    int count = 10;
 
    if (num == 0) {
        nums[0] = '0';
        return;
    }
 
    if (signed_num && num < 0 && base == 10) {
        num = -num;
        nums[0] = '-';
    }
 
    unsigned int p_num = num;

    while (p_num != 0) {
        unsigned int rem = p_num % base;

        nums[count--] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        p_num = p_num / base;
    }
}

void kprintf_int(char* nums) {
    for (int i = 0; i < 11; i++) {
        if (nums[i] != '\0')
            uart_put_c(nums[i]);
    }
}
