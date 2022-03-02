#include <stdio.h>                                                              // Include stdio.h
#include <stdlib.h>                                                             // Include stdlib.h
#include <xc.h>                                                                 // Include xc.h
#include <math.h>                                                               // Include math.h
#include <p18f4620.h>                                                           // Include p18f4620.h
#include <usart.h>                                                              // Include usart.h
#include <string.h>                                                             // Include string.h

#include "Main.h"                                                               // Include Main.h
#include "I2C_Support.h"                                                        // Include I2C_Support.h
#include "I2C_Soft.h"                                                           // Include I2C_Soft.h
#include "TFT_ST7735.h"                                                         // Include TFT_ST7735.h
#include "Interrupt.h"                                                          // Include Interrupt.h
#include "Main_Screen.h"                                                        // Include Main_Screen.h

#pragma config OSC      =   INTIO67                                             // Configure OSC as INTIO67
#pragma config BOREN    =   OFF                                                 // Configure BOREN as OFF
#pragma config WDT      =   OFF                                                 // COnfigure WDT as OFF
#pragma config LVP      =   OFF                                                 // Configure LVP as OFF
#pragma config CCP2MX   =   PORTBE                                              // Configure CCP2MX as PORTBE

void Initialize_Screen(void);                                                   // Function to initialize LCD screen
void Update_Screen(void);                                                       // Function to update LCD screen
void Do_Init(void);                                                             // Function to initialize TRIS, ADC, OSCCON, UART
float read_volt();                                                              // Function to read voltage
int get_duty_cycle(int,int);                                                    // Function to get duty_cycle value based on set/ambient temperatures
int get_RPM();                                                                  // Function to get rpm value of fan
void Monitor_Fan();                                                             // Function to monitor fan values
void Turn_Off_Fan();                                                            // Function to turn off fan
void Turn_On_Fan();                                                             // Function to turn on fan
unsigned int get_full_ADC();                                                    // Function to perform A/D conversion
void Get_Temp(void);                                                            // Function to read temperature from DS1621
void Update_Volt(void);                                                         // Function to update volt reading
void Test_Alarm(void);                                                          // Function to manage alarm modes
void Activate_Buzzer();                                                         // Function to turn on buzzer
void Deactivate_Buzzer();                                                       // Function to turn off buzzer

void Main_Screen(void);                                                         // Function to enter main screen
void Do_Setup(void);                                                            // Function to enter setup modes
void do_update_pwm(char);                                                       // Function to update pwm
void Set_RGB_Color(int color);                                                  // Function to set RGB colors

char buffer[31]         = " ECE3301L Fall'20 L12\0";                            // Variable for LCD header
char *nbr;                                                                      // Variable for *nbr
char *txt;                                                                      // Variable for *txt
char tempC[]            = "+25";                                                // text storage for temperature celsius
char tempF[]            = "+77";                                                // text storage for temperature fahrenheit
char time[]             = "00:00:00";                                           // text storage for time
char date[]             = "00/00/00";                                           // text storage for date
char alarm_time[]       = "00:00:00";                                           // text storage for alarm time
char Alarm_SW_Txt[]     = "OFF";                                                // text storage for alarm enable
char Fan_SW_Txt[]       = "OFF";                                                // text storage for Heater Mode
char Fan_Set_Temp_Txt[] = "075F";                                               // text storage for fan set temperature
char Volt_Txt[]         = "0.00V";                                              // text storage for Volt     
char DC_Txt[]           = "000";                                                // text storage for Duty Cycle value
char RTC_ALARM_Txt[]    = "0";                                                  // text storage for RTC_ALARM
char RPM_Txt[]          = "0000";                                               // text storage for RPM

char setup_time[]       = "00:00:00";                                           // text storage for setup time
char setup_date[]       = "01/01/00";                                           // text storage for setup date
char setup_alarm_time[] = "00:00:00";                                           // text storage for alarm time
char setup_fan_text[]   = "075F";                                               // text storage for setup_fan temperature

signed int DS1621_tempC, DS1621_tempF;                                          // Variables for temperature celsius/fahrenheit

