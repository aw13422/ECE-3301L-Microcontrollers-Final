#include <xc.h>                                                                                     // Include xc.h
#include <string.h>                                                                                 // Include string.h
#include <stdlib.h>                                                                                 // Include stdlib.h
#include <stdio.h>                                                                                  // Include stdio.h

#include "TFT_ST7735.h"                                                                             // Include TFT_ST7735.h
#include "I2C_Support.h"                                                                            // Include I2C_Support.h
#include "Setup_Time.h"                                                                             // Include Setup_Time.h
#include "Main_Screen.h"                                                                            // Include Main_Screen.h
#include "Main.h"                                                                                   // Include Main.h

extern unsigned char second, minute, hour, dow, day, month, year;                                   // Variables for clock time
extern unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;    // Variables for setup time

extern int INT0_flag, INT1_flag, INT2_flag;                                                         // Variables for interrupt flags

extern char setup_time[];                                                                           // Text storage for setup_time
extern char setup_date[];                                                                           // Text storage for setup_date
    
extern char buffer[31];                                                                             // Text storage for buffer
extern char *nbr;                                                                                   // Variable for *nbr
extern char *txt;                                                                                   // Variable for *txt    
extern char tempC[];                                                                                // Text storage for tempC
extern char tempF[];                                                                                // Text storage for tempF
extern char time[];                                                                                 // Text storage for time
extern char date[];                                                                                 // Text storage for date

