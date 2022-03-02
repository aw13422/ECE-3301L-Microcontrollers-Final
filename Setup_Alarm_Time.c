#include <xc.h>                                                                     // Include xc.h
#include <string.h>                                                                 // Include string.h
#include <stdlib.h>                                                                 // Include stdlib.h
#include <stdio.h>                                                                  // Include stdio.h
    
#include "I2C_Support.h"                                                            // Include I2C_Support.h
#include "Setup_Alarm_Time.h"                                                       // Include Setup_Alarm_Time.h
#include "Main_Screen.h"                                                            // Include Main_Screen.h
#include "Main.h"                                                                   // Include Main.h
#include "Setup_Time.h"                                                             // Include Setup_time.h
#include "TFT_ST7735.h"                                                             // Include TFT_ST7735.h

extern char setup_alarm_time[];                                                     // Text storage for setup alarm time
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;            // Variables for alarm time
extern unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;      // Variables for setup alarm time
extern int INT0_flag, INT1_flag, INT2_flag;                                         // Variables for interrupt flags
extern char *txt;                                                                   // Variable for *txt

void Setup_Alarm_Time(void)                                                         // Function to setup alarm time
{
    char Key_Next_Flag, Key_Up_Flag, Key_Dn_Flag;                                   // Variables for increment/decrement/next flags
    char Select_Field;                                                              // Variable for field select
    Select_Field = 0;                                                               // Set Select_Field to 0
    DS3231_Read_Alarm_Time();                                                       // Read alarm time
    alarm_second = bcd_2_dec(alarm_second);                                         // Convert second to dec for use
    alarm_minute = bcd_2_dec(alarm_minute);                                         // Convert minute to dec for use
    alarm_hour   = bcd_2_dec(alarm_hour);                                           // Convert hour to dec for use
    setup_alarm_second = alarm_second;                                              // Set setup alarm second as current set alarm second
    setup_alarm_minute = alarm_minute;                                              // Set setup alarm minute as current set alarm minute
    setup_alarm_hour = alarm_hour;                                                  // Set setup alarm hour as current set alarm hour
    Initialize_Setup_Alarm_Time_Screen();                                           // Initialize the alarm setup screen
    Update_Setup_Alarm_Time_Screen();                                               // Update the alarm screen with latest info

    while (enter_setup == 1)                                                        // While in setup mode,
    {   
        if (INT0_flag == 1)                                                         // If software INT0 flag is set, 
        { 
            INT0_flag = 0;                                                          // Clear INT0 flag
            Key_Up_Flag = 1;                                                        // Set increment flag
        }
        if (INT1_flag == 1)                                                         // If software INT1 flag is set, 
        { 
            INT1_flag = 0;                                                          // Clear INT1 flag
            Key_Dn_Flag = 1;                                                        // Set decrement flag
        }        
        if (INT2_flag == 1)                                                         // If software INT2 flag is set,
        { 
            INT2_flag = 0;                                                          // Clear INT2 flag
            Key_Next_Flag = 1;                                                      // Set next flag
        }  

        if (Key_Up_Flag == 1)                                                       // If user increments,
        {
            switch (Select_Field)                                                   // Increment the proper field
            {
                case 0:                                                             // Case 0:
                    setup_alarm_hour++;                                             // Increment setup_alarm_hour
                    if (setup_alarm_hour == 24) setup_alarm_hour = 0;               // If setup_alarm_hour is 24, set equal to 0
                    break;                                                          // Break

                case 1:                                                             // Case 1:
                    setup_alarm_minute++;                                           // Increment setup_alarm_minute
                    if (setup_alarm_minute == 60) setup_alarm_minute = 0;           // If setup_alarm_minute is 60, set equal to 0
                    break;                                                          // Break

                case 2:                                                             // Case 2:
                    setup_alarm_second++;                                           // Increment setup_alarm_second
                    if (setup_alarm_second == 60) setup_alarm_second = 0;           // If setup_alarm_second is 60, set equal to 0
                    break;                                                          // Break

                default:                                                            // Default case:
                    break;                                                          // Break
            }    
            Update_Setup_Alarm_Time_Screen();                                       // Update screen with latest info
            Key_Up_Flag = 0;                                                        // Clear the increment flag
        }

        if (Key_Dn_Flag == 1)                                                       // If user decrements,
        {
            switch (Select_Field)                                                   // Decrement the proper field
            {
                case 0:                                                             // Case 0:
                    if (setup_alarm_hour == 0) setup_alarm_hour = 23;               // If setup_alarm_hour is 0, set equal to 23
                    else --setup_alarm_hour;                                        // else decrement setup_alarm_hour
                    break;                                                          // Break

                case 1:                                                             // Case 1:
                    if (setup_alarm_minute == 0) setup_alarm_minute = 59;           // If setup_alarm_minute is 0, set equal to 59
                    else --setup_alarm_minute;                                      // else decrement setup_alarm_minute
                    break;                                                          // Break

                case 2:                                                             // Case 2:
                    if (setup_alarm_second == 0) setup_alarm_second = 59;           // If setup_alarm_second is 0, set equal to 59
                    else --setup_alarm_second;                                      // else decrement setup_alarm_second
                    break;                                                          // Break

                default:                                                            // Default case:
                    break;                                                          // Break
            }                
            Update_Setup_Alarm_Time_Screen();                                       // Update screen with latest info
            Key_Dn_Flag = 0;                                                        // Clear the decrement flag
        } 

        if (Key_Next_Flag == 1 )                                                    // If user switches to next field,
        {        
            Select_Field++;                                                         // Move to next field
            if (Select_Field == 3) Select_Field = 0;                                // Wrap around if necessary
            Update_Setup_Screen_Cursor(Select_Field);                               // Update cursor
            Key_Next_Flag = 0;                                                      // Clear flag
        }
    }         
    DS3231_Write_Alarm_Time();                                                      // Write alarm time
    DS3231_Read_Alarm_Time();                                                       // Read alarm time
    DS3231_Read_Time();                                                             // Read current time
    Initialize_Screen();                                                            // Initialize main screen before returning
}
     
