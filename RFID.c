#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <driverlib/pin_map.h>


#include<inc/tm4c123gh6pm.h>
#include<inc/hw_memmap.h>
#include<driverlib/gpio.h>
#include<driverlib/sysctl.h>
#include<driverlib/uart.h>

#include<utils/uartstdio.h>
#include<utils/uartstdio.c>


char data;
 char dest[11];

 int i=0;
 int j=0;
char User1[11] = "$0013127726";
char User2[11] = "$0013142483";
int red=0,blue=0;

int Rahul_Attendance = 0;
int Praveen_Attendance = 0;
int Total_Classes = 80;

void print_init() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, SysCtlClockGet());
    UARTStdioConfig(0, 9600, SysCtlClockGet());
}

void UART_1() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART1_BASE, SysCtlClockGet());
    UARTStdioConfig(1, 9600, SysCtlClockGet());
}



int main(void) {
    print_init();
    UART_1();
    int j;
    while (1) {
        if (data = UARTCharGet(UART1_BASE)) {
            UARTCharPut(UART0_BASE, data);
            dest[i] = data;
            i++;

            if(i>12)
                             {
                                i=0;
                                for(j=0;j<10;j++)
                                {
                                    if(User1[j+1]==dest[j+1])
                                    {
                                        red=1;
                                    }
                                    else
                                    {
                                        red=0;
                                    }
                                }

                                for(j=0;j<10;j++)
                                {
                                    if(User2[j+1]==dest[j+1])
                                    {
                                        blue=1;
                                     }
                                    else
                                     {
                                        blue=0;
                                     }
                                 }

                                 if(red==1)
                                 {
                                     Rahul_Attendance++;
                                     print_init();
                                    UARTprintf("Rahul Present\n");
                                    UARTprintf("Attended=%d out of %d classes\n",Rahul_Attendance,Total_Classes);



                                  }

                                     else if(blue==1)
                                     {
                                         Praveen_Attendance++;
                                         print_init();
                                        UARTprintf("Praveen Present\n");
                                        UARTprintf("Attended=%d out of %d classes\n",Praveen_Attendance,Total_Classes);
                                     }

                                      else
                                      {
                                         print_init();
                                         UARTprintf("Invalid card\n");
                                      }
                                  }
                              }
             }
         }
