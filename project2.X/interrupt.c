
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "led.h"
// The following defines the time between interrupts
#define TMR_TIME    0.00195325 // 1953.25 us for each tick (interrupt roughly every 2ms)

/***	Timer1ISR
**
**	Description:
**		This is the interrupt handler for Timer1. It is used to implement proper SSD display.
**      Every time the interrupt occurs, the next of the 4 digits is displayed (in a circular manner).
**      First it deactivates all digits (anodes), then it drives the segments (cathodes) according to the digit that is displayed,
**      then the current digit is activated. 
**      This happens faster than the human eye can notice.
**          
*/

int switch_flag0=0;
int switch_flag1=0;
int switch_flag2=0;
int switch_flag3=0;
int switch_flag4=0;

int btnl_flag = 0;
int btnl_state_flag = 0;

int btnc_flag = 0;
int btnc_state_flag = 0;

int btnd_flag = 0;
int btnd_state_flag = 0;

int btnu_flag = 0;
int btnu_state_flag=0;

int btnr_flag = 0;
int btnr_state_flag = 0;

unsigned int counter = 0;
int btnc_counter = 0;
int btnd_counter = 0;

unsigned int  baseCnt = 0;
void __ISR(_TIMER_1_VECTOR, ipl7) Timer1ISR(void) 
{
    /*handles interrupt every 200ms*/
    if(baseCnt++ == 100)     
    {
        
    LED_SetValue(0,1);
            
    if(SWT_GetValue(0)) switch_flag0=1;
    else switch_flag0=0;
    if(SWT_GetValue(1)) switch_flag1=1;
    else switch_flag1=0;
    if(SWT_GetValue(2)) switch_flag2=1;
    else switch_flag2=0;
    if(SWT_GetValue(3)) switch_flag3=1;
    else switch_flag3=0;
    
    if(BTN_GetValue(0)&&btnu_state_flag==0) {
        btnu_flag = 1;
        btnu_state_flag=1;
    }
    else if(BTN_GetValue(0)==0) btnu_state_flag = 0;
    else btnu_flag = 0;
    
    if(BTN_GetValue(1)&&btnl_state_flag==0) {
        btnl_flag = 1;
        btnl_state_flag=1;
    }
    else if(BTN_GetValue(1)==0) btnl_state_flag = 0;
    else btnl_flag = 0;
     
    if(BTN_GetValue(2)&&btnc_state_flag==0) {
        btnc_flag = 1;
        btnc_state_flag=1;
    }
    else if(BTN_GetValue(2)==0) btnc_state_flag = 0;
    else btnc_flag = 0;
         
    if(BTN_GetValue(3)&&btnr_state_flag==0) {
        btnr_flag = 1;
        btnr_state_flag=1;
    }
    else if(BTN_GetValue(3)==0) btnr_state_flag = 0;
    else btnr_flag = 0;
    
    if(BTN_GetValue(4)&&btnd_state_flag==0) {
        btnd_flag = 1;
        btnd_state_flag=1;
    }
    else if(BTN_GetValue(4)==0) btnd_state_flag = 0;
    else btnd_flag = 0;
    
    
        
    
    
    baseCnt = 0;        
    }
    if(baseCnt == 16) counter++;
    if(counter == 4294967295) counter = 0;
    

   
    
    IFS0bits.T1IF = 0;     
    
    

    
}


void Timer1Setup()
{
  // The following configurations set the PR1 value and the PreScaling factor 
  PR1 = (int)(((float)(TMR_TIME * PB_FRQ) / 64)); //set period register, generates one interrupt every 2 ms
  TMR1 = 0;                           //    initialize count to 0
  T1CONbits.TCKPS = 2;                //    1:64 prescale value
  
  T1CONbits.TGATE = 0;                //    not gated input (the default)
  T1CONbits.TCS = 0;                  //    PCBLK input (the default)
  T1CONbits.ON = 1;                   //    turn on Timer1
  IPC1bits.T1IP = 7;                  //    priority
  IPC1bits.T1IS = 3;                  //    subpriority
  IFS0bits.T1IF = 0;                  //    clear interrupt flag
  IEC0bits.T1IE = 1;                  //    enable interrupt
  macro_enable_interrupts();          //    enable interrupts at CPU
}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	SSD_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the SSD module:
**      The pins corresponding to SSD module are initialized as digital outputs.
**      The Timer1 is initialized to generate interrupts every approx. 3 ms.
**      
**          
*/
void TMR_Init()
{
   
    Timer1Setup();  
}

/* ------------------------------------------------------------ */
/***	SSD_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins involved in the SSD module as digital output pins. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by SSD_Init(), so user should avoid calling it directly.       
**          
*/
