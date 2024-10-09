#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_LSM6DS33.h>
#include <MadgwickAHRS.h>

#define SENSOR_RATE 104.00
#define DEAD_ZONE 0.2
#define GYRO_SMOOTHING 0.0
#define ROLL_SMOOTHING 0.0
#define GYRO_RATE 60 // per second

Adafruit_LSM6DS3TRC lsm6ds3trc; // accelerometer, gyroscope
Adafruit_LSM6DS33 lsm6ds33;
Madgwick filter;

bool new_rev = true;
long int accel_array[6];
long int check_array[6]={0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

uint32_t lastGyro;
float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float currentRoll = 0;
float rollFilteredOld;
float smoothedGyro = 0;

void setup() {
  Serial.begin(115200);
  filter.begin(SENSOR_RATE);
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

void loop() {
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
  accel_x = accel.acceleration.x;
  accel_y = accel.acceleration.y;
  accel_z = accel.acceleration.z;
  gyro_x = gyro.gyro.x;
  gyro_y = gyro.gyro.y;
  gyro_z = gyro.gyro.z;

  filter.updateIMU(gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z);
  roll = filter.getRoll();
  float rollFiltered = (1.0 - ROLL_SMOOTHING) * roll + ROLL_SMOOTHING * rollFilteredOld; // low pass filter
  rollFilteredOld = rollFiltered;

  if (abs(gyro_x) < DEAD_ZONE) {
    gyro_x = 0;
  }
  smoothedGyro = (1.0 - GYRO_SMOOTHING) * gyro_x + GYRO_SMOOTHING * smoothedGyro;

  if (millis() - lastGyro >= 1000 / GYRO_RATE) {
    currentRoll += smoothedGyro;
    // Serial.println(currentRoll);
  }
  // Serial.print(accel_x);
  // Serial.print(" ");
  // Serial.println(accel_y);
  // Serial.print(" ");
  // Serial.println(accel_z);
  // Serial.print("Gyro: ");
  // Serial.println(gyro_x);
  // Serial.print(" ");
  // Serial.print(gyro_y);
  // Serial.print(" ");
  // Serial.println(gyro_z);

  Serial.println(roll);

}
