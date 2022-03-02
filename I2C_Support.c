#include <stdio.h>                                                                                  // Include stdio.h
#include "Main.h"                                                                                   // Include Main.h
#include "I2C_Support.h"                                                                            // Include I2C_Support.h
#include <p18f4620.h>                                                                               // Include p18f4620.h
#include "I2C_Soft.h"                                                                               // Include I2C_Soft.h

extern unsigned char second, minute, hour, dow, day, month, year;                                   // Variables for clock time
extern unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;    // Variables for setup time 
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;                            // Variables for alarm time
extern unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;                      // Variables for setup alarm time


void DS1621_Init()                                                                                  // Function to initialize DS1621
{       
char Device = 0x48;                                                                                 // Select 0x48 as Device
    I2C_Write_Cmd_Write_Data (Device, ACCESS_CFG, CONT_CONV);                                       // Write data to DS1621
    I2C_Write_Cmd_Only(Device, START_CONV);                                                         // Write Cmd to DS1621
}

int DS1621_Read_Temp()                                                                              // Function to read temperature
{
char Device = 0x48;                                                                                 // Select 0x48 for Device
char Cmd = READ_TEMP;                                                                               // Set Cmd equal to READ_TEMP
char Data_Ret;                                                                                      // Variable for Data_Ret
  I2C_Start();                                                                                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);                                                                     // Device address
  I2C_Write(Cmd);                                                                                   // Send register address
  I2C_ReStart();                                                                                    // Restart I2C
  I2C_Write((Device << 1) | 1);                                                                     // Initialize data read
  Data_Ret = I2C_Read(NAK);                                                                         // Read data from I2C, store in Data_Ret
  I2C_Stop();                                                                                       // Stop I2C protocol
  return Data_Ret;                                                                                  // Return Data_Ret

}

void DS3231_Read_Time()                                                                             // Function to read time
{                               
char Device = 0x68;                                                                                 // Select 0x68 as Device
char Address = 0x00;                                                                                // Select 0x00 as Address
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address
    I2C_Write(Address);                                                                             // Send register address
    I2C_ReStart();                                                                                  // Restart I2C
    I2C_Write((Device << 1) | 1);                                                                   // Initialize data read
    second = I2C_Read(ACK);                                                                         // Read second from DS3231
    minute = I2C_Read(ACK);                                                                         // Read minute from DS3231
    hour = I2C_Read(ACK);                                                                           // Read hour from DS3231
    dow = I2C_Read(ACK);                                                                            // Read day of week from DS3231
    day = I2C_Read(ACK);                                                                            // Read day from DS3231
    month = I2C_Read(ACK);                                                                          // Read month from DS3231
    year = I2C_Read(NAK);                                                                           // Read year from DS3231
    I2C_Stop();                                                                                     // Stop I2C protocol
}

void DS3231_Write_Time()                                                                            // Function to write time
{
    char Device = 0x68;                                                                             // Device ID
    char Address = 0x00;                                                                            // Beginning Address 0
    second = dec_2_bcd(setup_second);                                                               // Convert setup_second to bcd 
    minute = dec_2_bcd(setup_minute);                                                               // Convert setup_minute to bcd 
    hour = dec_2_bcd(setup_hour);                                                                   // Convert setup_hour to bcd 
    dow = 0x01;                                                                                     // Set dow to 0x01 
    day = dec_2_bcd(setup_day);                                                                     // Convert setup_day to bcd 
    month = dec_2_bcd(setup_month);                                                                 // Convert setup_month to bcd 
    year = dec_2_bcd(setup_year);                                                                   // Convert setup_year to bcd 
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address);                                                                             // Send register address
    I2C_Write(second);                                                                              // Write seconds
    I2C_Write(minute);                                                                              // Write minutes
    I2C_Write(hour);                                                                                // Write hours
    I2C_Write(dow);                                                                                 // Write DOW
    I2C_Write(day);                                                                                 // Write day
    I2C_Write(month);                                                                               // Write month
    I2C_Write(year);                                                                                // Write year
    I2C_Stop();                                                                                     // End I2C protocol
}

