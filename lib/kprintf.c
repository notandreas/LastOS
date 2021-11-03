#include <stdarg.h>

#include <kernel/panic.h>

#include <lib/kprintf.h>
#include <arch/bsp/uart.h>

__attribute__((format(printf, 1, 2)))
void kprintf(char* fmt_str, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt_str);

    // Flags
    bool field_flag = false;
    bool zeros_flag = false;

    // Variables to handle input.
    char print_num_str[12];
    unsigned char print_c;
    char* print_str;
    int number;

    while (*fmt_str != '\0') {
        field_flag = false;
        zeros_flag = false;

        // If is a Formatter.
        if (*fmt_str == '%') {

            // Test next char for a 0 or 8 Flag.
            fmt_str++;
            if (*(fmt_str) == '0') {
                zeros_flag = true;
                fmt_str++;
            }
            if (*(fmt_str) == '8') {
                field_flag = true;
                fmt_str++;
            }

            // If 0 padding is used but has no padding.
            if (zeros_flag && !field_flag) {
                panic("Use of %%0 without padding and a flag!");
            }

            // Check the formatting flag.
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
                    kprintf_int(print_num_str, false, field_flag, zeros_flag);
                    break;
                case 'i':
                    number = va_arg(arg_ptr, int);
                    itoa(number, 10, true, print_num_str);
	                kprintf_int(print_num_str, false, field_flag, zeros_flag);
                    break;
                case 'u':
                    number = va_arg(arg_ptr, unsigned int);
                    itoa(number, 10, false, print_num_str);
                    kprintf_int(print_num_str, false, field_flag, zeros_flag);
                    break;
                case 'p':
                    number = (int) va_arg(arg_ptr, void*);
                    itoa(number, 16, false, print_num_str);
                    print_num_str[11] += 2;
                    kprintf_int(print_num_str, true, field_flag, zeros_flag);
                    break;
                case '%':
                    uart_put_c('%');
                    break;
                default:
                    panic("No flags after %%!");
                    break;
            }
        }
        else {
           uart_put_c(*fmt_str);
        }

        // If no formatter is used JUST PRINT.
        fmt_str++; 
    }

    va_end(arg_ptr);
}

void itoa(int num, int base, bool signed_num, char* nums) {
    // clear String
    for (int i = 0; i < 11; i++) {
		nums[i] = '\0';
	}

    // to count number spaces.
    nums[11] = 0;
    int count = 10;
 
    if (num == 0) {
        nums[0] = '0';
        nums[11]++;
        return;
    }
 
    // add a minus if required.
    if (signed_num && num < 0 && base == 10) {
        num = -num;
        nums[0] = '-';
        nums[11]++;
    }

    // convert number to decimal/hex.
    unsigned int p_num = num;

    while (p_num != 0) {
        unsigned int rem = p_num % base;

        nums[count--] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        p_num = p_num / base;
        nums[11]++;
    }
}

void kprintf_int(char* nums, bool hex_prefix, bool field_flag, bool zeros_flag) {
    int n = 0;

    // calculate and print padding.
    if (field_flag) {
        // calculate padding.
        int padding = 8 - nums[11];
        padding = padding < 0 ? 0 : padding;

        // print minus in right place
        if (zeros_flag && nums[0] == '-') {
            uart_put_c('-');
            n++;
        }

        if (hex_prefix && zeros_flag)
            kprintf("0x");

        // print padding.
        for (int i = 0; i < padding; i++) {
            if (zeros_flag) {
                uart_put_c('0');
            }
            else
                uart_put_c(' ');
        }

        if (hex_prefix && !zeros_flag)
            kprintf("0x");
    }

    if (hex_prefix && !field_flag)
            kprintf("0x");

    // print number.
    for (; n < 11; n++) {
        if (nums[n] != '\0')
            uart_put_c(nums[n]);
    }
}
