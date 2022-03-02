void interrupt  high_priority chkisr(void);     // Function to check interrupts
void INT0_ISR(void);                            // Function for INT0 service routine
void INT1_ISR(void);                            // Function for INT1 service routine
void INT2_ISR(void);                            // Function for INT2 service routine
void T0_ISR();                                  // Function for Timer0 interrupt service routine
void Init_Interrupt(void);                      // Function to initialize interrupts

