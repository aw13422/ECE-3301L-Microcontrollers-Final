void I2C_Wait(void);                                        // Function to wait for I2C
void I2C_Start(void);                                       // Function to start I2C
void I2C_ReStart(void);                                     // Function to restart I2C
void I2C_Stop(void);                                        // Function to stop I2C
char I2C_Read(char);                                        // Function to read I2C
void I2C_Write(unsigned char);                              // Function to write I2C
void I2C_Write_Cmd_Only(char, char);                        // Function to write cmd only for I2C
void I2C_Write_Cmd_Write_Data(char, char, char);            // Function to write cmd write date for I2C
void I2C_Write_Address_Write_One_Byte(char, char, char);    // Function to write address write one byte for I2C
BYTE I2C_Write_Address_Read_One_Byte(char, char);           // Function to write address read one byte for I2C
 
void i2cHighSda(void);                                      // Function for i2cHighSda
void i2cLowSda(void);                                       // Function for i2cLowSda
void i2cHighScl(void);                                      // Function for i2cHighScl
void i2cLowScl(void);                                       // Function for i2cLowScl
void i2cNack(void);                                         // Function for i2cNack
void i2cAck(void);                                          // Function for i2cAck

void I2C_Init(unsigned long);                               // Function for I2C initialization