void DS3231_Write_Initial_Alarm_Time()                                                              // Function to write alarm time
{
    DS3231_Read_Time();                                                                             // Read current time
    alarm_date = day;                                                                               // Set alarm to today
    char Device = 0x68;                                                                             // Device ID given
    char Address = 0x07;                                                                            // Write to register 07
    alarm_hour = dec_2_bcd(0x01);                                                                   // Convert hour to bcd
    alarm_minute = dec_2_bcd(0x01);                                                                 // Convert minute bcd
    alarm_second = dec_2_bcd(0x01);                                                                 // Convert second to bcd
    alarm_second = alarm_second & 0x7f;                                                             // Mask off bit 7
    alarm_minute = alarm_minute & 0x7f;                                                             // Mask off bit 7 
    alarm_hour   = alarm_hour   & 0x7f;                                                             // Mask off bit 7
    alarm_date   = alarm_date   | 0x80;                                                             // Set A1M4 as 1
    
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address);                                                                             // Send register address 7

    I2C_Write(alarm_second);                                                                        // Write alarm seconds value to DS3231
    I2C_Write(alarm_minute);                                                                        // Write alarm minute value to DS3231
    I2C_Write(alarm_hour);                                                                          // Write alarm hour value to DS3231
    I2C_Write(alarm_date);                                                                          // Write alarm date value to DS3231    
    I2C_Stop();                                                                                     // End I2C protocol
}    

void DS3231_Read_Alarm_Time()                                                                       // Function to read alarm time
{   
char Device = 0x68;                                                                                 // Device ID given
char Address = 0x07;                                                                                // Read from register 07
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address
    I2C_Write(Address);                                                                             // Send register address
    I2C_ReStart();                                                                                  // Restart I2C
    I2C_Write((Device << 1) | 1);                                                                   // Initialize data read
    alarm_second = I2C_Read(ACK);                                                                   // Read seconds from register 7
    alarm_minute = I2C_Read(ACK);                                                                   // Read minutes from register 8
    alarm_hour   = I2C_Read(ACK);                                                                   // Read hour from register 9
    alarm_date   = I2C_Read(NAK);                                                                   // Read hour from register A
    I2C_Stop();                                                                                     // End I2C protocol
}

void DS3231_Init()                                                                                  // Function to initialize DS3231
{
char Device = 0x68;                                                                                 // Set device to 0x68
char Address_7 = 0x07;                                                                              // Set Address_7 to 0x07
char Address_E = 0x0E;                                                                              // Set Address_E to 0x0F
char control_E;                                                                                     // Variable for control_E

    control_E = I2C_Write_Address_Read_One_Byte(Device, Address_E);                                 // Set control_E 
    control_E = control_E & 0x01;                                                                   // Logical AND control_E with 0x01
    control_E = control_E | 0x25;                                                                   // Logical OR control_E with 0x25
    I2C_Write_Address_Write_One_Byte(Device, Address_E, control_E);                                 // Call I2C_Write_Address_Write_One_Byte() function
    I2C_Start();                                                                                    // Start I2C protocol   
    I2C_Write((Device << 1) | 0);                                                                   // Device address
    I2C_Write(Address_7);                                                                           // Send register address
    I2C_ReStart();                                                                                  // Restart I2C
    I2C_Write((Device << 1) | 1);                                                                   // Initialize data read
    alarm_second = I2C_Read(ACK);                                                                   // Read seconds from register 7
    alarm_minute = I2C_Read(ACK);                                                                   // Read minutes from register 8
    alarm_hour   = I2C_Read(ACK);                                                                   // Read hour from register 9
    alarm_date   = I2C_Read(NAK);                                                                   // Read hour from register A
      
    alarm_second = alarm_second & 0x7f;                                                             // Mask off bit 7
    alarm_minute = alarm_minute & 0x7f;                                                             // Mask off bit 7 
    alarm_hour   = alarm_hour   & 0x7f;                                                             // Mask off bit 7
    alarm_date   = alarm_date   | 0x80;                                                             // Mask off bit 7
    
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address_7);                                                                           // Send register address 7

    I2C_Write(alarm_second);                                                                        // Reset alarm seconds value to DS3231
    I2C_Write(alarm_minute);                                                                        // Write alarm minute value to DS3231
    I2C_Write(alarm_hour);                                                                          // Write alarm hour value to DS3231
    I2C_Write(alarm_date);                                                                          // Write alarm date value to DS3231    
    I2C_Stop();                                                                                     // Stop I2C protocol
}