void Initialize_Setup_Alarm_Time_Screen(void)                                       // Function to initialize setup alarm time screen
{ 
    fillScreen(ST7735_BLACK);                                                       // Fills background of screen with color passed to it
 
    strcpy(txt, "ECE3301L Fall20 Final\0");                                         // Write "ECE3301L Fall20 Final" header
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);           // X and Y coordinates of where the text is to be displayed
                                                                

    strcpy(txt, "Alrm Setup");                                                      // Write "Alrm Setup"
    drawtext(start_x+5 , start_y+20, txt, ST7735_MAGENTA, ST7735_BLACK, TS_2);      // X and Y coordinates of where the text is to be displayed
    
    strcpy(txt, "Time");                                                            // Write "Time"
    drawtext(time_x  , time_y , txt, ST7735_BLUE   , ST7735_BLACK, TS_1);           // Write time text
    fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_CYAN);                      // Write time data text
}

void Update_Setup_Alarm_Time_Screen(void)                                           // Function to update setup alarm time screen
{
    setup_alarm_time[0]  = (setup_alarm_hour / 10) + '0';                           // Set first digit of setup_alarm_hour
    setup_alarm_time[1]  = (setup_alarm_hour % 10) + '0';                           // Set second digit of setup_alarm_hour
    setup_alarm_time[3]  = (setup_alarm_minute / 10) + '0';                         // Set first digit of setup_alarm_minute
    setup_alarm_time[4]  = (setup_alarm_minute % 10) + '0';                         // Set second digit of setup_alarm_minute
    setup_alarm_time[6]  = (setup_alarm_second / 10) + '0';                         // Set first digit of setup_alarm_second
    setup_alarm_time[7]  = (setup_alarm_second % 10) + '0';                         // Set second digit of setup_alarm_second
    drawtext(data_time_x, data_time_y, setup_alarm_time,                            // Write setup_alarm_time text
            ST7735_CYAN, ST7735_BLACK, TS_2);
}
 

