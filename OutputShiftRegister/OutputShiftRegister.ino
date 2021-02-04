/**
 * @mainpage Arduino And 74HC595 Output Shift Register Project
 *
 * @section description_main Description
 * Interfaces a 74HC595 serial-in parallel-out shift register IC to add digital
 * outputs to an Arduino board.
 *
 * @section circuit_main Circuit
 * - A 74HC595 shift register IC is connected to pins D2, D3, and D4.
 *   - D2 is connected to the 74HC595 SER (14) pin.
 *   - D3 is connected to the 74HC595 RCLK (12) pin.
 *   - D4 is connected to the 74HC595 SRCLK (11) pin.
 *   - 8 LEDs are connected to the shift register's outputs (QA - QH).
 *
 * @section notes_main Notes
 * - Provides examples for multiple approaches to implementing how to visualize
 *   and send digital output data to the 74HC595 shift register.
 *
 * Copyright (c) 2021 Woolsey Workshop.  All rights reserved.
 */


/**
 * @file OutputShiftRegister.ino
 *
 * @brief Primary Arduino sketch implementation.
 *
 * @section description_outputshiftregister_ino Description
 * An Arduino sketch that interfaces a 74HC595 serial-in parallel-out shift
 * register IC to add digital outputs to an Arduino board.
 *
 * @section notes_outputshiftregister_ino Notes
 * - This sketch assumes a single 74HC595 shift register IC is being utilized.
 *   If two or more '595s are daisy chained together, the size of the uint8_t
 *   data type used to write the output data needs to be increased.
 *   For instance, use uint16_t for two '595s and uint32_t for four '595s.
 *   See function specific comments for additional details.
 * - Comments are Doxygen compatible.
 *
 * @section todo_outputshiftregister_ino TODO
 * - None.
 *
 * @section author_outputshiftregister_ino Author(s)
 * - Created by John Woolsey on 01/20/2021.
 * - Modified by John Woolsey on 02/04/2021.
 *
 * Copyright (c) 2021 Woolsey Workshop.  All rights reserved.
 */


// Pin Mapping
const int OSRDataPin = 2;   ///< The pin connected to the 74HC595 SER (14) pin.
const int OSRLatchPin = 3;  ///< The pin connected to the 74HC595 RCLK (12) pin.
const int OSRClockPin = 4;  ///< The pin connected to the 74HC595 SRCLK (11) pin.


/**
 * Standard Arduino setup function used for setup and configuration tasks.
 */
void setup() {
   // 74HC595 shift register
   pinMode(OSRDataPin, OUTPUT);
   pinMode(OSRLatchPin, OUTPUT);
   pinMode(OSRClockPin, OUTPUT);
   osrWriteRegister(0);  // turn off all LEDs

   delay(1000);  // wait a second
}


/**
 * Standard Arduino loop function used for repeating tasks.
 */
void loop() {
   // Each function provides a different approach for setting outputs
   changeOutputsWithDigitalWrite();
   changeOutputsWithBinaryValues();
   changeOutputsWithDefinedNames();
   changeOutputsWithBitOperations();
   // cycleLEDs();
}


/**
 * Writes (shifts) all output values into the 74HC595 SIPO shift register.
 *
 * If daisy chaining multiple shift registers together, increase the size of the
 * outputs argument type (uint8_t) and add extra shiftOut() operations for each
 * additional byte.
 *
 * @param outputs  A byte representing all 8 outputs.
 */
void osrWriteRegister(uint8_t outputs) {
   // Initiate latching process, next HIGH latches data
   digitalWrite(OSRLatchPin, LOW);
   // Shift output data into the shift register, most significant bit first
   shiftOut(OSRDataPin, OSRClockPin, MSBFIRST, outputs);
   // Latch outputs into the shift register
   digitalWrite(OSRLatchPin, HIGH);
}


/**
 * Sets the value of a single 74HC595 shift register output.
 *
 * Implements the same calling mechanism as Arduino's standard digitalWrite()
 * function.
 *
 * Values other than LOW or 0 and HIGH or 1 are ignored.
 *
 * Increase the size of the outputs variable type (uint8_t) if daisy chaining
 * multiple shift registers together.
 *
 * @param pin    The output pin to set, 0-7 for a single 8-bit shift register.
 * @param value  The value to set the pin to, LOW or HIGH.
 */
