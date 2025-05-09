# Intermittent Counter on MSP430FR4133

## Overview
This is a simple intermittent counter program running on MSP430FR4133 microcontroller. The program increments a counter every second and saves its value to FRAM while indicating its status through an LED. Use FRAM to save the counter value when the power is off. The counter will resume counting from that value when the power is restored.

## Features
- 16-bit counter implementation
- Non-volatile storage using FRAM memory
- Parity error detection and recovery mechanism
- LED indicator for system operation status

## Hardware & Development Platform
- MSP430FR4133 microcontroller
- LED connected to pin P1.0
- Code Composer Studio (CCS)

## Code Structure
1. **FRAM Operations**:
   - `FRAMWrite()`: Writes counter value to FRAM with parity check
   - `FRAMRead()`: Reads counter value from FRAM and performs parity verification
2. **Parity Check**: `CheckCounter()` function calculates parity for 16-bit values
3. **Backup Recovery**: `RestoreCounter()` function recovers counter value from backup location
4. **Main Loop**: Setting up hardware, initializing FRAM pointers, increments counter, updates FRAM, and toggles LED state

## Memory Layout
- Main counter value and parity bit stored at FRAM_TEST_START (0x1800)
- Backup counter value stored at FRAM_TEST_START+4

## Error Handling
The program implements a simple error detection and recovery mechanism:
- Calculates parity bit for counter value
- Verifies parity bit during read operations
- Recovers counter value from backup location if an error is detected