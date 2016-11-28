/* -----------------------------------------------------------
This example shows a lot of different features. As configured here
it'll check for a good GPS fix every 10 minutes and publish that data
if there is one. If not, it'll save you data by staying quiet. It also
registers 3 Particle.functions for changing whether it publishes,
reading the battery level, and manually requesting a GPS reading.
---------------------------------------------------------------*/

// Getting the library
#include "lib/AssetTracker/firmware/AssetTracker.h"
#include "trackertwo.h"


void setup() {
    t.begin();

    // Enable the GPS module. Defaults to off to save power.
    // Takes 1.5s or so because of delays.
    t.gpsOn();

    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);
    Serial1.begin(9600);

    // These three functions are useful for remote diagnostics. Read more below.
    Particle.function("tmode", transmitMode);
    Particle.function("gps", gpsPublish);
    Particle.function("aThresh",accelThresholder);
}

// loop() runs continuously
void loop() {
    // You'll need to run this every loop to capture the GPS output
    t.updateGPS();
    // digitalWrite(D7, LOW);  //turn off built in led

        // Check if there's been a big acceleration

    /*
    if(t.readXYZmagnitude() > accelThreshold ){
        // Create a nice string with commas between x,y,z
        String pubAccel = String::format("%d,%d,%d",t.readX(),t.readY(),t.readZ());


        // Send that acceleration to the serial port where it can be read by USB
        Serial.print("pubAccel"); Serial.println(pubAccel);
        Serial.println(t.readXYZmagnitude());
       // digitalWrite(D7, HIGH);  //tunr on built in led
        Serial.println("  x  y  z ");
        Serial.print("  ");
        Serial.print(t.readX());  Serial.print(" ");
        Serial.print(t.readY());  Serial.print(" ");
        Serial.print(t.readZ());  Serial.print(" ");

        // If it's set to transmit AND it's been at least delayMinutes since the last one...
        if(transmittingData && ((millis()-lastPublish) > (delayMinutes*60*1000))){
            lastPublish = millis();
            Particle.publish("A", pubAccel, 60, PRIVATE);
        }

    }  */

    // if the current time - the last time we published is greater than your set delay...
    if(millis()-lastPublish > delayMinutes*60*1000){
        // Remember when we published
        lastPublish = millis();

        //String pubAccel = String::format("%d,%d,%d",t.readX(),t.readY(),t.readZ());
        //Serial.println(pubAccel);
        //Particle.publish("A", pubAccel, 60, PRIVATE);

        // Dumps the full NMEA sentence to serial in case you're curious
        Serial.println(t.preNMEA());

        // GPS requires a "fix" on the satellites to give good data,
        // so we should only publish data if there's a fix
        if(t.gpsFix()){
            // Only publish if we're in transmittingData mode 1;
            if(transmittingData){
                // Short publish names save data!
                Particle.publish("G", t.readLatLon(), 60, PRIVATE);
            }
            // but always report the data over serial for local development
            Serial.println(t.readLatLon());
        }
    }

    // 30 second serial logger
    if(millis()%5000 == 0 ) {
       Serial.print("doing the 30 loop readXYZmagnitude ");
        //Serial.println(t.readXYZmagnitude());

        Serial.print( "SoC: ");
        Serial.print( fuel.getSoC() );
        Serial.print( " VCell: ");
        Serial.println( fuel.getVCell() );

        Serial.print("GPS FIX: ");
        Serial.print( t.gpsFix() );

        Serial.println(t.readLatLon());

        Serial.println("  x  y  z ");
        Serial.print(t.readX());  Serial.print(" ");
        Serial.print(t.readY());  Serial.print(" ");
        Serial.print(t.readZ());  Serial.print(" ");




    }

   //debug the gps serial
   while (Serial1.available()){
        Serial.print(char(Serial1.read()));
    }
}

// Remotely change the trigger threshold!
int accelThresholder(String command){
    accelThreshold = atoi(command);
    return 1;
}

// Allows you to remotely change whether a device is publishing to the cloud
// or is only reporting data over Serial. Saves data when using only Serial!
// Change the default at the top of the code.
int transmitMode(String command){
    transmittingData = atoi(command);
    return 1;
}

// Actively ask for a GPS reading if you're impatient. Only publishes if there's
// a GPS fix, otherwise returns '0'
int gpsPublish(String command){
    if(t.gpsFix()){
        Particle.publish("G", t.readLatLon(), 60, PRIVATE);

        // uncomment next line if you want a manual publish to reset delay counter
        // lastPublish = millis();
        return 1;
    }
    else { return 0; }
}
