#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

#define ROWS 4
#define COLS 4

#define KEYPAD_BASE GPIO_PORTB_BASE
#define ROW_PINS GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
#define COL_PINS GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7

void UARTConfig(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void KeypadInit(void);
char GetPressedKey(void);

int main(void)
{
  // Set the clock to 50 MHz
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  UARTConfig();
  KeypadInit();

  // Add the following line to send the string "Hello, world!\n" to the serial monitor when the program starts.
  UARTSend((const uint8_t *)"Hello, world!\n", 13);

  while (1)
  {
    char key = GetPressedKey();
    if (key != '\0')
    {
      UARTSend((const uint8_t *)&key, 1);
    }
  }
}

void UARTConfig(void)
{
  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  // Configure UART pins
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Initialize the UART for 115200 baud rate
  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                     (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                     UART_CONFIG_PAR_NONE));
}

void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
  while (ui32Count--)
  {
    UARTCharPut(UART0_BASE, *pui8Buffer++);
  }
}

void KeypadInit(void)
{
  // Enable GPIO Port B
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

  // Set ROW_PINS as outputs and COL_PINS as inputs
  GPIOPinTypeGPIOOutput(KEYPAD_BASE, ROW_PINS);
  GPIOPinTypeGPIOInput(KEYPAD_BASE, COL_PINS);

  // Enable pull-up resistors on COL_PINS
  GPIOPadConfigSet(KEYPAD_BASE, COL_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

char GetPressedKey(void)
{
  char keypad[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };

  uint8_t i, j;

  for (i = 0; i < ROWS; i++)
  {
    GPIOPinWrite(KEYPAD_BASE, ROW_PINS, (1 << i));

    for (j = 0; j < COLS; j++)
    {
      if (!(GPIOPinRead(KEYPAD_BASE, COL_PINS) & (1 << j)))
      {
        while (!(GPIOPinRead(KEYPAD_BASE, COL_PINS) & (1 << j)))
          ; // Wait for key release

        return keypad[i][j];
      }
    }
  }

  return '\0'; // No key pressed
}
