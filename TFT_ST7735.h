void drawRoundRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char r, unsigned int color);        // Function to draw round recteangle
void fillRoundRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char r, unsigned int color);        // Function to fill round rectangle
void drawTriangle(signed int x0, signed int y0, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);    // Function to draw triangle
void fillTriangle(signed int x0, signed int y0, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);    // Function to fill triangle 
void setTextWrap(BOOL );                                                                                                            // Function to set text wrap
void invertDisplay(BOOL );                                                                                                          // Function to invert display 
void NormalDisplay();                                                                                                               // Function for normal display
int Color565(int r, int g, int b);                                                                                                  // Function for color565
void spiwrite(unsigned char spidata);                                                                                               // Function to write spidata
void write_command(unsigned char cmd_);                                                                                             // Function to write command
void write_data(unsigned char data_);                                                                                               // Function to write data
void Rcmd1();                                                                                                                       // Function for Rcmd1
void Rcmd2red();                                                                                                                    // Function for Rcmd2red
void Rcmd2green();                                                                                                                  // Function for Rcmd2green
void Rcmd3();                                                                                                                       // Function for Rcmd3
void setScrollDefinition(unsigned char top_fix_height, unsigned char bottom_fix_height, BOOL _scroll_direction);                    // Function to set scroll definition
void setAddrWindow(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);                                         // Function to set address window
void drawPixel(unsigned char x, unsigned char y, unsigned int color);                                                               // Function to draw pixel
void drawFastVLine(unsigned char x, unsigned char y, unsigned char h, unsigned int color);                                          // Function to draw fast vline
void fillRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned int color);                              // Function to fill rectangle
void drawtext(unsigned char x, unsigned char y, char *_text, unsigned int color, unsigned int bg, unsigned char size);              // Function to draw text
void drawChar(unsigned char x, unsigned char y, unsigned char c, unsigned int color, unsigned int bg,  unsigned char size);         // Function to draw character
void fillRectangle(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned int color);                         // Function to fill rectangle
void drawCircle(signed int x0, signed int y0, signed int r, unsigned int color);                                                    // Function to draw circle
void drawCircleHelper(signed int x0, signed int y0, signed int r, unsigned int cornername, unsigned int color);                     // Function for draw circle helper
void fillCircle(signed int x0, signed int y0, signed int r, unsigned int color);                                                    // Function to fill circle
void fillCircleHelper(signed int x0, signed int y0, signed int r, unsigned int cornername, signed int delta, unsigned int color);   // Function for fill circle helper
void fillScreen(unsigned int color);                                                                                                // Function to fill screen
void drawFastHLine(unsigned char x, unsigned char y, unsigned char w, unsigned int color);                                          // Function to draw fast Hline
void VerticalScroll(unsigned char _vsp);                                                                                            // Function for vertical scroll
void TFT_BlackTab_Initialize();                                                                                                     // Function to initialize TFT blacktab
void TFT_GreenTab_Initialize();                                                                                                     // Function to initialize TFT greentab
void TFT_RedTab_Initialize(void);                                                                                                   // Function to initialize TFT redtab
void TFT_ST7735B_Initialize(void);                                                                                                  // Function to initialize TFT ST7735B
void LCD_Reset(void);                                                                                                               // Function to reset LCD
void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);                                  // Function to draw rectangle

BOOL wrap = TRUE;                                                                                                                   // Set wrap equal to TRUE


unsigned char colstart = 0, rowstart = 0, _tft_type;
#define _swap(a, b) { signed int t; t = a; a = b; b = t;}
#define _width         128
#define _height        160
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
#define ST7735_PTLAR   0x30
#define ST7735_VSCRDEF 0x33
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
#define ST7735_VSCRSADD 0x37
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD
#define ST7735_PWCTR6  0xFC
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

                                                                                                                                    // Color definitions
#define   ST7735_BLACK   0x0000                                                                                                     // Black
#define   ST7735_BLUE    0x001F                                                                                                     // Blue
#define   ST7735_RED     0xF800                                                                                                     // Red
#define   ST7735_GREEN   0x07E0                                                                                                     // Green
#define   ST7735_CYAN    0x07FF                                                                                                     // Cyan
#define   ST7735_MAGENTA 0xF81F                                                                                                     // Magenta
#define   ST7735_YELLOW  0xFFE0                                                                                                     // Yellow
#define   ST7735_WHITE   0xFFFF                                                                                                     // White


