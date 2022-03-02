void DS1621_Init(void);                         // Function to initialize DS1621
void DS3231_Read_Time(void);                    // Function to read time
void DS3231_Write_Time(void);                   // Function to write time
void DS3231_Read_Alarm_Time(void);              // Function to read alarm time
void DS3231_Write_Alarm_Time(void);             // Function to write alarm time
int DS3231_Read_Temp(void);                     // Function to read temperature
int DS1621_Read_Temp(void);                     // Function to read temperature
void DS3231_Turn_On_Alarm();                    // Function to turn on alarm
void DS3231_Turn_Off_Alarm();                   // Function to turn off alarm
void DS3231_Init();                             // Function to initialize DS3231
void DS3231_Write_Initial_Alarm_Time(void);     // Function to write initial alarm time
int dec_2_bcd (char);                           // Function to convert decimal to bcd
int bcd_2_dec (char);                           // Function to convert bcd to decimal

