#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#define PF1 (*((volatile long *) 0x40025008))
#define PD (*((volatile long *) 0x40007010))
#define PB (*((volatile long *) 0x40005010))
#define PA2 (*((volatile long *) 0x40004010))
float x=0.0;
uint32_t  tempo=0;
uint32_t pontos[42]={16880,23600,12480,27680,8640,30960,5840,33120,4240,34080,3840,33840,4800,32240,7120,29360,10480,25760,14560,21440,19120,16800,23600,12480,27600,8800,30880,5840,33120,4240,34080,3840,33760,4880,32160,7200,29360,10560,25600,14640,21440,18960}; 
int boco=1;
//uint32_t pontos[4]={16880,16880,16880,16880}; 
int i=1;
void Timer0A_IntHandler(void) //ISR do Timer0A
{
  IntDisable(INT_TIMER0A_TM4C123);
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  TimerDisable(TIMER0_BASE, TIMER_A); //inicia

 if(boco==0)
  {
    PD=0xFF;
   
  SysCtlDelay(280);
     PB=0xFF;
       
    

   

 boco=1;
  }
  else if(boco==1)
  {
      PB=0;
      
   
      SysCtlDelay(280);
       PD=0;
    //PB=1;
 
    //PD=1;
  boco=0;
  }

//PD=~PD;
//PB=~PD;
 x=1.1;
tempo=(pontos[i])/x;
TimerLoadSet(TIMER0_BASE, TIMER_A,tempo); //valor em us

i++;
if(i>=42)
{
  i=0;
  PA2=~PA2;
}
IntEnable(INT_TIMER0A_TM4C123);
TimerEnable(TIMER0_BASE, TIMER_A); //inicia
}
// Configura Timer0A como um temporizador one-shot de 16 bits
int main(void)
{
//SysCtlClockSet(SYSCTL_SYSDIV_8 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
//usa o como base de tempo o cristal de 16 MHz/16 = 1MHz (1 microssegundo).
SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);

////Configuração da GPIO F ////
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);

SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT);

TimerLoadSet(TIMER0_BASE, TIMER_A, pontos[0]); //valor em us

TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
IntEnable(INT_TIMER0A_TM4C123);
IntRegister(INT_TIMER0A_TM4C123, Timer0A_IntHandler);
PD=0xFF;
PB=0xFF;
PA2=0;
TimerEnable(TIMER0_BASE, TIMER_A); //inicia

while(1);
}
    

