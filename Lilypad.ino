//START LIGHT PART
/******************************************************************************

LilyPad Light Sensor Example
SparkFun Electronics

This example code reads the input from a LilyPad Light Sensor and displays in
the Serial Monitor.

Light Sensor connections:
   * S pin to A2
   * + pin to "NEED A CONTROLLER"
   * - to -

******************************************************************************/

// Set which pin the Signal output from the light sensor is connected to
// If using the LilyPad Development Board, change this to A6
int sensorPin = A2;
// Create a variable to hold the light reading
int lightValue;
//END LIGHT PART



//START ACCELOMETRO PART
// these constants describe the pins. They won't change:
const int xpin = A5;                  // x-axis of the accelerometer
const int ypin = A4;                  // y-axis
const int zpin = A3;                  // z-axis (only on 3-axis models)
//
int sampleDelay = 500;   //number of milliseconds between readings
//END ACCELOMETRO PART



void setup()
{
    // Set sensorPin as an INPUT
    pinMode(sensorPin, INPUT);

    // Set pin A5 to use as a power pin for the light sensor
    // If using the LilyPad Development Board, comment out these lines of code
    pinMode("NEED A CONTROLLER", OUTPUT);
    digitalWrite("NEED A CONTROLLER", HIGH);

    // Initialize Serial, set the baud rate to 9600 bps.
    Serial.begin(9600);
}

void loop()
{
    //START LIGHT PART
   // Get the current light level
    lightValue = analogRead(sensorPin);

   // Print some descriptive text and then the value from the sensor
    Serial.print("Light value is:");
    Serial.println(lightValue);

    // Delay so that the text doesn't scroll too fast on the Serial Monitor. 
    // Adjust to a larger number for a slower scroll.
    delay(200);
    //END LIGHT PART
    
    
    
    //START ACCELOMETRO PART
    int x = analogRead(xpin);
     //
     //add a small delay between pin readings.  I read that you should
     //do this but haven't tested the importance
       delay(1); 
     //
     int y = analogRead(ypin);
     //
     //add a small delay between pin readings.  I read that you should
     //do this but haven't tested the importance
       delay(1); 
     //
     int z = analogRead(zpin);
     //
     //zero_G is the reading we expect from the sensor when it detects
     //no acceleration.  Subtract this value from the sensor reading to
     //get a shifted sensor reading.
     float zero_G =512; 
     //
     //scale is the number of units we expect the sensor reading to
     //change when the acceleration along an axis changes by 1G.
     //Divide the shifted sensor reading by scale to get acceleration in Gs.
     float scale =102.3;
     //
     Serial.print(((float)x - zero_G)/scale);
     Serial.print("\t");
     //
     Serial.print(((float)y - zero_G)/scale);
     Serial.print("\t");
     //
     Serial.print(((float)z - zero_G)/scale);
     Serial.print("\n");
     //
     // delay before next reading:
     delay(sampleDelay);
    //END ACCELOMETRO PART
    
}
