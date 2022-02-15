
#include <Adafruit_HMC5883_U.h>

#include "sensesp/sensors/sensor.h"
#include "sensesp/transforms/angle_correction.h"
#include "sensesp_app_builder.h"
#include "sensesp_nmea0183/wiring.h"

using namespace sensesp;

ReactESP app;

constexpr int kGNSSBitRate = 9600;
constexpr int kGNSSRxPin = 15;
// set the Tx pin to -1 if you don't want to use it
constexpr int kGNSSTxPin = 13;

// SH-ESP32 default SDA and SCL pins
constexpr int kSDAPin = 16;
constexpr int kSCLPin = 17;

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup() {
  SensESPAppBuilder builder;
  sensesp_app = builder.set_hostname("sensesp-bn880")
                    ->enable_ota("thisisfine")
                    ->get_app();

#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  // initialize the I2C bus
  Wire.begin(kSDAPin, kSCLPin);

  HardwareSerial* serial = &Serial1;
  serial->begin(kGNSSBitRate, SERIAL_8N1, kGNSSRxPin, kGNSSTxPin);

  NMEA0183Input* nmea_input = new NMEA0183Input(serial);
  ConnectLocationSKOutputs(nmea_input);

  debugD("Initializing HM5883...");
  // Initialize the magnetometer. The call always returns true,
  // so we can't use the return value to indicate hardware presence.
  mag.begin();

  auto heading_magnetic = new RepeatSensor<float>(1000, []() -> float {
    sensors_event_t event;
    mag.getEvent(&event);
    float heading = atan2(event.magnetic.y, event.magnetic.x);
    return heading;
  });
  auto hdg_correction =
      new AngleCorrection(0, 0, "/Compass/Heading Correction");
  auto hdg_mag_output = new SKOutputFloat("navigation.headingMagnetic",
                                          "/SK Path/Heading (Magnetic)");
  heading_magnetic->connect_to(hdg_correction)->connect_to(hdg_mag_output);

  sensesp_app->start();
}

void loop() { app.tick(); }
