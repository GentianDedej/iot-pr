*/
// C++ code
//

#define H HIGH
#define L LOW


// PHOTORESISTOR CONFIGURATION:
const int photoresistor_input = A3;
const float swap_limit_light_value = 600.0;
bool isNight = false;

// var for debug purpose:
bool lastLumState = false;

// ACCELEROMETER CONFIGURATION:

#define NOTHING -1
#define X  0
#define Y  1
#define Z  2

struct acm_t {
  float axeValue;
  int calibration[3] = {511, 511, 511}; //setup  accelorometer calibration
  const int accelerometer_axespin[3] = {A0, A1, A2};// X, Y, Z
} acm = {0};
// Volts per G-Force
float sensitivity = 0.206;

const int ACCELERATOR_MARGIN_ERROR = 0.3f;


// LED CONFIGURATION:
const int MATRIX_SIDE_SIZE = 4;

// 2-dimentional array of raw pin numbers:
const int row[MATRIX_SIDE_SIZE] = { 9, 8, 7, 6 };
const int rowLen = MATRIX_SIDE_SIZE;

// 2-dimentional array of column pin numbers:
const int col[MATRIX_SIDE_SIZE] = { 5, 4, 3, 2 };
const int colLen = MATRIX_SIDE_SIZE;

// 2-dimensional array of pixels:
int pixels[MATRIX_SIDE_SIZE][MATRIX_SIDE_SIZE];

// 3-dimensional patterns array:
const int patternsNumber = 9;
const int patternsArray[patternsNumber][MATRIX_SIDE_SIZE][MATRIX_SIDE_SIZE] = {
  // up arrow pattern:
  {
    {H, L, L, H},
    {H, L, L, H},
    {L, H, H, L},
    {L, H, H, L}
  },
  // down arrow pattern:
  {
    {L, H, H, L},
    {L, H, H, L},
    {H, L, L, H},
    {H, L, L, H}
  },
  // left arrow pattern:
  {
    {H, H, L, L},
    {L, L, H, H},
    {L, L, H, H},
    {H, H, L, L}
  },
  // right arrow pattern:
  {
    {L, L, H, H},
    {H, H, L, L},
    {H, H, L, L},
    {L, L, H, H}
  },
  // :) pattern:
  {
    {L, H, H, L},
    {H, H, H, H},
    {L, H, H, L},
    {H, L, L, H}
  },
};
int patternIndex = 0;

enum {UP = 0, DW = 1, LF = 2, RG = 3, SMY = 4};
const int patternMovement[4] = {UP, DW, LF, RG};

// cursor position:
int x = 0;
int y = 0;

// global variable for elapsedTime calculation:
unsigned long startTime;

void setup()
{
  Serial.begin(9600); // /!\ only to printing purpose

  // PHOTOESISTOR CONF:

  // ACCELEROMETER CONF:
  analogReference(EXTERNAL); // tinkercad calibration
  acm.calibration[X] = analogRead(acm.accelerometer_axespin[X]);
  acm.calibration[Y] = analogRead(acm.accelerometer_axespin[Y]);
  acm.calibration[Z] = analogRead(acm.accelerometer_axespin[Z]);

  // LED CONF:
  for (int i = 0; i < MATRIX_SIDE_SIZE; ++i)
  {
    // initialize the output pins:
    pinMode(col[i], OUTPUT);
    pinMode(row[i], OUTPUT);
    // take the col pins (i.e. the cathodes) high to ensure that the LEDS are off:
    digitalWrite(col[i], HIGH);
  }

  // initialize the pixel matrix:
  for (int x = 0; x < MATRIX_SIDE_SIZE; ++x)
  {
    for (int y = 0; y < MATRIX_SIDE_SIZE; ++y)
    {
      pixels[x][y] = HIGH;
    }
  }
  
  // initialize time
  startTime = millis();
}

void loop()
{
  // read input:
  readSensors();

  if (isNight == true) {
    //swapDrawing();
    
    // draw the screen:
    refreshScreen();
  }
  delay(100);
}
/*
unsigned long getTimeElapsed() {
	return millis() - startTime;
}

void resetTime() {
  startTime = millis();
}

void swapDrawing() {
  unsigned long elapsedTime = getTimeElapsed();

  if (elapsedTime > 300) {
    resetTime();
    patternIndex = (patternIndex + 1) % patternsNumber;
  }
}
*/
void checkLuminosity() {
  int luminosity = analogRead(photoresistor_input);

  if (luminosity < swap_limit_light_value) {
    isNight = true;
    
    if (lastLumState != isNight) {
    	Serial.println("NIGHT");
      	lastLumState = isNight;
    }
  } else {
    isNight = false;
    if (lastLumState != isNight) {
    	Serial.println("DAY");
      	lastLumState = isNight;
    }
  }
}

int checkAcceleration() {
  int greatestAxe = NOTHING;

  float greatestAxeVal = 0.0;
  float axe;

  for (int i = 0; i < 3; ++i) {
    axe = (analogRead(acm.accelerometer_axespin[i]) - acm.calibration[i]) * 3.3 / (sensitivity * 1023);

    acm.axeValue = axe;
    if (axe < 0.0) axe = axe -1.0;
    if (axe > ACCELERATOR_MARGIN_ERROR && greatestAxeVal < axe) {
      greatestAxe = i;
      greatestAxeVal = axe;
    }
  }

  return greatestAxe;
}

void readSensors()
{
  checkLuminosity();

  if (isNight == true) {
    int movement;

    movement = checkAcceleration();
    if (movement != NOTHING) {
      switch (movement)
      {
      case Y:
        movement = acm.axeValue < 0.0 ? RG : LF;
      case Z:
        movement = acm.axeValue < 0.0 ? UP : DW;
        break;
      default:
        movement = SMY;
        break;
      }
      patternIndex = patternMovement[movement];
    } else {
      patternIndex = SMY;
    }
  }
}

void refreshScreen()
{

  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < rowLen; ++thisRow)
  {
    // take the row pin (node) high:
    digitalWrite(row[thisRow], HIGH);
    // iterate over the cols (cathodes):
    for (int thisCol = 0; thisCol < colLen; ++thisCol)
    {
      // get the state of the curent pixel:
      //int thisPixel = pixels[thisRow][thisCol];

      int thisPixel = patternsArray[patternIndex][thisRow][thisCol];
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      if (thisPixel == LOW)
      {
        digitalWrite(col[thisCol], HIGH);
      }
    }
  //delay(2000);
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], LOW);
  }
}