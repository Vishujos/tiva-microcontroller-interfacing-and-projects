#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"

#define SLAVE_ADDRESS1   0x30  // slave address of 1 microcontroller
#define SLAVE_ADDRESS2   0x33 // slave address of 2 microcontroller
void InitConsole(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 9600, 16000000);
}

void I2C0_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
//    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    //HWREG(I2C0_BASE + I2C_O_MCR) |= 0x01;
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
}

void I2C0_Send(uint16_t device_address, uint8_t device_data)
{
    // Determine Slave address
    // false: transmit Master --> Slave
    // true:  receive  Master <-- Slave
    I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);
    // Put data
    I2CMasterDataPut(I2C0_BASE, device_data);
    // Transmit data
    UARTprintf("data sent to slave");
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

}

void I2C_SendToSlave1(uint8_t device_data)
{
    I2C0_Send(SLAVE_ADDRESS1, device_data);
}

void I2C_SendToSlave2(uint8_t device_data)
{
    I2C0_Send(SLAVE_ADDRESS2, device_data);
}


int main(void)
{
    uint32_t ui32DataTx;

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    I2C0_Init();
    InitConsole();

    I2CMasterIntEnable(I2C0_BASE);
    IntMasterEnable();

    UARTprintf("I2C Slave Interrupt Example ->");
    UARTprintf("\n  Module = I2C0");
    UARTprintf("\n  Mode = Receive interrupt on the Slave module");
    UARTprintf("\n  Rate = 100kbps\n\n");
    while (1)
    {
        if (UARTCharsAvail(UART0_BASE))
        {
            ui32DataTx = UARTCharGet(UART0_BASE);
            UARTprintf("Transferring from: Master -> Slave\n");
            UARTprintf("  Sending: '%c'", ui32DataTx);

            // Decide which slave to send the data to based on some criteria
            if (ui32DataTx =='R'|ui32DataTx =='G'|ui32DataTx =='B')  // Example: even ASCII values to SLAVE1, odd to SLAVE2
            {
                I2C_SendToSlave1(ui32DataTx);
            }
            else if(ui32DataTx =='r'|ui32DataTx =='g'|ui32DataTx =='b')
            {
                I2C_SendToSlave2(ui32DataTx);
            }
        }
    }

    }
