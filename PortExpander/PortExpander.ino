/**
 * @mainpage Arduino And MCP23017 I/O Expander Project
 *
 * @section description_main Description
 * Interfaces an MCP23017 16-Bit I2C I/O Expander With Serial Interface IC to
 * add digital inputs and outputs to an Arduino board.
 *
 * @section circuit_main Circuit
 * - An MCP23017 I/O expander IC is connected to pins SCL, SDA, and D2.
 *   - SCL is connected to the MCP23017 SCK (12) pin.
 *   - SDA is connected to the MCP23017 SDA (13) pin.
 *   - D2 is connected to the MCP23017 INTB (19) pin.
 *   - 8 LEDs are connected to the MCP23017 GPA0-GPA7 (21-28) pins.
 *   - 8 switches are connected to the MCP23017 GPB0-GPB7 (1-8) pins.
 *
 * @section notes_main Notes
 * - Provides examples for multiple approaches to configuring and using digital
 *   I/O with the MCP23017 I/O expander IC.
 *
 * Copyright (c) 2021 Woolsey Workshop.  All rights reserved.
 */


/**
 * @file PortExpander.ino
 *
 * @brief Primary Arduino sketch implementation.
 *
 * @section description_portexpander_ino Description
 * An Arduino sketch that interfaces an MCP23017 16-Bit I2C I/O Expander With
 * Serial Interface IC to add digital inputs and outputs to an Arduino board.
 *
 * @section libraries_portexpander_ino Libraries
 * - MCP23017 (https://github.com/blemasle/arduino-mcp23017)
 *   - Provides full control of the MCP23017 IC.
 *
 * @section notes_portexpander_ino Notes
 * - Comments are Doxygen compatible.
 *
 * @section todo_portexpander_ino TODO
 * - None.
 *
 * @section author_portexpander_ino Author(s)
 * - Created by John Woolsey on 02/23/2021.
 * - Modified by John Woolsey on 03/18/2021.
 *
 * Copyright (c) 2021 Woolsey Workshop.  All rights reserved.
 */


// Includes
#include <MCP23017.h>


// Defines
#define MCP23017_I2C_ADDRESS 0x20  ///< The I2C address of the MCP23017 IC.


// Pin Mapping
const int MCP23017_INTB = 2;  ///< The pin connected to the MCP23017 INTB (19) pin.


// MCP23017 I/O Pin Definitions
const uint8_t LED0 = 0;      ///< Refers to pin GPA0 (21) of the MCP23017.
const uint8_t LED1 = 1;      ///< Refers to pin GPA1 (22) of the MCP23017.
const uint8_t LED2 = 2;      ///< Refers to pin GPA2 (23) of the MCP23017.
const uint8_t LED3 = 3;      ///< Refers to pin GPA3 (24) of the MCP23017.
const uint8_t LED4 = 4;      ///< Refers to pin GPA4 (25) of the MCP23017.
const uint8_t LED5 = 5;      ///< Refers to pin GPA5 (26) of the MCP23017.
const uint8_t LED6 = 6;      ///< Refers to pin GPA6 (27) of the MCP23017.
const uint8_t LED7 = 7;      ///< Refers to pin GPA7 (28) of the MCP23017.
const uint8_t Switch0 = 8;   ///< Refers to pin GPB0 (1) of the MCP23017.
const uint8_t Switch1 = 9;   ///< Refers to pin GPB1 (2) of the MCP23017.
const uint8_t Switch2 = 10;  ///< Refers to pin GPB2 (3) of the MCP23017.
const uint8_t Switch3 = 11;  ///< Refers to pin GPB3 (4) of the MCP23017.
const uint8_t Switch4 = 12;  ///< Refers to pin GPB4 (5) of the MCP23017.
const uint8_t Switch5 = 13;  ///< Refers to pin GPB5 (6) of the MCP23017.
const uint8_t Switch6 = 14;  ///< Refers to pin GPB6 (7) of the MCP23017.
const uint8_t Switch7 = 15;  ///< Refers to pin GPB7 (8) of the MCP23017.


