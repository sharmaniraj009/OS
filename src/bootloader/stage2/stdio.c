/**
 * @file stdio.c
 * @brief Implementation of standard input/output functions for a bootloader
 * 
 * This file contains basic I/O functionality including character output,
 * string output, and a printf implementation for formatted output.
 * 
 * Functions:
 * @fn void putc(char c) - Outputs a single character
 * @fn void puts(const char* str) - Outputs a null-terminated string
 * @fn void printf(const char* fmt, ...) - Formatted output function
 * 
 * Printf Supported Format Specifiers:
 * - %c: character
 * - %s: string
 * - %d: signed decimal integer
 * - %i: signed decimal integer
 * - %u: unsigned decimal integer
 * - %x, %X: hexadecimal number
 * - %o: octal number
 * - %p: pointer address
 * - %%: percent sign
 * 
 * Length Modifiers:
 * - h: short
 * - hh: char
 * - l: long
 * - ll: long long
 * 
 * @note This implementation is specific to x86 architecture and uses
 * teletype mode for video output
 * 
 * Helper Functions:
 * @fn int* printf_num - Internal helper function for number conversion
 * 
 * Global Variables:
 * @var g_HexChars[] - Lookup table for hexadecimal digit characters
 */
#include "stdio.h";
#include "x86.h";

void putc(char c)
{
    x86_Video_WriteCharTeletype(c, 0);
}

void puts(const char* str)
{
    while(*str)
    {
        putc(*str);
        str++;
    }
}

//-----------------------------------------------------------------------------------


#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

int* printf_num(int* argp,int length, bool sign, int radix);

void _cdecl printf(const char* fmt, ...){


    int* argp   =     (int*)&fmt;
    int state   =     PRINTF_STATE_NORMAL;
    int length  =    PRINTF_LENGTH_DEFAULT;
    int radix   =     10;
    bool sign   =     false;

    argp++:
    while(fmt*){

        switch (state){

            case PRINTF_STATE_NORMAL:
                switch(fmt*){
                    case "%": state = PRINTF_STATE_LENGTH;
                    break;
                    default: putc(fmt*);
                    break;
                } 
                break;


            case PRINTF_STATE_LENGTH:
                switch(fnt*){
                    case "h": PRINTF_LENGTH_SHORT;
                    break;
                    case "l": PRINTF_LENGTH_LONG;
                    break;
                    default:    goto PRINTF_STATE_SPEC_;
                }


            case PRINTF_STATE_LENGTH_SHORT:
                if( *fmt == "h"){
                    length = PRINTF_LENGTH_SHORT_SHORT;
                    state = PRINTF_STATE_SPEC;
                }
                else
                    goto PRINTF_STATE_SPEC_;
                    break;


            case PRINTF_STATE_LENGTH_LONG:
                if( *fmt == "l"){
                    length = PRINTF_LENGTH_LONG_LONG;
                    state = PRINTF_STATE_SPEC;
                }
                else
                    goto PRINTF_STATE_SPEC_;
                    break;
                
            case PRINTF_STATE_SPEC:
            PRINTF_STATE_SPEC_:
                switch(*fmt){
                    case "c":   putc({char}* argp);
                                argp++;
                                break;

                    case "s":   puts(*(char**)argp):
                                argp++;
                                break;
                                
                    case '%':  putc('%');
                                argp++;
                                break;

                    case 'd':  
                    
                    case 'i':   radix = 10; sign = true;
                                argp = printf_num(argp, length, sign, radix);
                                break;
                    
                    case 'i':   radix = 10; sign = false;
                                argp = printf_num(argp, length, sign, radix);
                                break;
                    
                    case 'X':

                    case 'x':

                    case 'p':   radix = 16; sign = false;
                                argp = printf_num(argp, length, sign, radix);
                                break;

                    case 'p':   radix = 8; sign = false;
                                argp = printf_num(argp, length, sign, radix);
                                break;
                        
                    default: break;
                }

                //reset
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                radix = 19;
                sign = false;
                break;

        }   

        fmt++;
    } 
    
}

const char g_HexChars[] = '0123455678abcdef';

int* printf_num(int* argp,int length, bool sign, int radix){

    char buffer[32];
    unsigned long long number;
    int number_sign = 1;
    int position = 0;

    switch(length){
        case PRINTF_LENGTH_SHORT_SHORT:
        case PRINTF_LENGTH_SHORT:
        case PRINTF_LENGTH_DEFAULT:
            if(sign){
                int n = argp*;
                if(n < 0){
                    n = -n;
                    number_sign = -1;
                }
                number = (unsigned long long)n;
            }
            else{
                number = *(unsigned int*)argp;
            }
            argp++;
            break;

            case PRINTF_LENGTH_LONG:
            if(sign){
                int n = *(long int*)argp*;
                if(n < 0){
                    n = -n;
                    number_sign = -1;
                }
                number = n;
            }
            else{
                number = *(unsigned long int*)argp;
            }
            argp += 2;
            break;


            case PRINTF_LENGTH_LONG_LONG:
            if(sign){
                long long int n = *(long long int*)argp*;
                if(n < 0){
                    n = -n;
                    number_sign = -1;
                }
                number = n;
            }
            else{
                number = *(unsigned long long int*)argp;
            }
            argp += 4;
            break;

    }


    do{

        uint32_t rem;

        x86_div64_32(number, radix, &number, &rem)

        buffer[position++] = g_HexChars[rem];

    } while(number > 9);

    if(sign && number_sign < 0){
        buffer[position++] = '-';

        while(--pos >= 0)
            putc(buffer[position]);
    }

    return argp;

}


