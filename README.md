<a href="https://www.woolseyworkshop.com"><img src="https://www.woolseyworkshop.com/wp-content/uploads/WWSLogoTitleLines.png" alt="Woolsey Workshop" height="70"></a>

# Adding Digital I/O To Your Arduino Article Resources
This repository contains resources related to the [Adding Digital I/O To Your Arduino](https://www.woolseyworkshop.com/2021/02/04/adding-digital-io-to-your-arduino-part-1-the-74hc595/) article published on [WoolseyWorkshop.com](https://www.woolseyworkshop.com). It is a three-part tutorial describing how to add more digital inputs and outputs to your Arduino board.

## [Part 1 - The 74HC595](https://www.woolseyworkshop.com/2021/02/04/adding-digital-io-to-your-arduino-part-1-the-74hc595/)
Teaches you how to add digital outputs using the 74HC595 8-bit serial-in parallel-out (SIPO) shift register IC.

### [OutputShiftRegister Sketch](OutputShiftRegister)
An Arduino sketch that interfaces a 74HC595 serial-in parallel-out shift register IC to add digital outputs to an Arduino board.

### [Arduino_74HC595 Schematic](Arduino_74HC595)
A KiCad schematic for a 74HC595 serial-in parallel-out shift register IC, with LEDs attached to the outputs, connected to an Arduino Uno.

## [Part 2 - The 74HC165](https://www.woolseyworkshop.com/2021/02/18/adding-digital-io-to-your-arduino-part-2-the-74hc165/)
Teaches you how to add digital inputs using the 74HC165 8-bit parallel-in serial-out (PISO) shift register IC.

### [InputShiftRegister Sketch](InputShiftRegister)
An Arduino sketch that interfaces a 74HC165 parallel-in serial-out shift register IC to add digital inputs to an Arduino board.

### [Arduino_74HC165 Schematic](Arduino_74HC165)
A KiCad schematic for a 74HC165 parallel-in serial-out shift register IC, with an 8-position dip switch attached to the inputs, connected to an Arduino Uno.

## [Part 3 - The MCP23017](https://www.woolseyworkshop.com/2021/03/18/adding-digital-io-to-your-arduino-part-3-the-mcp23017/)
Teaches you how to add both digital inputs and outputs using the MCP23017 16-Bit I2C I/O Expander With Serial Interface IC.

### [PortExpander Sketch](PortExpander)
An Arduino sketch that interfaces an MCP23017 16-Bit I2C I/O Expander With Serial Interface IC to add digital inputs and outputs to an Arduino board.

### [Arduino_MCP23017 Schematic](Arduino_MCP23017)
A KiCad schematic for an MCP23017 16-Bit I2C I/O Expander With Serial Interface IC, with an 8-position dip switch and 8 LEDs attached to its GPIO pins, connected to an Arduino Uno.
