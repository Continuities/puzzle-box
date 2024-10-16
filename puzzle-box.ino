#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_LSM6DS33.h>

#define SQUARE_BUTTON_PIN 12
#define SILVER_BUTTON_PIN 11
#define RED_SWITCH_PIN 10
#define GREEN_SWITCH_PIN 9
#define BLUE_SWITCH_PIN 6

struct ButtonState {
  uint32_t pin;
  bool pushed;
  bool pushed_this_frame;
};

struct SwitchState {
  uint32_t pin;
  bool switched_on;
};

Adafruit_LSM6DS3TRC lsm6ds3trc; // accelerometer, gyroscope
Adafruit_LSM6DS33 lsm6ds33;

bool new_rev = true;
long int accel_array[6];
long int check_array[6]={0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

// inputs
ButtonState square_button = { SQUARE_BUTTON_PIN, false, false };
ButtonState silver_button = { SILVER_BUTTON_PIN, false, false };
SwitchState red_switch = { RED_SWITCH_PIN, false };
SwitchState green_switch = { GREEN_SWITCH_PIN, false };
SwitchState blue_switch = { BLUE_SWITCH_PIN, false };

float accelX,            accelY,             accelZ,            // units m/s/s i.e. accelZ if often 9.8 (gravity)
      gyroX,             gyroY,              gyroZ,             // units dps (degrees per second)
      gyroDriftX,        gyroDriftY,         gyroDriftZ,        // units dps
      gyroRoll,          gyroPitch,          gyroYaw,           // units degrees (expect major drift)
      gyroCorrectedRoll, gyroCorrectedPitch, gyroCorrectedYaw,  // units degrees (expect minor drift)
      accRoll,           accPitch,           accYaw,            // units degrees (roll and pitch noisy, yaw not possible)
      complementaryRoll, complementaryPitch, complementaryYaw;  // units degrees (excellent roll, pitch, yaw minor drift)

long lastTime;
long lastInterval;

void initIMU() {
  lsm6ds33.begin_I2C();
  // check for readings from LSM6DS33
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds33.getEvent(&accel, &gyro, &temp);
  accel_array[0] = accel.acceleration.x;
  accel_array[1] = accel.acceleration.y;
  accel_array[2] = accel.acceleration.z;
  accel_array[3] = gyro.gyro.x;
  accel_array[4] = gyro.gyro.y;
  accel_array[5] = gyro.gyro.z;
  // if all readings are empty, then new rev
  for (int i =0; i < 5; i++) {
    if (accel_array[i] != check_array[i]) {
      new_rev = false;
      break;
    }
  }
  // and we need to instantiate the LSM6DS3TRC
  if (new_rev) {
    lsm6ds3trc.begin_I2C();
  }
}

/*
  the gyro's x,y,z values drift by a steady amount. if we measure this when arduino is still
  we can correct the drift when doing real measurements later
*/
void calibrateIMU(int delayMillis, int calibrationMillis) {

  int calibrationCount = 0;

  delay(delayMillis); // to avoid shakes after pressing reset button

  float sumX, sumY, sumZ;
  int startTime = millis();
  while (millis() < startTime + calibrationMillis) {
    if (readIMU()) {
      // in an ideal world gyroX/Y/Z == 0, anything higher or lower represents drift
      sumX += gyroX;
      sumY += gyroY;
      sumZ += gyroZ;

      calibrationCount++;
    }
  }

  if (calibrationCount == 0) {
    Serial.println("Failed to calibrate");
  }

  gyroDriftX = sumX / calibrationCount;
  gyroDriftY = sumY / calibrationCount;
  gyroDriftZ = sumZ / calibrationCount;

}

/**
   Read accel and gyro data.
   returns true if value is 'new' and false if IMU is returning old cached data
*/
bool readIMU() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  float roll, pitch, heading;
  if (new_rev) {
    lsm6ds3trc.getEvent(&accel, &gyro, &temp);
  }
  else {
    lsm6ds33.getEvent(&accel, &gyro, &temp);
  }
  accelX = accel.acceleration.x;
  accelY = accel.acceleration.y;
  accelZ = accel.acceleration.z;
  gyroX = gyro.gyro.x;
  gyroY = gyro.gyro.y;
  gyroZ = gyro.gyro.z;
  return true;
}

