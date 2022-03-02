#include <xc.h>                                                                                     // Include xc.h
#include <string.h>                                                                                 // Include string.h
#include <stdlib.h>                                                                                 // Include stdlib.h
#include <stdio.h>                                                                                  // Include stdio.h

#include "Main.h"                                                                                   // Include Main.h
#include "I2C_Support.h"                                                                            // Include I2C_Support.h
#include "Setup_Fan_Temp.h"                                                                         // Include Setup_Fan_Temp.h
#include "Main_Screen.h"                                                                            // Include Main_Screen.h
#include "TFT_ST7735.h"                                                                             // Include TFT_ST7735.h

extern char setup_fan_text[];                                                                       // Text storage for fan setup temperature
extern int INT0_flag, INT1_flag, INT2_flag;                                                         // Variables for interrupt flags
extern unsigned char setup_fan_temp;                                                                // Variable for fan setup temperature
extern char *txt;                                                                                   // Variable for *txt

void Setup_Fan_Temp(void)                                                                           // Function for fan setup screen
{
    char Key_Up_Flag, Key_Dn_Flag;                                                                  // Increment/decrement flags
    char Select_Field;                                                                              // Variable for which field you are on
    Select_Field = 0;                                                                               // Set Select_Field to 0
    Initialize_Setup_Fan_Screen();                                                                  // Initialize setup fan screen
    Update_Setup_Fan_Screen();                                                                      // Update the screen with latest info

    while (enter_setup == 1)                                                                        // While in setup mode,
    {
        if (INT0_flag == 1)                                                                         // If INT0 flag is 1,
        {
            INT0_flag = 0;                                                                          // Clear INT0 flag to 0
            Key_Up_Flag = 1;                                                                        // Set increment flag to 1
        }
        if (INT1_flag == 1)                                                                         // If INT1 flag is 1,
        {   
            INT1_flag = 0;                                                                          // Clear INT1 flag to 0
            Key_Dn_Flag = 1;                                                                        // Set decrement flag to 1
        }
        
        
        if (Key_Up_Flag == 1)                                                                       // If user increments,
        {
            if (setup_fan_temp < 110)                                                               // If setup_fan_temp is less than 110,
                setup_fan_temp++;                                                                   // Increment setup_fan_temp
            Update_Setup_Fan_Screen();                                                              // Update fan setup screen with latest info
            Key_Up_Flag = 0;                                                                        // Clear the increment flag to 0
        }
        if (Key_Dn_Flag == 1)                                                                       // If user decrements,
        {
            if (setup_fan_temp > 50)                                                                // If setup_fan_temp is greater than 50,
                setup_fan_temp--;                                                                   // Decrement setup_fan_temp 
            Update_Setup_Fan_Screen();                                                              // Update fan setup screen
            Key_Dn_Flag = 0;                                                                        // Clear decrement flag to 0
        }                 
    }   
    DS3231_Read_Time();                                                                             // Read time from DS3231
    Initialize_Screen();                                                                            // Initialize main screen before returning

} 
    
void Initialize_Setup_Fan_Screen(void)                                                              // Function to initialize fan setup screen
{ 
    fillScreen(ST7735_BLACK);                                                                       // Fills background of screen with color passed to it
 
    strcpy(txt, "ECE3301L Fall20 Final\0");                                                         // Write "ECE3301L Fall20 Final" header
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);                           // X and Y coordinates of where the text is to be displayed

    strcpy(txt, " Fan Setup\0");                                                                    // Write " Fan Setup" 
    drawtext(start_x , start_y+25, txt, ST7735_YELLOW, ST7735_BLACK, TS_2);                         // X and Y coordinates of where the text is to be displayed
                         
    strcpy(txt, "  Set Fan Temp");                                                                  // Write " Set Fan Temp"
    drawtext(setup_fan_x , setup_fan_y, txt, ST7735_CYAN  , ST7735_BLACK, TS_1);                    // Write setup_fan temp text
}
    
void Update_Setup_Fan_Screen(void)                                                                  // Function to update setup_fan screen
{   

    setup_fan_text[0] = setup_fan_temp / 100 + '0';                                                 // Get first digit of setup fan temperature
    setup_fan_text[1] = (setup_fan_temp / 10) % 10 + '0';                                           // Get second digit of setup fan temperature 
    setup_fan_text[2] = setup_fan_temp % 10 + '0';                                                  // Get third digit of setup fan temperature 
    drawtext(setup_data_fan_x, setup_data_fan_y ,setup_fan_text, ST7735_RED, ST7735_BLACK, TS_2);   // Write setup fan temp text
}


