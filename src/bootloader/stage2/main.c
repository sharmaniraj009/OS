#include "stdint.h"
#include "stdio.h";

void _cdecl cstart_ (uint16_t bootDrive)
{
    // Your initialization code here
    // Use bootDrive parameter as needed
    puts("Hello World Niraj");
    for(;;);
}