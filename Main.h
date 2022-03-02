#define _XTAL_FREQ      8000000                 // Define _XTAL_FREQ as 8000000

#define ACK             1                       // Define ACK as 1
#define NAK             0                       // Define NAK as 0

#define ACCESS_CFG      0xAC                    // Define ACCESS_CFG as 0xAC
#define START_CONV      0xEE                    // Define START_CONV as 0xEE
#define READ_TEMP       0xAA                    // Define READ_TEMP as 0xAA
#define CONT_CONV       0x02                    // Define CONT_CONV as 0x02

#define TFT_DC          PORTDbits.RD0           // Define TFT_DC as RD0
#define TFT_CS          PORTDbits.RD1           // Define TFT_CS as RD1
#define TFT_RST         PORTDbits.RD2           // Define TFT_RST as RD2 

#define enter_setup     PORTAbits.RA1           // Define enter_setup as RA1
#define setup_sel0      PORTAbits.RA4           // Define setup_sel0 as RA4
#define setup_sel1      PORTAbits.RA5           // Define setup_sel1 as RA5

#define SEC_LED         PORTDbits.RD6           // Define SEC_LED as RD6

#define FANEN_LED       PORTDbits.RD7           // Define FANEN_LED as RD7
#define RTC_ALARM_NOT   PORTEbits.RE0           // Define RTC_ALARM_NOT as RE0


#define RGB_RED         PORTDbits.RD3           // Define RGB_RED as RD3
#define RGB_GREEN       PORTDbits.RD4           // Define RGB_GREEN as RD4
#define RGB_BLUE        PORTDbits.RD5           // Define RGB_BLUE as RD5

