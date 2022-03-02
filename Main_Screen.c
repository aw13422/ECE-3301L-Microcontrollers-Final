#include <xc.h>                                                                                             // Include xc.h
#include <string.h>                                                                                         // Include string.h
#include <stdlib.h>                                                                                         // Include stdlib.h
#include <stdio.h>                                                                                          // Include stdio.h
#include "Main.h"                                                                                           // Include Main.h
#include "Main_Screen.h"                                                                                    // Include Main_Screen.h
#include "TFT_ST7735.h"                                                                                     // Include TFT_ST7735.h

extern char buffer[31];                                                                                     // Text storage for buffer
extern char *nbr;                                                                                           // Variable for *nbr
extern char *txt;                                                                                           // Variable for *txt
extern char tempC[];                                                                                        // Text storage for temperature C
extern char tempF[];                                                                                        // Text storage for temperature F
extern char time[];                                                                                         // Text storage for time field
extern char date[];                                                                                         // Text storage for date field
extern char alarm_time[];                                                                                   // Text storage for alarm time
extern char Alarm_SW_Txt[];                                                                                 // Text storage for Alarm SW field
extern char Fan_Set_Temp_Txt[];                                                                             // Text storage for Fan_Set_Temp field
extern char Fan_SW_Txt[];                                                                                   // Text storage for Fan SW field
extern char Volt_Txt[];                                                                                     // Text storage for Volt field
extern char DC_Txt[];                                                                                       // Text storage for DC field
extern char RPM_Txt[];                                                                                      // Text storage for RPM field
extern char RTC_ALARM_Txt[];                                                                                // Text storage for RM field
extern signed int DS1621_tempC, DS1621_tempF;                                                               // Variables for temperature C/F
extern unsigned char second, minute, hour, dow, day, month, year;                                           // Variables for clock time
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;                                    // Variables for alarm time/date
extern unsigned char setup_fan_temp;                                                                        // Variable for setup fan temperature
extern int duty_cycle;
extern int ALARMEN;                                                                                         // Variable for alarm enable
extern int FANEN;                                                                                           // Variable for fan enable
extern int rpm;                                                                                             // Variable to store rpm of fan
extern float volt;                                                                                          // Variable to store voltage 

