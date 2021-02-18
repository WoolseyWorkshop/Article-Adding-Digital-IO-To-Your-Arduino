/**
 * @mainpage Arduino And 74HC165 Input Shift Register Project
 *
 * @section description_main Description
 * Interfaces a 74HC165 parallel-in serial-out shift register IC to add digital
 * inputs to an Arduino board.
 *
 * @section circuit_main Circuit
 * - A 74HC165 shift register IC is connected to pins D2, D3, and D4.
 *   - D2 is connected to the 74HC165 QH (9) pin.
 *   - D3 is connected to the 74HC165 SH/LD (1) pin.
 *   - D4 is connected to the 74HC165 CLK (2) pin.
 *   - 8 switches, with pull-down resistors, are connected to the shift
 *     register's inputs (A - H).
 *
 * @section notes_main Notes
 * - Provides examples for multiple approaches to implementing how to visualize
 *   and read digital input data from the 74HC165 shift register.
 * - Use the Serial Monitor to view output.
 *
 * Copyright (c) 2021 Woolsey Workshop.  All rights reserved.
 */


/**
 * @file InputShiftRegister.ino
 *
 * @brief Primary Arduino sketch implementation.
 *
 * @section description_inputshiftregister_ino Description
 * An Arduino sketch that interfaces a 74HC165 parallel-in serial-out shift
 * register IC to add digital inputs to an Arduino board.
 *
 * @section notes_inputshiftregister_ino Notes
 * - This sketch assumes a single 74HC165 shift register IC is being utilized.
 *   If two or more '165s are daisy chained together, the size of the uint8_t
 *   data type used to read the input data needs to be increased.
 *   For instance, use uint16_t for two '165s and uint32_t for four '165s.
 *   See function specific comments for additional details.
 * - Comments are Doxygen compatible.
 *
 * @section todo_inputshiftregister_ino TODO
 * - None.
 *
 * @section author_inputshiftregister_ino Author(s)
 * - Created by John Woolsey on 01/31/2021.
 * - Modified by John Woolsey on 02/12/2021.
 *
 * Copyright (c) 2021 Woolsey Workshop.  All rights reserved.
 */


// Defines
#define SAMPLE_RATE 0.2  ///< The sensor sampling rate in Hz.


// Pin Mapping
const int ISRDataPin = 2;   ///< The pin connected to the 74HC165 QH (9) pin.
const int ISRLatchPin = 3;  ///< The pin connected to the 74HC165 SH/LD (1) pin.
const int ISRClockPin = 4;  ///< The pin connected to the 74HC165 CLK (2) pin.


// Global Constants
const uint8_t InputA = 0;  ///< The bit position for the 74HC165 A input.
const uint8_t InputB = 1;  ///< The bit position for the 74HC165 B input.
const uint8_t InputC = 2;  ///< The bit position for the 74HC165 C input.
const uint8_t InputD = 3;  ///< The bit position for the 74HC165 D input.
const uint8_t InputE = 4;  ///< The bit position for the 74HC165 E input.
const uint8_t InputF = 5;  ///< The bit position for the 74HC165 F input.
const uint8_t InputG = 6;  ///< The bit position for the 74HC165 G input.
const uint8_t InputH = 7;  ///< The bit position for the 74HC165 H input.


/**
 * Standard Arduino setup function used for setup and configuration tasks.
 */
void setup() {
   // Serial Monitor
   Serial.begin(9600);  // initialize serial bus
   while (!Serial);     // wait for serial connection

   // 74HC165 shift register
   pinMode(ISRDataPin, INPUT);
   pinMode(ISRLatchPin, OUTPUT);
   pinMode(ISRClockPin, OUTPUT);
}


/**
 * Standard Arduino loop function used for repeating tasks.
 */
void loop() {
   // Read and print inputs at the specified sampling rate
   static unsigned long previousTime = 0;
   unsigned long currentTime = millis();
   if (currentTime - previousTime >= 1000/SAMPLE_RATE) {
      readInputsWithDigitalRead();
      // readInputsWithBinaryValues();
      // readInputsWithDefinedNamesAndBitOperations();
      previousTime = currentTime;
   }

   // Read inputs and print values upon any changes
   // readAndPrintInputsOnChange();
}


/**
 * Reads (shifts) all input values from the 74HC165 PISO shift register.
 *
 * The standard Arduino shiftIn() function reads values after a rising clock
 * edge.  The 74HC165 requires values to be read before a rising clock edge.
 * Therefore, the clock pin is driven high early in the reading sequence to take
 * this difference into account.
 *
 * The shift register continuously latches inputs when not being read.
 *
 * If daisy chaining multiple shift registers together, increase the size of the
 * inputs variable and return types (uint8_t) and add extra shiftIn() operations
 * for each additional byte.
 *
 * @return  A byte representing all 8 inputs.
 */