int INT0_flag, INT1_flag, INT2_flag, Tach_cnt;                                  // Variable for INT flags and Tach_cnt
int ALARMEN;                                                                    // Variable for alarm enable
int FANEN;                                                                      // Variable for fan enable
int alarm_mode, MATCHED, color;                                                 // Variables for alarm_mode, MATCHED, color
unsigned char second, minute, hour, dow, day, month, year, old_sec;             // Variables for second, minute, hour, dow, day, month, year, old_sec
unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;               // Variables for alarm second, minute, hour, date
unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;         // Variables for setup alarm second, minute, hour
unsigned char setup_second, setup_minute, setup_hour,                           // Variables for setup second, minute, hour, day, month, year
        setup_day, setup_month, setup_year;
unsigned char setup_fan_temp = 75;                                              // Variable for setup_fan_temp
float volt;                                                                     // Variable for volt from ADC
int duty_cycle;                                                                 // Variable for duty_cycle
int rpm;                                                                        // Variable for rpm of fan


int Tach_cnt = 0;                                                               // Set Tach_cnt to 0


void putch (char c)                                                             // Putch function for Tera Term
{
    while (!TRMT);                                                              // while not TRMT
    TXREG = c;                                                                  // Set TXREG equal to c
}

void init_UART()                                                                // Initialize UART
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &                            // Initialize UART for Tera Term
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x70;                                                              // Set OSCCON to 0x70, 8 MHz
}

void Init_ADC()                                                                 // Function to initialize ADC
{
    ADCON0 = 0x01;                                                              // Set ADCON0 to 0x01
    ADCON1 = 0x0E;                                                              // Set ADCON1 to 0x0E
    ADCON2 = 0xA9;                                                              // Set ADCON2 to 0xA9
}

void Init_IO()                                                                  // Function to initialize IO
{
    TRISA = 0x33;                                                               // Set TRISA to 0x33
    TRISB = 0x07;                                                               // Set TRISB to 0x07
    TRISC = 0x01;                                                               // Set TRISC to 0x01
    TRISD = 0x00;                                                               // Clear TRISD to all outputs
    TRISE = 0x00;                                                               // Clear TRISE to all outputs
}

void Do_Init()                                                                  // Function to initialize UART, OSCCON, ADC, IO, timers, DS1621, interrupts, I2C 
{ 
    init_UART();                                                                // Initialize the uart
    OSCCON = 0x70;                                                              // Set oscillator to 8 MHz 
    Init_ADC();                                                                 // Initialize ADC
    Init_IO();                                                                  // Initialize TRIS registers
    RBPU = 0;                                                                   // Set RBPU to 0
       
    TMR1L = 0x00;                                                               // Load TMR1L with 0x00                             
    T1CON = 0x03;                                                               // Set T1CON to 0x03
 
    T0CON = 0x03;                                                               // Set T0CON to 0x03                                                 
    TMR0H = 0x0B;                                                               // Load TMR0H to 0x0B                           
    TMR0L = 0xDB;                                                               // Load TMR0L to 0xDB         
    INTCONbits.TMR0IF = 0;                                                      // Clear Timer0 flag to 0       
    T0CONbits.TMR0ON = 1;                                                       // Start Timer0        
    INTCONbits.TMR0IE = 1;                                                      // Set Timer0 interrupt enable to 1       
    Init_Interrupt();                                                           // Initialize interrupts
    
    I2C_Init(100000);                                                           // Initialize I2C         
    DS1621_Init();                                                              // Initialize DS1621
} 

void main()                                                                     // Start of main program
{
    Do_Init();                                                                  // Initialization    

    txt = buffer;                                                               // Set txt equal to buffer

    Initialize_Screen();                                                        // Initialize LCD screen

    old_sec = 0xff;                                                             // Set old second equal to 0xff
    Turn_Off_Fan();                                                             // Turn off fan
//  DS3231_Write_Initial_Alarm_Time();                                          // uncommented this line if alarm time was corrupted    
    DS3231_Read_Time();                                                         // Read time for the first time
    DS3231_Read_Alarm_Time();                                                   // Read alarm time for the first time
    DS3231_Turn_Off_Alarm();                                                    // Turn off alarm
    color = 0;                                                                  // Set color to 0 (no color)
    ALARMEN = 0;                                                                // Set ALARMEN to 0
    Deactivate_Buzzer();                                                        // Turn off buzzer

    while(TRUE)                                                                 // Infinite while loop
    { 
        if (enter_setup == 0)                                                   // If setup switch is LOW
        {
            Main_Screen();                                                      // Stay on main screen.
        }
        else                                                                    // else,
        {
            Do_Setup();                                                         // Go to setup screen.
        }
    }
}