// Global Variables
volatile bool switchDidChange = false;  ///< The change status of the MCP23017 input pins.  The 'volatile' specifier is used since its value can be changed from within an interrupt service routine.


// Global Instances
MCP23017 mcp23017 = MCP23017(MCP23017_I2C_ADDRESS);  ///< The instance of the connected MCP23017 IC.


/**
 * Standard Arduino setup function used for setup and configuration tasks.
 */
void setup() {
   Wire.begin();     // initialize I2C serial bus
   mcp23017.init();  // initialize MCP23017 IC

   // Configure MCP23017 I/O pins
   // configurePinsWithPinMode();  // familiar pinMode() style
   configurePinsWithPortMode();  // concise portMode() style

   // Configure MCP23017 interrupts
   configureInterrupts();

   // Reset MCP23017 ports
   mcp23017.writeRegister(MCP23017Register::GPIO_A, 0x00);
   mcp23017.writeRegister(MCP23017Register::GPIO_B, 0x00);
}


/**
 * Standard Arduino loop function used for repeating tasks.
 */
void loop() {
   // Each function provides a different approach for accessing I/O pins
   // readAndWriteWithDigitalReadAndDigitalWrite();
   // portCopy();
   // readAndWritePortOnInputChange();
   readAndWritePinOnInputChange();
}


/**
 * Configures MCP23017 I/O pins using familiar pinMode() style.
 */
void configurePinsWithPinMode() {
   // Configure output pins
   mcp23017.pinMode(LED0, OUTPUT);
   mcp23017.pinMode(LED1, OUTPUT);
   mcp23017.pinMode(LED2, OUTPUT);
   mcp23017.pinMode(LED3, OUTPUT);
   mcp23017.pinMode(LED4, OUTPUT);
   mcp23017.pinMode(LED5, OUTPUT);
   mcp23017.pinMode(LED6, OUTPUT);
   mcp23017.pinMode(LED7, OUTPUT);

   // Configure input pins with internal 100K pull-up resistors
   // Third argument inverts the polarity of the input value when read
   mcp23017.pinMode(Switch0, INPUT_PULLUP, true);
   mcp23017.pinMode(Switch1, INPUT_PULLUP, true);
   mcp23017.pinMode(Switch2, INPUT_PULLUP, true);
   mcp23017.pinMode(Switch3, INPUT_PULLUP, true);
   mcp23017.pinMode(Switch4, INPUT_PULLUP, true);
   mcp23017.pinMode(Switch5, INPUT_PULLUP, true);
   mcp23017.pinMode(Switch6, INPUT_PULLUP, true);
   mcp23017.pinMode(Switch7, INPUT_PULLUP, true);
}


/**
 * Configures MCP23017 I/O pins using concise portMode() style.
 */
void configurePinsWithPortMode() {
   // Configure PORTA (LEDs)
   mcp23017.portMode(MCP23017Port::A,
      0b00000000);  // direction (IODIRA) - set direction of all pins as outputs

   // Configure PORTB (switches)
   mcp23017.portMode(MCP23017Port::B,
      0b11111111,   // direction (IODIRB) - set direction of all pins as inputs
      0b11111111,   // pull-up (GPPUB) - enable 100K pull-up resistors on all inputs
      0b11111111);  // polarity (IPOLB) - invert logic polarity for all inputs
}


/**
 * Configures and enables MCP23017 interrupts on PORTB.
 *
 * The mcp23017ChangeDetectedOnPortB() interrupt service routine is called when
 * an interrupt is triggered.
 */
void configureInterrupts() {
   // Configure MCP23017 interrupts
   mcp23017.interruptMode(MCP23017InterruptMode::Separated);  // INTA and INTB act independently
   mcp23017.interrupt(MCP23017Port::B, CHANGE);  // trigger an interrupt when an input pin CHANGE is detected on PORTB

   // Set up interrupt connection and attach interrupt service routine
   mcp23017.clearInterrupts();  // reset interrupt system
   pinMode(MCP23017_INTB, INPUT_PULLUP);  // utilize microprocessor's internal pull-up resistor
   attachInterrupt(digitalPinToInterrupt(MCP23017_INTB), mcp23017ChangeDetectedOnPortB, FALLING);  // INTB is active LOW
}


