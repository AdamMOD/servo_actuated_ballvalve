#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_ADS1115 ads;   

float time_zero_seconds;

enum valve{regulator_1=1};
#define USMIN  900 // This is the rounded 'minimum' microsecond length
#define USMAX  2100 // This is the rounded 'maximum' microsecond length
#define SERVO_FREQ 330
#define REG1_US_CLOSED 1800
#define REG1_US_OPEN 900

void commandServo(float open_fraction, int valve)
{
  switch(valve)
  {
    float command_microseconds;
    case 1:
      command_microseconds = map(open_fraction, 0, 1, REG1_US_OPEN, REG1_US_CLOSED);
      if(command_microseconds >= REG1_US_OPEN && command_microseconds <= REG1_US_CLOSED){
        pwm.writeMicroseconds(1, command_microseconds);
      }
      else{
        Serial.print("Command out of range!");
      }
      break;
    default:
      Serial.print("Unknown servo commanded.");
  }
}

/*
float readADCDegrees(int joint)
{
  switch(joint)
  {
    case 1:
      return map((float) ads.readADC_SingleEnded(1), LEFT_HIP_ADC_READ_CALC_LOW, LEFT_HIP_ADC_READ_CALC_HIGH, LEFT_HIP_ANGLE_COMMAND_CALC_LOW, LEFT_HIP_ANGLE_COMMAND_CALC_HIGH);
      break;
    case 3:
      return map((float) ads.readADC_SingleEnded(3), LEFT_KNEE_ADC_READ_CALC_LOW, LEFT_KNEE_ADC_READ_CALC_HIGH, LEFT_KNEE_ANGLE_COMMAND_CALC_LOW, LEFT_KNEE_ANGLE_COMMAND_CALC_HIGH);
      break;
    default:
      Serial.print("Unknown joint read.");
  }
}
*/

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void setup() {
  Serial.begin(9600);
  Serial.print("Time,Command,Reading\n");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();
  ads.setGain(GAIN_TWOTHIRDS);
  ads.setDataRate(RATE_ADS1115_860SPS);

  
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  
  time_zero_seconds = ((float) millis()) / 1000.0;
}

void loop() {
  
  int16_t adc;
  float  volts;
  float time_seconds;
  float command_us;
  float command;
  volts = ads.computeVolts(0);


  time_seconds = ((float) millis()) / 1000.0 - time_zero_seconds;
  // sin command
  command_us = 1350 + 45 * sin(time_seconds);
  //square command
  //command_us = 1333 + 400 * sgn(sin(time_seconds));
  command = .5; //+ .5 * sin(time_seconds);
  //adc = ads.readADC_SingleEnded(1);
  //pwm.writeMicroseconds(1, command_us);
  commandServo(command, regulator_1);
  //pwm.writeMicroseconds(1, command_us);
  
  Serial.print(time_seconds, 4); Serial.print(",");
  //Serial.print(command_us); Serial.print(","); 
  //Serial.print(adc); Serial.print("\n");
  Serial.print(command); Serial.print(",");
  Serial.print("  "); Serial.print(volts); Serial.println("V");
  
  delay(10);
  

}
