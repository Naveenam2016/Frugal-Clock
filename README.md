# Frugal-Clock
## A digital clock with only two digits

As the name suggests, this clock aims to be frugal by doing away with some things that a standard clock has - like half a display, and a built in timekeeper.

The clock still functions like any other clock. It does this by alternating the hour and minute on the half-display it has, and it gets the time from an external source - a web-service.

The clock is built around the ESP8266-based Wemos D1 mini. The Wemos needs to be wired up as shown in the wiring diagram below. It needs to be flashed with the frugal_clock.ino program.


![Frugal Clock Wiring Diagram](https://github.com/ajithvasudevan/Frugal-Clock/raw/master/Frugal%20Clock%20-%20Wiring%20Diagram.png "Frugal Clock - Wiring Diagram")

## Notes:

* Connections from D0 to D8 on the Wemos D1 mini to their corresponding transistors are omitted in the diagram for clarity. Similarly marked points need to be connected together

* All Transistor, except T1, T2 are NPN type  – Part No. BC 547
 
* T1, T2 are PNP type Transistors                  – Part No. BC 557 

* All Resistors are 4.7 kΩ, unless specified in the diagram

* The 7-Segment LED Displays ( https://en.wikipedia.org/wiki/Seven-segment_display  ) used here are “Common Anode (+ve)” type, and work with 5V.

* The cathodes (-ve) of the segments of one display are connected to the corresponding ones on the other, i.e., cathode of segment ‘A’ of both displays are connected together, cathode of segment ‘B’ are connected together, and so on.

* The Decimal Point (dp) of the display on the right (units place) is used to indicate that the currently displayed number is “hour”

* The Decimal Point (dp) of the display on the left (tens place) is not used, and not connected. 
