#include <Arduino.h>
#include "driver/mcpwm.h"
#include "math.h"

// PWM Outputs
#define PWM0A_OUT 12  
#define PWM0B_OUT 14

// ADC pin
#define ADC_PIN 25

// Control variables
double Vref = 0.0;
double inputVoltage = 0.0;
double Duty = 0.0;
float Vread_adc = 0.0;
double last_error = 0.0 , last_last_error = 0.0;

// PID coefficients
double Kp = 1;
double Ki = 12.0;
double Kd = 0.0;

unsigned timee = 0;

mcpwm_config_t pwm_config;

void setup() {

  Serial.begin(115200);

  pinMode(PWM0A_OUT , OUTPUT);
  pinMode(PWM0B_OUT , OUTPUT);
  pinMode(ADC_PIN , INPUT);
  
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PWM0A_OUT);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, PWM0B_OUT);

  // MCPWM configuration
  pwm_config.frequency = 50000; 
  pwm_config.cmpr_a = 50;
  pwm_config.cmpr_b = 50;
  pwm_config.counter_mode = MCPWM_UP_DOWN_COUNTER;  
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

  // Dead time(ns)
  mcpwm_deadtime_enable(
    MCPWM_UNIT_0,
    MCPWM_TIMER_0,
    MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE,
    50, 50
  );
 
  Serial.println("Enter the desired output voltage:");

}

void loop() {

  delay(500);

  if(Serial.available() > 0){
    inputVoltage = Serial.parseFloat();
    Serial.println(inputVoltage);
    Serial.println("Enter the Voltage you want: ");
  }
  // PID loop every 5ms
  if(millis() - timee > 5){
    float read = analogRead(25);
    Vread_adc =  (read * 3.3) / 4095;
    float Vfit_adc = (-0.03*pow(Vread_adc , 3)) + (0.0742 * pow(Vread_adc , 2)) + (1.0191 * Vread_adc) + 0.1038;
    Serial.print("read voltage: ");
    Serial.println(Vread_adc);
    // Voltage after divider by resistor
    Vref = 8 * 0.1515;         
    Duty += (Kp*((Vref - Vread_adc) - last_error)) + (Ki*(Vref - Vread_adc)) + (Kd*(Vref - Vread_adc - (2*last_error) + last_last_error));
    Serial.print("Vref:");
    Serial.println(Vref);
    

    // Duty saturation
    if(Duty > 75){
      Duty = 75;
    }
    else if(Duty < 30){
      Duty = 30;
    }
    Serial.print("Duty :");
    Serial.println(Duty);
    pwm_config.cmpr_a = Duty;
    pwm_config.cmpr_b = Duty;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    timee = millis();
    last_error = Vref - Vfit_adc;
    last_last_error = last_error;
  }
  
}

