#include <xc.h>                                         // Include xc.h
#include <p18f4620.h>                                   // Include p18f4620.h
#include "Interrupt.h"                                  // Include Interrupt.h
#include "Main.h"                                       // Include Main.h   

extern int INT0_flag, INT1_flag, INT2_flag, Tach_cnt;   // Variables for INT flags and Tach_cnt

void Init_Interrupt(void)                               // Function to initialize interrupts
{                                               
    INTCONbits.INT0IF = 0;                              // Clear INT0 flag to 0
    INTCON3bits.INT1IF = 0;                             // Clear INT1 flag to 0
    INTCON3bits.INT2IF = 0;                             // Clear INT2 flag to 0
    INTCON2bits.INTEDG0 = 0;                            // INT0 EDGE falling
    INTCON2bits.INTEDG1 = 0;                            // INT1 EDGE falling
    INTCON2bits.INTEDG2 = 1;                            // INT2 EDGE rising
    INTCONbits.INT0IE = 1;                              // Set INT0 interrupt enable to 1
    INTCON3bits.INT1IE = 1;                             // Set INT1 interrupt enable to 1
    INTCON3bits.INT2IE = 1;                             // Set INT2 interrupt enable to 1
    INTCONbits.GIE = 1;                                 // Set the Global Interrupt Enable to 1
    
    INTCONbits.PEIE = 1;                                // Set peripheral interrupt enable to 1
    INTCONbits.TMR0IE = 1;                              // Set TMR0 interrupt enable to 1
    INTCONbits.TMR0IF = 0;                              // Clear TMR0 flag to 0
}

void interrupt  high_priority chkisr()                  // Function to check interrupts
{
    if (INTCONbits.TMR0IF == 1) T0_ISR();               // Timer Interrupt Routine    
    if (INTCONbits.INT0IF == 1) INT0_ISR();             // check the INT0 interrupt
    if (INTCON3bits.INT1IF == 1) INT1_ISR();            // check the INT1 interrupt
    if (INTCON3bits.INT2IF == 1) INT2_ISR();            // check the INT2 interrupt
}

void INT0_ISR()                                         // Function for INT0 service routine
{    
    INTCONbits.INT0IF=0;                                // Clear INT0 flag
    __delay_ms(5);                                      // Delay 5 ms
    INT0_flag = 1;                                      // Set software INT0_flag to 1 
} 

void INT1_ISR()                                         // Function for INT1 service routine
{ 
    INTCON3bits.INT1IF=0;                               // Clear the INT1 flag
    __delay_ms(5);                                      // Delay 5 ms
    INT1_flag = 1;                                      // Set software INT1_flag to 1 
} 

void INT2_ISR()                                         // Function for INT2 service routine
{    
    INTCON3bits.INT2IF=0;                               // Clear the INT2 flag
    __delay_ms(5);                                      // Delay 5 ms
    INT2_flag = 1;                                      // Set software INT2_flag to 1 
} 
    
void T0_ISR()                                           // Function for Timer0 interrupt service routine   
{ 
    INTCONbits.TMR0IF=0;                                // Clear the Timer0 interrupt flag 
    T0CONbits.TMR0ON=0;                                 // Turn off Timer0 
    
    TMR0H = 0x0B;                                       // Load TMR0H with 0x0B                           
    TMR0L = 0xDB;                                       // Load TMR0L with 0xDB
    SEC_LED = ~SEC_LED;                                 // Invert SEC_LED
    
    Tach_cnt = TMR1L;                                   // Store TMR1L into Tach Count
    TMR1L = 0;                                          // Clear TMR1L
    T0CONbits.TMR0ON=1;                                 // Turn on Timer0  
} 