void Initialize_Screen(void)                                                                                // Function to initialize LCD screen
{ 
    LCD_Reset();                                                                                            // Screen reset
    TFT_GreenTab_Initialize();                                                                              // Initialize TFT
    fillScreen(ST7735_BLACK);                                                                               // Fills background of screen with color passed to it
 
    strcpy(txt, "ECE3301L Fall20 Final\0");                                                                 // Write ECE3301L Fall20 Final header 
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);                                   // Starting point of text
                                                                                                            // X and Y coordinates of where the text is to be displayed
    strcpy(txt, "Temperature:");                                                                            // Write "Temperature"
    drawtext(temp_x  , temp_y , txt, ST7735_MAGENTA, ST7735_BLACK, TS_1);                                   // Write temperature text                                                                  // including text color and the background of it
    drawCircle(circ_x, circ_y , 2  , ST7735_YELLOW);                                                        // Draw circle
    strcpy(txt, "C/");                                                                                      // Write "C/"
    drawtext(tempc_x , tempc_y, txt, ST7735_YELLOW , ST7735_BLACK, TS_2);                                   // Write temperature C text
    strcpy(txt, "F");                                                                                       // Write "F"
    drawCircle(cirf_x, cirf_y , 2  , ST7735_YELLOW);                                                        // Draw circle
    drawtext(tempf_x , tempf_y, txt, ST7735_YELLOW , ST7735_BLACK, TS_2);                                   // Write temperature F text
    strcpy(txt, "Time");                                                                                    // Write "Time"
    drawtext(time_x  , time_y , txt, ST7735_BLUE   , ST7735_BLACK, TS_1);                                   // Write time text
    strcpy(txt, "Date");                                                                                    // Write "Date"
    drawtext(date_x  , date_y , txt, ST7735_RED    , ST7735_BLACK, TS_1);                                   // Write date text
    strcpy(txt, "Alarm Time");                                                                              // Write "Alarm Time"
    drawtext(alarm_time_x  , alarm_time_y , txt, ST7735_YELLOW, ST7735_BLACK, TS_1);                        // Write alarm time text
    strcpy(txt, "Alarm SW");                                                                                // Write "Alarm SW"
    drawtext(alarm_sw_x, alarm_sw_y, txt, ST7735_YELLOW, ST7735_BLACK, TS_1);                               // Write alarm sw text
    strcpy(txt, "FAN Set Temp");                                                                            // Write "FAN Set Temp"
    drawtext(fan_set_temp_x, fan_set_temp_y, txt, ST7735_BLUE  , ST7735_BLACK, TS_1);                       // Write fan temperature text
    strcpy(txt, "Fan SW");                                                                                  // Write "Fan SW"
    drawtext(fan_sw_x, fan_sw_y, txt, ST7735_BLUE  , ST7735_BLACK, TS_1);                                   // Write fan sw text
    strcpy(txt, "DC");                                                                                      // Write "DC"
    drawtext(dc_x, dc_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);                                          // Write dc text
    strcpy(txt, "RM");                                                                                      // Write "RM"
    drawtext(rtc_x, rtc_y, txt, ST7735_WHITE       , ST7735_BLACK  , TS_1);                                 // Write rtc text
    strcpy(txt, "Volt");                                                                                    // Write "Volt"
    drawtext(volt_x, volt_y, txt, ST7735_WHITE       , ST7735_BLACK  , TS_1);                               // Write volt text
    strcpy(txt, "RPM");                                                                                     // Write "RPM"
    drawtext(rpm_x, rpm_y, txt, ST7735_WHITE       , ST7735_BLACK  , TS_1);                                 // Write rpm text
                
}
void Update_Screen(void)                                                                                    // Function to update LCD screen
{
    if ((DS1621_tempC & 0x80) == 0x80)                                                                      // If tempC is negative,
    {
        tempC[0] = '-';                                                                                     // Show - for negative tempC
        tempC[1]  = (DS1621_tempC & 0x7f)/10  + '0';                                                        // Tens digit of tempC
        tempC[2]  = (DS1621_tempC & 0x7f)%10  + '0';                                                        // Ones digit of tempC
    }
    else                                                                                                    // else,
    {
        tempC[0] = '+';                                                                                     // Show + for positive tempC
        tempC[1]  = DS1621_tempC/10  + '0';                                                                 // Tens digit of tempC
        tempC[2]  = DS1621_tempC%10  + '0';                                                                 // Ones digit of tempC
    }
    
    if (DS1621_tempF < 0)                                                                                   // If tempF is negative,
    {
        tempF[0] = '-';
        int neg_tempF = -DS1621_tempF;                                                                      // Store negative tempF in variable
        tempF[1]  = neg_tempF/10  + '0';                                                                    // Tens digit of tempF
        tempF[2]  = neg_tempF%10  + '0';                                                                    // Ones digit of tempF
    }
    else                                                                                                    // else,
    {
        tempF[0] = '+';                                                                                     // Show + for positive tempF
        tempF[1]  = DS1621_tempF/10  + '0';                                                                 // Tens digit of tempF
        tempF[2]  = DS1621_tempF%10  + '0';                                                                 // Ones digit of tempF
    }    
    time[0]  = (hour >> 4)  + '0';                                                                          // Get hour MSD
    time[1]  = (hour & 0x0F) + '0';                                                                         // Get hour LSD
    time[3]  = (minute >> 4) + '0';                                                                         // Get minute MSD
    time[4]  = (minute & 0x0F) + '0';                                                                       // Get minute LSD
    time[6]  = (second >> 4) + '0';                                                                         // Get seconds MSD
    time[7]  = (second & 0x0F) + '0';                                                                       // Get seconds LSD
    
    date[0]  = (month >> 4) + '0';                                                                          // Get month MSD
    date[1]  = (month & 0x0F) + '0';                                                                        // Get month LSD
    date[3]  = (day >> 4) + '0';                                                                            // Get day MSD
    date[4]  = (day & 0x0F) + '0';                                                                          // Get day LSD
    date[6]  = (year >> 4) + '0';                                                                           // Get year MSD
    date[7]  = (year & 0x0F) + '0';                                                                         // Get year LSD
    
    alarm_time[0]  = (alarm_hour >> 4)  + '0';                                                              // Alarm Hour MSD
    alarm_time[1]  = (alarm_hour & 0x0F) + '0';                                                             // Alarm hour LSD
    alarm_time[3]  = (alarm_minute >> 4) + '0';                                                             // Alarm minute MSD
    alarm_time[4]  = (alarm_minute & 0x0F) + '0';                                                           // Alarm minute LSD
    alarm_time[6]  = (alarm_second >> 4) + '0';                                                             // Alarm second MSD
    alarm_time[7]  = (alarm_second & 0x0F) + '0';                                                           // Alarm second LSD
    
    if (ALARMEN == 1)                                                                                       // If ALARMEN is 1, 
    {       
        strcpy(Alarm_SW_Txt, "ON ");                                                                        // Show alarm sw on
    }
    else                                                                                                    // else,
    {
        strcpy(Alarm_SW_Txt, "OFF");                                                                        // Show alarm sw off
    }
      
    Fan_Set_Temp_Txt[0] = setup_fan_temp / 100 + '0';                                                       // Set first digit of Fan_Set_Temp
    Fan_Set_Temp_Txt[1] = (setup_fan_temp / 10) % 10 + '0';                                                 // Set second digit of Fan_Set_Temp
    Fan_Set_Temp_Txt[2] = setup_fan_temp % 10 + '0';                                                        // Set third digit of Fan_Set_Temp
    
    if (FANEN == 1)                                                                                         // If FANEN is 1,
    {
        strcpy(Fan_SW_Txt, "ON ");                                                                          // Show fan on
    }
    else                                                                                                    // else,
    {
        strcpy(Fan_SW_Txt, "OFF");                                                                          // Show fan off
    }                       
    
    if (RTC_ALARM_NOT == 0) RTC_ALARM_Txt[0] = '1';                                                         // If RTC_ALARM_NOT is 0, set RM field to 1
    else RTC_ALARM_Txt[0] = '0';                                                                            // else set RM field to 0
       
    Volt_Txt[0]= ((int) volt) + '0';                                                                        // Set first digit of volt
    Volt_Txt[2]= ((int)(volt * 10 )) % 10 + '0';                                                            // Set second digit of volt
    Volt_Txt[3]= ((int)(volt * 100 )) % 10 + '0';                                                           // Set third digit of volt

    DC_Txt[0] = duty_cycle / 100 + '0';                                                                     // Set first digit of duty_cycle
    DC_Txt[1] = (duty_cycle / 10) % 10 + '0';                                                               // Set second digit of duty_cycle
    DC_Txt[2] = duty_cycle % 10 + '0';                                                                      // Set third digit of duty_cycle
    
    RPM_Txt[0] = rpm / 1000  + '0';                                                                         // Set first digit of rpm
    RPM_Txt[1] = (rpm / 100) % 10 + '0';                                                                    // Set second digit of rpm
    RPM_Txt[2] = (rpm / 10) % 10 + '0';                                                                     // Set third digit of rpm
    RPM_Txt[3] = rpm % 10 + '0';                                                                            // Set fourth digit of rpm   
    
    drawtext(data_tempc_x, data_tempc_y, tempC , ST7735_YELLOW , ST7735_BLACK , TS_2);                      // Write tempC text
    drawtext(data_tempf_x, data_tempf_y, tempF , ST7735_YELLOW , ST7735_BLACK , TS_2);                      // Write tempF text
    drawtext(data_time_x , data_time_y , time  , ST7735_CYAN   , ST7735_BLACK , TS_2);                      // Write time text
    drawtext(data_date_x , data_date_y , date  , ST7735_GREEN  , ST7735_BLACK , TS_2);                      // Write date text
    drawtext(data_alarm_time_x , data_alarm_time_y , alarm_time  , ST7735_CYAN   , ST7735_BLACK , TS_1);    // Write alarm time text
    drawtext(data_alarm_sw_x , data_alarm_sw_y , Alarm_SW_Txt  , ST7735_CYAN   , ST7735_BLACK , TS_1);      // Write alarm sw text
    drawtext(data_fan_set_temp_x, data_fan_set_temp_y ,Fan_Set_Temp_Txt, ST7735_RED, ST7735_BLACK, TS_1);   // Write fan set temp text
    drawtext(data_fan_sw_x, data_fan_sw_y, Fan_SW_Txt, ST7735_RED    , ST7735_BLACK , TS_1);                // Write fan sw text
    drawtext(data_dc_x, data_dc_y, DC_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);                               // Write dc text
    drawtext(data_rtc_x, data_rtc_y, RTC_ALARM_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);                      // Write RTC text
    drawtext(data_volt_x, data_volt_y, Volt_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);                         // Write volt text
    drawtext(data_rpm_x, data_rpm_y, RPM_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);                            // Write rpm text
}