void osrDigitalWrite(uint8_t pin, uint8_t value) {
   static uint8_t outputs = 0;  // retains shift register output values

   if (value == HIGH) bitSet(outputs, pin);  // set output pin to HIGH
   else if (value == LOW) bitClear(outputs, pin);  // set output pin to LOW
   osrWriteRegister(outputs);  // write all outputs to shift register
}


/**
 * Example code for setting an individual shift register output with each write.
 *
 * This approach should be the most familiar to Arduino users.  It uses the same
 * calling mechanism as Arduino's standard digitalWrite() function, but may
 * involve more shift operations than other approaches since only one output at
 * a time can be changed.
 */
void changeOutputsWithDigitalWrite() {
   // Output pin definitions
   const uint8_t LED0 = 0;
   const uint8_t LED1 = 1;
   const uint8_t LED2 = 2;
   const uint8_t LED3 = 3;
   const uint8_t LED4 = 4;
   const uint8_t LED5 = 5;
   const uint8_t LED6 = 6;
   const uint8_t LED7 = 7;

   // Set individual LEDs
   osrDigitalWrite(LED1, HIGH);  // turn on LED1 only
   delay(1000);
   osrDigitalWrite(LED1, LOW);   // turn off LED1 only
   osrDigitalWrite(LED6, HIGH);  // turn on LED6 only
   delay(1000);
   osrDigitalWrite(LED6, LOW);   // turn off LED6 only
   delay(1000);

   // Set multiple LEDs
   osrDigitalWrite(LED0, HIGH);  // turn on even numbered LEDs
   osrDigitalWrite(LED2, HIGH);
   osrDigitalWrite(LED4, HIGH);
   osrDigitalWrite(LED6, HIGH);
   delay(1000);
   osrDigitalWrite(LED0, LOW);   // turn off even numbered LEDs
   osrDigitalWrite(LED2, LOW);
   osrDigitalWrite(LED4, LOW);
   osrDigitalWrite(LED6, LOW);
   osrDigitalWrite(LED1, HIGH);  // turn on odd numbered LEDs
   osrDigitalWrite(LED3, HIGH);
   osrDigitalWrite(LED5, HIGH);
   osrDigitalWrite(LED7, HIGH);
   delay(1000);
   osrDigitalWrite(LED1, LOW);   // turn off odd numbered LEDs
   osrDigitalWrite(LED3, LOW);
   osrDigitalWrite(LED5, LOW);
   osrDigitalWrite(LED7, LOW);
   delay(1000);
}


/**
 * Example code for setting all shift register outputs with each write using
 * binary values (1 = HIGH, 0 = LOW).
 *
 * This approach produces the most concise code, but does not portray the
 * meaning of each of the outputs.
 */
void changeOutputsWithBinaryValues() {
   // Set individual LEDs
   osrWriteRegister(0b00000010);  // turn on LED1 only
   delay(1000);
   osrWriteRegister(0b01000000);  // turn on LED6 only
   delay(1000);
   osrWriteRegister(0b00000000);  // turn off all LEDs
   delay(1000);

   // Set multiple LEDs
   osrWriteRegister(0b01010101);  // turn on only even numbered LEDs
   delay(1000);
   osrWriteRegister(0b10101010);  // turn on only odd numbered LEDs
   delay(1000);
   osrWriteRegister(0b00000000);  // turn off all LEDs
   delay(1000);
}


/**
 * Example code for setting all shift register outputs with each write using
 * named outputs.
 *
 * This approach provides the ability to use named outputs with single shift
 * register writes, but all outputs must still be represented with each write.
 *
 * Only include named outputs to set HIGH, everything else will be set LOW.
 *
 * Increase the size of the constants' type (uint8_t) if daisy chaining
 * multiple shift registers together.
 */