void Main_Screen()                                                              // Function for main screen
{   
    DS3231_Read_Time();                                                         // Read time

        
    if (INT0_flag == 1)                                                         // If INT0 flag is 1
    {
        INT0_flag = 0;                                                          // Clear INT0_flag to 0 
        Turn_Off_Fan();                                                         // Turn off fan
    }
    if (INT1_flag == 1)                                                         // If INT1 flag is 1,
    {
        INT1_flag = 0;                                                          // Clear INT1_flag to 0
        Turn_On_Fan();                                                          // Turn on fan
    }
    if (INT2_flag == 1)                                                         // If INT2 flag is 1,
    {
        INT2_flag = 0;                                                          // Clear INT2_flag to 0
        ALARMEN = !ALARMEN;                                                     // Toggle ALARMEN
    }
    if (old_sec != second)                                                      // If 1 second has passed,
    {
        old_sec = second;                                                       // Set old second equal to current second
        
        volt = read_volt();                                                     // Read voltage from ADC
        Test_Alarm();                                                           // call routine to handle the alarm  function
  
        printf ("%02x:%02x:%02x %02x/%02x/%02x ",hour,minute,second,month,day,year);// Print time and date 
        printf ("duty cycle = %d  RPM = %d ", duty_cycle, rpm);                 // Print duty cycle and rpm
        Get_Temp();                                                             // Read tempC/F from DS1621
                
        if (FANEN == 1)                                                         // If FANEN is 1,
        {
            Monitor_Fan();                                                      // Call Monitor_Fan() function
        }
            
        Update_Screen();                                                        // Update screen values
    }   
}

void Do_Setup()                                                                 // Function to enter setup screens
{   
    if (setup_sel0 == 0 && setup_sel1 == 0)                                     // If setup_sel0 is 0 and setup_sel1 is 0,
    {
        Setup_Time();                                                           // Enter Setup_Time mode
    }
    else if (setup_sel0 == 1 && setup_sel1 == 0)                                // else if setup_sel0 is 1 and setup_sel1 is 0,
    {
        Setup_Alarm_Time();                                                     // Enter Setup_Alarm_Time mode
    }
    else if (setup_sel1 == 1)                                                   // else if setup_sel1 is 1,
    {
        Setup_Fan_Temp();                                                       // Enter Setup_Fan_Temp mode
    }
            

}

void Get_Temp(void)                                                             // Function to read temperature
{
    DS1621_tempC = DS1621_Read_Temp();                                          // Read temp

    if ((DS1621_tempC & 0x80) >= 0x80)                                          // If DS1621_tempC is negative,
    {
        DS1621_tempC = 0x80 - (DS1621_tempC & 0x7f);
        DS1621_tempF = 32 - DS1621_tempC * 9 /5;                                // Convert celsius into fahrenheit
        printf ("Temperature = -%dC or %dF\r\n", DS1621_tempC, DS1621_tempF);   // Print temperature C/F
        DS1621_tempC = 0x80 | DS1621_tempC;                                     // Bitwise OR DS1621_tempC with 0x80
    }
    else                                                                        // else,
    {
        DS1621_tempF = DS1621_tempC * 9 /5 + 32;                                // Convert celsius to fahrenheit
        printf ("Temperature = %dC or %dF\r\n", DS1621_tempC, DS1621_tempF);    // Print temperature C/F
    }
}

void Monitor_Fan()                                                              // Function to monitor fan
{
    duty_cycle = get_duty_cycle(DS1621_tempF,setup_fan_temp);                   // Get duty_cycle depending on difference between set/ambient temperatures
    do_update_pwm(duty_cycle);                                                  // Update pwm based on duty_cycle
    if (duty_cycle == 0)                                                        // If duty_cycle is 0,
    {
        rpm = 0;                                                                // Set rpm to 0
    }
    else                                                                        // else,
        rpm = get_RPM();                                                        // Get rpm value of fan
}

float read_volt()                                                               // Function to read ADC voltage
{
	int nStep = get_full_ADC();                                                 // Get full ADC value
    volt = nStep * 5 /1024.0;                                                   // Convert nstep to volt
    
	return (volt);                                                              // Return volt
}
    
