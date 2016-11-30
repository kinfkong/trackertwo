//defines
#define FILENAME "trackertwo.cpp"
#define MYVERSION "0.04.0"
#define MYBUILD "1128.1712"

//declarations
// Set whether you want the device to publish data to the internet by default here.
  // 1 will Particle.publish AND Serial.print, 0 will just Serial.print
  // Extremely useful for saving data while developing close enough to have a cable plugged in.
  // You can also change this remotely using the Particle.function "tmode" defined in setup()
int transmittingData = 1;

// Used to keep track of the last time we published data
long lastPublish = 0;

// How many minutes minimum between publishes? 10+ recommended!
int delayMinutes = 2;

// Threshold to trigger a publish
// 9000 is VERY sensitive, 12000 will still detect small bumps
int accelThreshold = 12000;

unsigned long lastTime = 0;

bool gpsserialdebug = true;
bool mydebug = true;

int serial1Avail = 0;

int gpsloctime = 0;

http_header_t headers[] = {
      { "Content-Type", "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t request;
http_response_t response;

// Creating an AssetTracker named 't' for us to reference
AssetTracker t = AssetTracker();

 HttpClient http;

// A FuelGauge named 'fuel' for checking on the battery state
FuelGauge fuel;

// Prototypes
 int transmitMode(String command);
 int gpsPublish(String command);

#ifndef __PAYLOAD_BUILDER__
#define __PAYLOAD_BUILDER__

class PayloadBuilder {
public:
    PayloadBuilder() {
        _jsonObj = &_jsonBuffer.createObject();
    }

    String toString() {
        _jsonObj->printTo(_buf, sizeof(_buf));
        return String(_buf);
    }

    template <typename T>
    void addKeyValue(const char* key, T value) {
        (*_jsonObj)[key] = value;
    }

private:
    char _buf[4096];
    JsonObject* _jsonObj;
    StaticJsonBuffer<4096> _jsonBuffer;
};

#endif