/**
 * Example code for reading and writing individual inputs and outputs.
 *
 * This approach should be the most familiar to Arduino users.  It uses the same
 * calling mechanism as Arduino's standard digitalRead() and digitalWrite()
 * functions, but may involve more operations than other approaches since only
 * one input or output at a time can be read or written.
 */
void readAndWriteWithDigitalReadAndDigitalWrite() {
   // Read and write individual inputs and outputs
   mcp23017.digitalWrite(LED0, mcp23017.digitalRead(Switch0));
   mcp23017.digitalWrite(LED1, mcp23017.digitalRead(Switch1));
   mcp23017.digitalWrite(LED2, mcp23017.digitalRead(Switch2));
   mcp23017.digitalWrite(LED3, mcp23017.digitalRead(Switch3));
   mcp23017.digitalWrite(LED4, mcp23017.digitalRead(Switch4));
   mcp23017.digitalWrite(LED5, mcp23017.digitalRead(Switch5));
   mcp23017.digitalWrite(LED6, mcp23017.digitalRead(Switch6));
   mcp23017.digitalWrite(LED7, mcp23017.digitalRead(Switch7));
}


/**
 * Example code for reading all inputs of a port in a single read and setting
 * all outputs of a port with a single write.
 *
 * This approach produces the most concise code, but does not indicate the
 * meaning of each of the inputs or outputs.
 */
void portCopy() {
   // Copy values from PORTB (switches) to PORTA (LEDs)
   mcp23017.writePort(MCP23017Port::A, mcp23017.readPort(MCP23017Port::B));
}


/**
 * Sets the switchDidChange flag when the MCP23017 INTB pin goes LOW.
 *
 * Invoked as an interrupt service routine (ISR).
 */
void mcp23017ChangeDetectedOnPortB() {
   switchDidChange = true;
}


/**
 * Example code using interrupts that determines when an input pin has changed
 * and then performs a simple copy from PORTB (inputs) to PORTA (outputs).
 *
 * This approach should be used when inputs are not expected to change rapidly
 * and a full port copy will suffice.  It allows the microcontroller to spend
 * more time on other tasks.
 */
void readAndWritePortOnInputChange() {
   if (switchDidChange) {
      delay(100);  // allow time for MCP23017 to set interrupt registers
      portCopy();  // copy values from PORTB (switches) to PORTA (LEDs)
      mcp23017.clearInterrupts();  // clear interrupt
      switchDidChange = false;
   }
}


/**
 * Example code using interrupts that determines when an input pin has changed,
 * captures which pin caused the interrupt along with its associated value,
 * and then updates the appropriate output pin with the new value.
 *
 * This approach should be used when inputs are not expected to change rapidly
 * and the changed pin's port position and value are desired.  It allows the
 * microcontroller to spend more time on other tasks.
 *
 * Note: PORTA interrupt registers are ignored since we are only interested in
 * PORTB inputs.
 */
void readAndWritePinOnInputChange() {
   uint8_t flagA, flagB;  // MCP23017 INTFA/B registers
   uint8_t capA, capB;    // MCP23017 INTCAPA/B registers

   if (switchDidChange) {
      delay(100);  // allow time for MCP23017 to set interrupt registers
      mcp23017.interruptedBy(flagA, flagB);  // retrieve pin causing interrupt
      mcp23017.clearInterrupts(capA, capB);  // clear interrupt and capture pin states at time of interrupt
      uint8_t pin = 0;  // input pin causing interrupt
      for (pin = 0; pin < 8; pin++) {
         if (bitRead(flagB, pin)) break;
      }
      uint8_t value = bitRead(capB, pin);  // new value of input pin causing interrupt
      mcp23017.digitalWrite(pin, value);  // set appropriate LED with new value
      switchDidChange = false;
   }
}
