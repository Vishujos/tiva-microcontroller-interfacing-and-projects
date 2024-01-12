#include<stdint.h>
#include<stdbool.h>
#include<inc/tm4c123gh6pm.h>
#include<inc/hw_memmap.h>
#include<driverlib/gpio.h>
#include<driverlib/sysctl.h>
#include<driverlib/timer.h>
#include<driverlib/interrupt.h>
int a,b=0;
// PE_1: - RED LED
// PE_2: - YELLOW LED
//PE_3: - GREEN LED
int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_8|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL);

    // Enable peripheral for external LED's
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    // Enable peripheral for timers
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // Configure timer
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    TimerEnable(TIMER0_BASE,TIMER_A);

    a=SysCtlClockGet();
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x02);

    // Load the Timer
    TimerLoadSet(TIMER0_BASE,TIMER_A,a);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);


    while(1){

    }
}

void ISR_timer()
{
    b+=a;



             if(a*10==b){
                 GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);
                GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2, 0x04);

            }

            else if(a*15==b){
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x08);

            }
            else if(a*25==b){

                            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x00);

                        }





    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}

