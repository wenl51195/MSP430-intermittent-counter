#include <msp430.h>

// Define the 16-bit counter variable
unsigned int counter;
unsigned long *FRAM_write_ptr;
unsigned long *FRAM_write_ptr1;
unsigned long *FRAM_read_ptr;

#define FRAM_TEST_START 0x1800

// Calculate odd parity for a 16-bit value
unsigned char CheckCounter(unsigned int value) {
    unsigned char parity = 0;
    while (value) {
        parity ^= (value & 1);
        value >>= 1;
    }
    return parity;
}

// Restore the counter value from FRAM
unsigned int RestoreCounter() {
    unsigned int data = *FRAM_write_ptr1;
	return data;
}

void FRAMWrite(void) {
    unsigned char parity = CheckCounter(counter);
    SYSCFG0 &= ~DFWP;     // Disable Data FRAM Write Protection
    *FRAM_write_ptr = ((unsigned long) counter << 8) | parity;
    *FRAM_write_ptr1 = (unsigned long) counter;
    SYSCFG0 |= DFWP;      // Enable Data FRAM Write Protection
}

void FRAMRead(void) {
    unsigned long data = *FRAM_read_ptr;
    unsigned char read_parity = data & 0xFF;
    unsigned int read_counter = data >> 8;
    unsigned char calc_parity = CheckCounter(read_counter);
	
    if (read_parity == calc_parity) {
        counter = read_counter;
    } else {
        counter = RestoreCounter();
    }
}

void main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    
	// Initialize FRAM read/write pointers
    FRAM_write_ptr = (unsigned long *)FRAM_TEST_START;
    FRAM_write_ptr1 = FRAM_write_ptr + 1;
    FRAM_read_ptr = FRAM_write_ptr;
	
	FRAMRead();
	
    while(1) {
        P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
        counter++;
        FRAMWrite();
        __delay_cycles(1000000);            // Delay for 1000000*(1/MCLK)=1s
    }
}