void DS3231_Write_Alarm_Time()                                                                      // Function to write alarm time
{
    DS3231_Read_Time();                                                                             // Read current time
    alarm_date = day;                                                                               // Set alarm to today
    char Device = 0x68;                                                                             // Device ID given
    char Address = 0x07;                                                                            // Write to register 07
    alarm_hour = dec_2_bcd(setup_alarm_hour);                                                       // Convert hour to bcd
    alarm_minute = dec_2_bcd(setup_alarm_minute);                                                   // Convert minute to bcd
    alarm_second = dec_2_bcd(setup_alarm_second);                                                   // Convert second to bcd
    alarm_second = alarm_second & 0x7f;                                                             // Mask off bit 7
    alarm_minute = alarm_minute & 0x7f;                                                             // Mask off bit 7 
    alarm_hour   = alarm_hour   & 0x7f;                                                             // Mask off bit 7
    alarm_date   = alarm_date   | 0x80;                                                             // Set A1M4 as 1
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address);                                                                             // Send register address 7

    I2C_Write(alarm_second);                                                                        // Write alarm seconds value to DS3231
    I2C_Write(alarm_minute);                                                                        // Write alarm minute value to DS3231
    I2C_Write(alarm_hour);                                                                          // Write alarm hour value to DS3231
    I2C_Write(alarm_date);                                                                          // Write alarm date value to DS3231    
    I2C_Stop();                                                                                     // End I2C protocol
}    

void DS3231_Turn_Off_Alarm()                                                                        // Function to turn off alarm
{
char Device = 0x68;                                                                                 // Set device to 0x68
char Address_E = 0x0E;                                                                              // Set Address_E to 0x0E
char Address_F = 0x0F;                                                                              // Set Address_F to 0x0F
char control_E;                                                                                     // Variable for control_E
char control_F;                                                                                     // Variable for control_F
    
    RTC_ALARM_NOT = 1;                                                                              // Set RTC_ALARM_NOT to 1
    control_E = I2C_Write_Address_Read_One_Byte(Device, Address_E);                                 // Set control_E variable
    control_E = control_E & 0xFE;                                                                   // Logical AND control_E with 0xFE
    I2C_Write_Address_Write_One_Byte(Device, Address_E, control_E);                                 // Call I2C_Write_Address_Write_One_Byte() function
    control_F = I2C_Write_Address_Read_One_Byte(Device, Address_F);                                 // Set control_F variable
    control_F = control_F & 0xFE;                                                                   // Logical AND control_F with 0xFE
    I2C_Write_Address_Write_One_Byte(Device, Address_F, control_F);                                 // Call I2C_Write_Address_Write_One_Byte() function
    DS3231_Init();                                                                                  // Initialize DS3231
}

void DS3231_Turn_On_Alarm()                                                                         // Function to turn on alarm
{
char Device = 0x68;                                                                                 // Set device to 0x68
char Address_E = 0x0E;                                                                              // Set Address_E to 0x0E
char Address_F = 0x0F;                                                                              // Set Address_F to 0x0F
char control_E;                                                                                     // Variable for control_E
char control_F;                                                                                     // Variable for control_F

    control_E = I2C_Write_Address_Read_One_Byte(Device, Address_E);                                 // Set control_E variable
    control_E = control_E | 0x01;                                                                   // Logical OR control_E with 0x01
    I2C_Write_Address_Write_One_Byte(Device, Address_F, control_F);                                 // Call I2C_Write_Address_Write_One_Byte() function                                 
    
    control_F = I2C_Write_Address_Read_One_Byte(Device, Address_F);                                 // Set control_F variable
    control_F = control_F & 0xFE;                                                                   // Logical AND control_F with 0xFE
    I2C_Write_Address_Write_One_Byte(Device, Address_F, control_F);                                 // Call I2C_Write_Address_Write_One_Byte() function
    DS3231_Init();                                                                                  // Initialize DS3231
    
}

int bcd_2_dec (char bcd)                                                                            // Function to convert bcd to decimal
{
    int dec;                                                                                        // Variable for decimal result
    dec = ((bcd>> 4) * 10) + (bcd & 0x0f);                                                          // Convert bcd to decimal
    return dec;                                                                                     // Return dec
}

int dec_2_bcd (char dec)                                                                            // Function to convert decimal to bcd
{
    int bcd;                                                                                        // Variable for bcd result
    bcd = ((dec / 10) << 4) + (dec % 10);                                                           // Convert decimal to bcd
    return bcd;                                                                                     // Return bcd
}

