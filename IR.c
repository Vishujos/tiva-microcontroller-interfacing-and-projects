#include<stdint.h>
#include<stdbool.h>
#include<inc/tm4c123gh6pm.h>
#include<inc/hw_memmap.h>
#include<driverlib/gpio.h>
#include<driverlib/sysctl.h>
#include<driverlib/uart.h>
#include<driverlib/pin_map.h>
#include<utils/uartstdio.h>
#include<utils/uartstdio.c>
int a;
#include<string.h>
void UART(){
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE,SysCtlClockGet());
    UARTStdioConfig(0, 9600,SysCtlClockGet());

}
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE ,  GPIO_PIN_3);
    UART();
    while(1){

        if (GPIOPinRead(GPIO_PORTE_BASE ,  GPIO_PIN_3)==0){
            UARTprintf("Objected not detected\n");
        }
        else{
            UARTprintf("Objected  detected\n");
        }
    }



}