/**
   I'm expecting, over time, the Arduino_LSM6DS3.h will add functions to do most of this,
   but as of 1.0.0 this was missing.
*/
void doCalculations() {
  accRoll = atan2(accelY, accelZ) * 180 / M_PI;
  accPitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / M_PI;

  float lastFrequency = (float) 1000000.0 / lastInterval;
  gyroRoll = gyroRoll + (gyroX / lastFrequency);
  gyroPitch = gyroPitch + (gyroY / lastFrequency);
  gyroYaw = gyroYaw + (gyroZ / lastFrequency);

  gyroCorrectedRoll = gyroCorrectedRoll + ((gyroX - gyroDriftX) / lastFrequency);
  gyroCorrectedPitch = gyroCorrectedPitch + ((gyroY - gyroDriftY) / lastFrequency);
  gyroCorrectedYaw = gyroCorrectedYaw + ((gyroZ - gyroDriftZ) / lastFrequency);

  complementaryRoll = complementaryRoll + ((gyroX - gyroDriftX) / lastFrequency);
  complementaryPitch = complementaryPitch + ((gyroY - gyroDriftY) / lastFrequency);
  complementaryYaw = complementaryYaw + ((gyroZ - gyroDriftZ) / lastFrequency);

  complementaryRoll = 0.98 * complementaryRoll + 0.02 * accRoll;
  complementaryPitch = 0.98 * complementaryPitch + 0.02 * accPitch;
}

/**
   This comma separated format is best 'viewed' using 'serial plotter' or processing.org client (see ./processing/RollPitchYaw3d.pde example)
*/
void printIMUCalculations() {
  Serial.print("complementaryRoll:");
  Serial.print(complementaryRoll);
  Serial.println("");
}

void initInputs() {
  pinMode(square_button.pin, INPUT_PULLUP);
  pinMode(silver_button.pin, INPUT_PULLUP);
  pinMode(red_switch.pin, INPUT_PULLUP);
  pinMode(green_switch.pin, INPUT_PULLUP);
  pinMode(blue_switch.pin, INPUT_PULLUP);
}

void readButton(ButtonState *button_state) {
  bool pushed = digitalRead(button_state->pin) == LOW;
  button_state->pushed_this_frame = pushed && !button_state->pushed;
  button_state->pushed = pushed;
}

void readButtons() {
  readButton(&square_button);
  readButton(&silver_button);
}

void readSwitch(SwitchState *switch_state) {
  switch_state->switched_on = digitalRead(switch_state->pin) == LOW;
}

void readSwitches() {
  readSwitch(&red_switch);
  readSwitch(&green_switch);
  readSwitch(&blue_switch);
}

void printButtons() {
  if (square_button.pushed_this_frame) {
    Serial.println("square_button pushed");
  }
  if (silver_button.pushed_this_frame) {
    Serial.println("silver_button pushed");
  }
}

void debugButtons() {
  Serial.print("square: ");
  Serial.print(digitalRead(SQUARE_BUTTON_PIN));
  Serial.print(" silver: ");
  Serial.println(digitalRead(SILVER_BUTTON_PIN));
}

void debugSwitches() {
  Serial.print("RGB: ");
  Serial.print(red_switch.switched_on);
  Serial.print(green_switch.switched_on);
  Serial.print(blue_switch.switched_on);
  Serial.println("");
}

void setup() {
  Serial.begin(115200);

  initInputs();
  initIMU();
  calibrateIMU(250, 250);

  lastTime = micros();
}

void loop() {
  if (readIMU()) {
    long currentTime = micros();
    lastInterval = currentTime - lastTime; // expecting this to be ~104Hz +- 4%
    lastTime = currentTime;

    doCalculations();
    // printIMUCalculations();
  }
  readButtons();
  readSwitches();
  printButtons();
  // debugButtons();
  debugSwitches();
}