int get_duty_cycle(int temp, int set_temp)                                      // Function to get duty_cycle 
{	
    int dc = (set_temp - temp) * 2;                                             // Calculate duty cycle based on set and ambient temperatures
    
    if (dc < 0)                                                                 // If dc < 0,
    {
        dc = 0;                                                                 // Set dc equal to 0
    }
    if (dc > 100)                                                               // else if dc > 100,
    {
        dc = 100;                                                               // Set dc equal to 100
    }
    return (dc);                                                                // Return dc value

}

int get_RPM()                                                                   // Function to get rpm value
{
    int rps = Tach_cnt;                                                         // Set rps equal to Tach_cnt
    return (rps * 60);                                                          // Convert rps to rpm
}


void Turn_Off_Fan()                                                             // Function to turn off fan
{
    duty_cycle = 0;                                                             // Clear duty_cycle to 0
    do_update_pwm(duty_cycle);                                                  // Update pwm
    rpm = 0;                                                                    // Clear rpm to 0
    FANEN = 0;                                                                  // Clear FANEN to 0
    FANEN_LED = 0;                                                              // Turn off FANEN_LED
}

void Turn_On_Fan()                                                              // Function to turn on fan
{
    FANEN = 1;                                                                  // Set FANEN to 1
    FANEN_LED = 1;                                                              // Turn on FANEN_LED
}

void do_update_pwm(char duty_cycle)                                             // Function to update pwm based on duty_cycle
{ 
    float dc_f;                                                                 // Variable dc_f
    int dc_I;                                                                   // Variable DC_I
    PR2 = 0b00000100 ;                                                          // Set the frequency for 25 Khz
    T2CON = 0b00000111 ;                                                        // Set value for T2Con
    dc_f = ( 4.0 * duty_cycle / 20.0);                                          // Calculate factor of duty cycle versus a 25 Khz signal
    dc_I = (int) dc_f;                                                          // Get the integer part
    if (dc_I > duty_cycle)                                                      // If dc_I > duty_cycle
        dc_I++;                                                                 // Round up function
    CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;                                // Set value for CCP1CON
    CCPR1L = (dc_I) >> 2;                                                       // Set value for CCPR1L
}

unsigned int get_full_ADC()                                                     // Function to perform A/D conversion
{
unsigned int result;                                                            // variable for ADC result
    ADCON0bits.GO=1;                                                            // Start Conversion
    while(ADCON0bits.DONE==1);                                                  // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                                         // combine result of upper byte and
                                                                                // lower byte into result
    return result;                                                              // Return result
}

void Activate_Buzzer()                                                          // Function to activate buzzer
{
    PR2 = 0b11111001;                                                           // Set PR2 to 11111001
    T2CON = 0b00000101;                                                         // Set T2CON to 00000101
    CCPR2L = 0b01001010;                                                        // Set CCPR2L to 01001010
    CCP2CON = 0b00111100;                                                       // Set CCP2CON to 00111100
}

void Deactivate_Buzzer()                                                        // Function to deactivate buzzer
{
    CCP2CON = 0x0;                                                              // Set CCP2CON to 0x0
    PORTBbits.RB3 = 0;                                                          // Clear RB3 to 0
}

