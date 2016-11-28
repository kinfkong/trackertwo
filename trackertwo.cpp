/* -----------------------------------------------------------
This example shows a lot of different features. As configured here
it'll check for a good GPS fix every 10 minutes and publish that data
if there is one. If not, it'll save you data by staying quiet. It also
registers 3 Particle.functions for changing whether it publishes,
reading the battery level, and manually requesting a GPS reading.
---------------------------------------------------------------*/

// includes
#include "lib/AssetTracker/firmware/AssetTracker.h"
 #include "trackertwo.h"
 #include "lib/streaming/firmware/spark-streaming.h"


void setup() {
    t.begin();
    t.gpsOn();
    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);
    // Serial1.begin(9600);  //NOTE:   it looks like I dont need this anymore

    // These three functions are useful for remote diagnostics. Read more below.
    Particle.function("tmode", transmitMode);
    Particle.function("gps", gpsPublish);


    pinMode(D7, OUTPUT);
    digitalWrite(D7, HIGH);  //turn on built in led
}


void loop() {
    // You'll need to run this every loop to capture the GPS output
    t.updateGPS();

    //FIRST GPS LOC
    if(t.gpsFix() && gpsloctime == 0 ) {
      gpsloctime = millis()/1000;
      digitalWrite(D7, LOW);
    }

    // if the current time - the last time we published is greater than your set delay...
    if(millis()-lastPublish > delayMinutes*60*1000){
        // Remember when we published
        lastPublish = millis();


        // Dumps the full NMEA sentence to serial in case you're curious
        Serial << millis()/1000 << "  ";
        Serial.println(t.preNMEA());

        // GPS requires a "fix" on the satellites to give good data,
        // so we should only publish data if there's a fix
        if(t.gpsFix()){
            // Only publish if we're in transmittingData mode 1;
            if(transmittingData){
                // Short publish names save data!
                Particle.publish("G", t.readLatLon(), 60, PRIVATE);
                Particle.publish("GLAT", String(t.readLatDeg()), 60, PRIVATE);
                Particle.publish("GLON", String(t.readLonDeg()), 60, PRIVATE);
            }
            // but always report the data over serial for local development
            Serial.print("in the if gpsfix condtion");
            Serial.println(t.readLatLon());
            digitalWrite(D7, LOW);   // turn off the led on the fix
        }
    }

    // debug logger
    if(millis()%5000 == 0 && mydebug ) {
       Serial << endl << MYBUILD << " 5s Belt " << millis()/1000 << "  GPS FIX TIME: " << gpsloctime  << "  " << t.readLatLon() << endl;
       if(gpsloctime > 0 ) {
         Serial << "Lat: " << String(t.readLatDeg()) << " LON " << String(t.readLonDeg()) << endl;
       }

    }

   //debug the gps serial

   while (Serial1.available() && gpsserialdebug ){
        Serial.print(char(Serial1.read()));
      //  Serial.print("k");
        //serial1Avail = 1;
    // atempt to echo from serial1 to serial
       //char c = Serial1.read();
        //Serial.write(c);

    }

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