void Setup_Time(void)                                                                               // Function for setup time
{
    char Key_Next_Flag, Key_Up_Flag, Key_Dn_Flag;                                                   // Variables for increment/decremnt/next flags
    char Select_Field;                                                                              // Variable to select which field you are on
    Select_Field = 0;                                                                               // Set Select_Field to 0
    DS3231_Read_Time();                                                                             // Read time from DS3231
    setup_second = bcd_2_dec(second);                                                               // Set setup second as current second
    setup_minute = bcd_2_dec(minute);                                                               // Set setup minute as current minute
    setup_hour = bcd_2_dec(hour);                                                                   // Set setup hour as current hour
    setup_day = bcd_2_dec(day);                                                                     // Set setup day as current day
    setup_month = bcd_2_dec(month);                                                                 // Set setup month as current month
    setup_year = bcd_2_dec(year);                                                                   // Set setup year as current year
    Initialize_Setup_Time_Screen();                                                                 // Initialize setup screen
    Update_Setup_Time_Screen();                                                                     // Update screen with latest information
    
    while (enter_setup == 1)                                                                        // While in setup mode,
    {   
        if (INT0_flag == 1)                                                                         // If software INT0 flag is set, 
        { 
            INT0_flag = 0;                                                                          // Clear INT0 flag
            Key_Up_Flag = 1;                                                                        // Set increment flag
        }
        if (INT1_flag == 1)                                                                         // If software INT1 flag is set, 
        { 
            INT1_flag = 0;                                                                          // Clear INT1 flag
            Key_Dn_Flag = 1;                                                                        // Set decrement flag
        }        
        if (INT2_flag == 1)                                                                         // If software INT2 flag is set, 
        { 
            INT2_flag = 0;                                                                          // Clear INT2 flag
            Key_Next_Flag = 1;                                                                      // Set next flag
        }  

        if (Key_Up_Flag == 1)                                                                       // If user increments,
        {
            switch (Select_Field)                                                                   // Increment the proper field
            {
                case 0:                                                                             // Case 0:
                    setup_hour++;                                                                   // Increment setup_hour
                    if (setup_hour == 24) setup_hour = 0;                                           // If setup_hour is 24, set equal to 0
                    break;                                                                          // Break 

                case 1:                                                                             // Case 1:
                    setup_minute++;                                                                 // Increment setup_minute 
                    if (setup_minute == 60) setup_minute = 0;                                       // If setup_minute is 60, set equal to 0
                    break;                                                                          // Break

                case 2:                                                                             // Case 2:
                    setup_second++;                                                                 // Increment setup_second
                    if (setup_second == 60) setup_second = 0;                                       // If setup_second is 60, set equal to 0
                    break;                                                                          // Break

                case 3:                                                                             // Case 3:
                    setup_month++;                                                                  // Increment setup_month
                    if (setup_month == 13) setup_month = 1;                                         // If setup_month is 13, set equal to 1
                    break;                                                                          // Break

                case 4:                                                                             // Case 4:
                    setup_day++;                                                                    // Increment setup_day
                    if (setup_day == 32) setup_day = 1;                                             // If setup_day is 32, set equal to 1
                    break;                                                                          // Break

                case 5:                                                                             // Case 5:
                    setup_year++;                                                                   // Increment setup_year
                    if (setup_year == 100) setup_year = 0;                                          // If setup_year is 100, set equal to 0
                    break;                                                                          // Break

                default:                                                                            // Default case:    
                    break;                                                                          // Break
            }    
            Update_Setup_Time_Screen();                                                             // Update screen with latest info
            Key_Up_Flag = 0;                                                                        // Clear the increment flag
        }

        if (Key_Dn_Flag == 1 )                                                                      // If user decrements,
        {
            switch (Select_Field)                                                                   // Decrement the proper field
            {
                case 0:                                                                             // Case 0: 
                    if (setup_hour == 0) setup_hour = 23;                                           // If setup_hour is 0, set equal to 23
                    else --setup_hour;                                                              // else decrement setup_hour
                    break;                                                                          // Break

                case 1:                                                                             // Case 1:
                    if (setup_minute == 0) setup_minute = 59;                                       // If setup_minute is 0, set equal to 59
                    else --setup_minute;                                                            // else decrement setup_minute
                    break;                                                                          // Break

                case 2:                                                                             // Case 2: 
                    if (setup_second == 0) setup_second = 59;                                       // If setup_second is 0, set equal to 59
                    else --setup_second;                                                            // else decrement setup_second
                    break;                                                                          // Break

                case 3:                                                                             // Case 3:
                    if (setup_month == 1) setup_month = 12;                                         // If setup_month is 1, set equal to 12
                    else --setup_month;                                                             // else decrement setup_month 
                    break;                                                                          // Break

                case 4:                                                                             // Case 4:
                    if (setup_day == 1) setup_day = 31;                                             // If setup_day is 1, set equal to 31   
                    else --setup_day;                                                               // else decrement setup_day
                    break;                                                                          // Break

                case 5:                                                                             // Case 5:
                    if (setup_year == 0) setup_year = 99;                                           // If setup_year is 0, set equal to 99
                    else --setup_year;                                                              // else decrement setup_year
                    break;                                                                          // Break

                default:                                                                            // Default case:
                break;                                                                              // Break
            }                
            Update_Setup_Time_Screen();                                                             // Update screen with latest info
            Key_Dn_Flag = 0;                                                                        // Clear the decrement flag
        } 

        if (Key_Next_Flag == 1 )                                                                    // If user switches to next field,
        {        
            Select_Field++;                                                                         // Move to next field
            if (Select_Field == 6) Select_Field = 0;                                                // Wrap around if necessary
            Update_Setup_Screen_Cursor(Select_Field);                                               // Update cursor
            Key_Next_Flag = 0;                                                                      // Clear Next flag
        }    
    }         

    DS3231_Write_Time();                                                                            // Write time
    DS3231_Read_Time();                                                                             // Read time
    Initialize_Screen();                                                                            // Initialize the screen before returning     
}
    
void Initialize_Setup_Time_Screen(void)                                                             // Function to initialize setup_time screen
{ 
    fillScreen(ST7735_BLACK);                                                                       // Fills background of screen with color passed to it
 
    strcpy(txt, "ECE3301L Fall20 Final\0");                                                         // Write "ECE3301L Fall20 Final" header 
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);                           // X and Y coordinates of where the text is to be displayed
                                                        

    strcpy(txt, "Time Setup\0");                                                                    // Write "Time Setup" 
    drawtext(start_x+3 , start_y+15, txt, ST7735_MAGENTA, ST7735_BLACK, TS_2);                      // X and Y coordinates of where the text is to be displayed
       
    strcpy(txt, "Time");                                                                            // Write "Time"
    drawtext(time_x  , time_y , txt, ST7735_BLUE   , ST7735_BLACK, TS_1);                           // X and Y coordinates of where the text is to be displayed
    
    fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_CYAN);                                      // Write time text
    strcpy(txt, "Date");                                                                            // Write "Date"
    drawtext(date_x  , date_y , txt, ST7735_RED    , ST7735_BLACK, TS_1);                           // X and Y coordinates of where the text is to be displayed
}