void Test_Alarm()                                                               // Function to manage alarm modes
{
    int alarm_case;                                                             // Variable for alarm switch case
    
    if (RTC_ALARM_NOT == 0)                                                     // If RTC_ALARM_NOT is 0,
    {
        MATCHED = 1;                                                            // Set MATCHED to 1
    }
    
                                                                                // Set switch cases
    if (ALARMEN == 1 && alarm_mode == 0)                                        // If ALARMEN is 1 and alarm_mode is 0,
    {
        alarm_case = 0;                                                         // Set alarm_case to 0
    }
    else if (ALARMEN == 0 && alarm_mode == 1)                                   // else if ALARMEN is 0 and alarm_mode is 0,
    {
        alarm_case = 1;                                                         // Set alarm_case to 1
    }
    else if (ALARMEN == 1 && alarm_mode == 1)                                   // else if ALARMEN is 1 and alarm_mode is 1,
    {
        alarm_case = 2;                                                         // Set alarm_mode to 2
    }
    
    
    
    switch (alarm_case)                                                         // Switch case to determine which mode the alarm is in
    {   
        case 0:                                                                 // Case 0:
            alarm_mode = 1;                                                     // Set alarm_mode to 1
            break;                                                              // break switch case

        case 1:                                                                 // Case 1:
            alarm_mode = 0;                                                     // Set alarm_mode to 0
            Deactivate_Buzzer();                                                // Turn off buzzer
            DS3231_Turn_Off_Alarm();                                            // Turn off alarm
            MATCHED = 0;                                                        // Clear MATCHED to 0
            color = 0;                                                          // Set color to 0 (no color)
            Set_RGB_Color(color);                                               // Update RGB color
            break;                                                              // break switch case

        case 2:                                                                 // Case 2:
            if (volt > 3.5)                                                     // Check if light sensor is covered
            {
                MATCHED = 0;                                                    // Clear MATCHED to 0
                DS3231_Turn_Off_Alarm();                                        // Turn off alarm
                color = 0;                                                      // Set color to 0 (no color)
                Set_RGB_Color(color);                                           // Update RGB color
                Deactivate_Buzzer();                                            // Turn off buzzer
            }
            else if (MATCHED == 1)                                              // else if alarm time matches clock time,
            {
                DS3231_Turn_On_Alarm();                                         // Turn on alarm
                Activate_Buzzer();                                              // Turn on buzzer
                Set_RGB_Color(color);                                           // Update RGB color
                    color++;                                                    // Increment RGB color
                if (color > 7)                                                  // If color is >7, 
                    color = 0;                                                  // Reset color back to 0
            }
            else DS3231_Turn_Off_Alarm();                                       // else, turn off alarm
            break;                                                              // Break switch case

        default:                                                                // Default case:
            break;                                                              // Break switch case
    }

}

void Set_RGB_Color(int color)                                                   // Function to set RGB color
{   
    if (color == 0)                                                             // If color is 0 (no color),
    {
        RGB_RED = 0;                                                            // Set RGB_RED to 0
        RGB_GREEN = 0;                                                          // Set RGB_GREEN to 0
        RGB_BLUE = 0;                                                           // Set RGB_BLUE to 0
    }
    else if (color == 1)                                                        // else if color is 1 (red),
    {
        RGB_RED = 1;                                                            // Set RGB_RED to 1
        RGB_GREEN = 0;                                                          // Set RGB_GREEN to 0
        RGB_BLUE = 0;                                                           // Set RGB_BLUE to 0
    }
    else if (color == 2)                                                        // else if color is 2 (green),
    {
        RGB_RED = 0;                                                            // Set RGB_RED to 0
        RGB_GREEN = 1;                                                          // Set RGB_GREEN to 1
        RGB_BLUE = 0;                                                           // Set RGB_BLUE to 0
    }
    else if (color == 3)                                                        // else if color is 3 (yellow),
    {
        RGB_RED = 1;                                                            // Set RGB_RED to 1
        RGB_GREEN = 1;                                                          // Set RGB_GREEN to 1
        RGB_BLUE = 0;                                                           // Set RGB_BLUE to 0
    }
    else if (color == 4)                                                        // else if color is 4 (blue),
    {
        RGB_RED = 0;                                                            // Set RGB_RED to 0
        RGB_GREEN = 0;                                                          // Set RGB_GREEN to 0
        RGB_BLUE = 1;                                                           // Set RGB_BLUE to 1
    }
    else if (color == 5)                                                        // else if color is 5 (purple),
    {
        RGB_RED = 1;                                                            // Set RGB_RED to 1
        RGB_GREEN = 0;                                                          // Set RGB_GREEN to 0
        RGB_BLUE = 1;                                                           // Set RGB_BLUE to 1
    }
    else if (color == 6)                                                        // else if color is 6 (cyan),
    {
        RGB_RED = 0;                                                            // Set RGB_RED to 0
        RGB_GREEN = 1;                                                          // Set RGB_GREEN to 1
        RGB_BLUE = 1;                                                           // Set RGB_BLUE to 1
    }
    else if (color == 7)                                                        // else if color is 7 (white),
    {
        RGB_RED = 1;                                                            // Set RGB_RED to 1
        RGB_GREEN = 1;                                                          // Set RGB_GREEN to 1
        RGB_BLUE = 1;                                                           // Set RGB_BLUE to 1
    }
    
}
