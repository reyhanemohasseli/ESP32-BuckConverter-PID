*ESP32 Buck Converter PID Control*:
A closed-loop PID controller for a DC-DC Buck Converter implemented on the ESP32 using the MCPWM peripheral and ADC voltage feedback.
Hardware Components: The following components were used to build and test the buck converter system:
 .ESP32 Development Board – main microcontroller for PWM generation and control algorithm
 .IR2113 MOSFET Driver – high and low side gate driver for switching MOSFETs for synchronous rectification purpse
 .N‑Channel MOSFETs – IRF3205
 .Inductor (Power Inductor) – energy storage element of the converter
 .Output Capacitor – smoothing the output voltage ripple
 .Voltage Divider Resistors – scaling output voltage for ESP32 ADC input
 .DC Power Supply – input power source
 .Load (Resistive Load) – to test voltage regulation

The ESP32 generates complementary PWM signals to drive the buck converter. The output voltage is measured using the ADC through a voltage divider.
A PID controller compares the measured voltage with the desired reference voltage and continuously adjusts the PWM duty cycle to maintain a stable 
output voltage.