void Update_Setup_Time_Screen(void)                                                                 // Function to update setup time screen
{
    setup_time[0]  = (setup_hour / 10)  + '0';                                                      // Set first digit of setup hour
    setup_time[1]  = (setup_hour % 10) + '0';                                                       // Set second digit of setup hour
    setup_time[3]  = (setup_minute / 10)  + '0';                                                    // Set first digit of setup minute
    setup_time[4]  = (setup_minute % 10)  + '0';                                                    // Set second digit of setup minute
    setup_time[6]  = (setup_second / 10)  + '0';                                                    // Set first digit of setup second
    setup_time[7]  = (setup_second % 10)  + '0';                                                    // Set second digit of setup second
    
    setup_date[0]  = (setup_month / 10)  + '0';                                                     // Set first digit of setup month
    setup_date[1]  = (setup_month % 10)  + '0';                                                     // Set second digit of setup month
    setup_date[3]  = (setup_day / 10)  + '0';                                                       // Set first digit of setup day
    setup_date[4]  = (setup_day % 10)  + '0';                                                       // Set second digit of setup day
    setup_date[6]  = (setup_year / 10)  + '0';                                                      // Set first digit of setup year
    setup_date[7]  = (setup_year % 10)  + '0';                                                      // Set second digit of setup year

    drawtext(data_time_x, data_time_y, setup_time, ST7735_CYAN, ST7735_BLACK, TS_2);                // Write time text
    drawtext(data_date_x, data_date_y, setup_date, ST7735_GREEN, ST7735_BLACK, TS_2);               // WRite date text
}

void Update_Setup_Screen_Cursor(char cursor_position)                                               // Function to update screen cursor
{
    char xinc = 36;                                                                                 // Variable for x coordinate
    char yinc = 30;                                                                                 // Variable for y coordinate
    switch (cursor_position)                                                                        // Switch case depending on cursor position
    {
        case 0:                                                                                     // Case 0:
            fillRect(data_time_x-1+2*xinc, data_time_y+16+yinc, 25,2,ST7735_BLACK);                 // Clear old cursor positions
            fillRect(data_time_x-1+2*xinc, data_time_y+16, 25,2,ST7735_BLACK);                      // Clear old cursor positions
            fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_CYAN);                              // Draw cursor in new position
            break;                                                                                  // Break
                
        case 1:                                                                                     // Case 1:
            fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_BLACK);                             // Clear old cursor position
            fillRect(data_time_x-1+xinc, data_time_y+16, 25,2,ST7735_CYAN);                         // Draw cursor in new position
            break;                                                                                  // Break
            
        case 2:                                                                                     // Case 2:
            fillRect(data_time_x-1+xinc, data_time_y+16, 25,2,ST7735_BLACK);                        // Clear old cursor position
            fillRect(data_time_x-1+2*xinc, data_time_y+16, 25,2,ST7735_CYAN);                       // Draw cursor in new position
            break;                                                                                  // Break
            
        case 3:                                                                                     // Case 3:
            fillRect(data_time_x-1+2*xinc, data_time_y+16, 25,2,ST7735_BLACK);                      // Clear old cursor position
            fillRect(data_time_x-1, data_time_y+16+yinc, 25,2,ST7735_CYAN);                         // Draw cursor in new position
            break;                                                                                  // Break
            
        case 4:                                                                                     // Case 4:
            fillRect(data_time_x-1, data_time_y+16+yinc, 25,2,ST7735_BLACK);                        // Clear old cursor position
            fillRect(data_time_x-1+xinc, data_time_y+16+yinc, 25,2,ST7735_CYAN);                    // Draw cursor in new position
            break;                                                                                  // Break

        case 5:                                                                                     // Case 5:
            fillRect(data_time_x-1+xinc, data_time_y+16+yinc, 25,2,ST7735_BLACK);                   // Clear old cursor position
            fillRect(data_time_x-1+2*xinc, data_time_y+16+yinc, 25,2,ST7735_CYAN);                  // Draw cursor in new position
            break;                                                                                  // Break
    }
}