uint8_t isrReadRegister() {
   uint8_t inputs = 0;
   digitalWrite(ISRClockPin, HIGH);  // preset clock to retrieve first bit
   digitalWrite(ISRLatchPin, HIGH);  // disable input latching and enable shifting
   inputs = shiftIn(ISRDataPin, ISRClockPin, MSBFIRST);  // capture input values
   digitalWrite(ISRLatchPin, LOW);  // disable shifting and enable input latching
   return inputs;
}


/**
 * Reads the value of a single 74HC165 shift register input.
 *
 * Implements the same calling mechanism as Arduino's standard digitalRead()
 * function.
 *
 * @param pin  The input pin to read, 0-7 for a single 8-bit shift register.
 *
 * @return  The value of the digital input, HIGH (1) or LOW (0).
 */
int isrDigitalRead(uint8_t pin) {
   return bitRead(isrReadRegister(), pin);
}


/**
 * Example code for reading an individual shift register input with each read.
 *
 * This approach should be the most familiar to Arduino users.  It uses the same
 * calling mechanism as Arduino's standard digitalRead() function, but may
 * involve more shift operations than other approaches since only one input at
 * a time can be read.
 */
void readInputsWithDigitalRead() {
   // Read and print individual inputs
   Serial.print("Input A = ");  Serial.println(isrDigitalRead(InputA) ? "HIGH" : "LOW");
   Serial.print("Input B = ");  Serial.println(isrDigitalRead(InputB) ? "HIGH" : "LOW");
   Serial.print("Input C = ");  Serial.println(isrDigitalRead(InputC) ? "HIGH" : "LOW");
   Serial.print("Input D = ");  Serial.println(isrDigitalRead(InputD) ? "HIGH" : "LOW");
   Serial.print("Input E = ");  Serial.println(isrDigitalRead(InputE) ? "HIGH" : "LOW");
   Serial.print("Input F = ");  Serial.println(isrDigitalRead(InputF) ? "HIGH" : "LOW");
   Serial.print("Input G = ");  Serial.println(isrDigitalRead(InputG) ? "HIGH" : "LOW");
   Serial.print("Input H = ");  Serial.println(isrDigitalRead(InputH) ? "HIGH" : "LOW");
   Serial.println();
}


/**
 * Example code for reading all shift register inputs with each read using
 * binary values (1 = HIGH, 0 = LOW).
 *
 * This approach produces the most concise code, but does not indicate the
 * meaning of each of the inputs.
 */
void readInputsWithBinaryValues() {
   // Read and print all inputs from shift register in binary format
   Serial.print("Inputs: 0b");
   Serial.println(isrReadRegister(), BIN);
}


/**
 * Example code for reading all shift register inputs with each read and
 * accessing individually named inputs using bit operations.
 *
 * This approach provides named inputs and less shift register reads, but
 * requires a variable to hold input values.
 *
 * If daisy chaining multiple shift registers together, increase the size of the
 * inputs variable type (uint8_t).
 */
void readInputsWithDefinedNamesAndBitOperations() {
   // Read all inputs from shift register
   uint8_t inputs = isrReadRegister();

   // Read and print individual inputs
   Serial.print("Input A = ");  Serial.println(bitRead(inputs, InputA) ? "HIGH" : "LOW");
   Serial.print("Input B = ");  Serial.println(bitRead(inputs, InputB) ? "HIGH" : "LOW");
   Serial.print("Input C = ");  Serial.println(bitRead(inputs, InputC) ? "HIGH" : "LOW");
   Serial.print("Input D = ");  Serial.println(bitRead(inputs, InputD) ? "HIGH" : "LOW");
   Serial.print("Input E = ");  Serial.println(bitRead(inputs, InputE) ? "HIGH" : "LOW");
   Serial.print("Input F = ");  Serial.println(bitRead(inputs, InputF) ? "HIGH" : "LOW");
   Serial.print("Input G = ");  Serial.println(bitRead(inputs, InputG) ? "HIGH" : "LOW");
   Serial.print("Input H = ");  Serial.println(bitRead(inputs, InputH) ? "HIGH" : "LOW");
   Serial.println();
}


/**
 * Example code for reading all shift register inputs in a single read and using
 * bit operations to print all the values when an input change is detected.
 *
 * If daisy chaining multiple shift registers together, increase the size of the
 * previousInputs and currentInputs variable types (uint8_t) and adjust the
 * number of iterations of the for loop.
 */
void readAndPrintInputsOnChange() {
   static uint8_t previousInputs = 0;
   uint8_t currentInputs = isrReadRegister();  // read all inputs from shift register
   if (currentInputs != previousInputs) {  // print values only if they changed
      Serial.print("Inputs: 0b");  // print all inputs represented as a full byte
      for (int i = 7; i >= 0; i--) {
         Serial.print(bitRead(currentInputs, i));  // print value for each bit
      }
      Serial.println();
      previousInputs = currentInputs;
   }
}