void changeOutputsWithDefinedNames() {
   // Output pin definitions
   const uint8_t LED0 = 0b00000001;
   const uint8_t LED1 = 0b00000010;
   const uint8_t LED2 = 0b00000100;
   const uint8_t LED3 = 0b00001000;
   const uint8_t LED4 = 0b00010000;
   const uint8_t LED5 = 0b00100000;
   const uint8_t LED6 = 0b01000000;
   const uint8_t LED7 = 0b10000000;

   // Set individual LEDs
   osrWriteRegister(LED1);  // turn on LED1 only
   delay(1000);
   osrWriteRegister(LED6);  // turn on LED6 only
   delay(1000);
   osrWriteRegister(0);     // turn off all LEDs
   delay(1000);

   // Set multiple LEDs
   osrWriteRegister(LED0 | LED2 | LED4 | LED6);  // turn on only even numbered LEDs
   delay(1000);
   osrWriteRegister(LED1 | LED3 | LED5 | LED7);  // turn on only odd numbered LEDs
   delay(1000);
   osrWriteRegister(0);                          // turn off all LEDs
   delay(1000);
}


/**
 * Example code for using bit operations to change individual shift register
 * outputs.
 *
 * This approach provides named outputs, single output updatability, and less
 * shift register writes, but requires a variable to keep track of output values
 * and can produce lengthy code when changing many outputs simultaneously.
 *
 * Increase the size of the outputs variable type (uint8_t) if daisy chaining
 * multiple shift registers together.
 */
void changeOutputsWithBitOperations() {
   // Output pin definitions
   const uint8_t LED0 = 0;
   const uint8_t LED1 = 1;
   const uint8_t LED2 = 2;
   const uint8_t LED3 = 3;
   const uint8_t LED4 = 4;
   const uint8_t LED5 = 5;
   const uint8_t LED6 = 6;
   const uint8_t LED7 = 7;

   uint8_t outputs = 0;  // holds shift register output values

   // Set individual LEDs
   bitSet(outputs, LED1);    // turn on LED1
   osrWriteRegister(outputs);
   delay(1000);
   bitClear(outputs, LED1);  // turn off LED1
   bitSet(outputs, LED6);    // turn on LED6
   osrWriteRegister(outputs);
   delay(1000);
   bitClear(outputs, LED6);  // turn off LED6
   osrWriteRegister(outputs);
   delay(1000);

   // Set multiple LEDs
   bitSet(outputs, LED0);    // turn on even numbered LEDs
   bitSet(outputs, LED2);
   bitSet(outputs, LED4);
   bitSet(outputs, LED6);
   osrWriteRegister(outputs);
   delay(1000);
   bitClear(outputs, LED0);  // turn off even numbered LEDs
   bitClear(outputs, LED2);
   bitClear(outputs, LED4);
   bitClear(outputs, LED6);
   bitSet(outputs, LED1);    // turn on odd numbered LEDs
   bitSet(outputs, LED3);
   bitSet(outputs, LED5);
   bitSet(outputs, LED7);
   osrWriteRegister(outputs);
   delay(1000);
   bitClear(outputs, LED1);  // turn off odd numbered LEDs
   bitClear(outputs, LED3);
   bitClear(outputs, LED5);
   bitClear(outputs, LED7);
   osrWriteRegister(outputs);
   delay(1000);
}


/**
 * Example code that continuously cycles through the LEDs (end to end) using
 * simple bit operations.
 *
 * Increase the size of the outputs variable type (uint8_t) and adjust the
 * for loops if daisy chaining multiple shift registers together.
 */
void cycleLEDs() {
   uint8_t outputs = 0;  // holds shift register output values

   // Cycle through individual LEDs from LED0 to LED7
   for (int i = 0; i < 8; i++) {
      bitSet(outputs, i);
      osrWriteRegister(outputs);
      bitClear(outputs, i);
      if (i < 7) delay(100);
   }

   // Cycle through individual LEDs from LED7 to LED0
   for (int i = 7; i >= 0; i--) {
      bitSet(outputs, i);
      osrWriteRegister(outputs);
      bitClear(outputs, i);
      if (i > 0) delay(100);
   }
}
