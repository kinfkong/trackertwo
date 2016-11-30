/**
 * TCSDEVELOPER 11.28.2016
 */

#ifndef __PAYLOAD_BUILDER__
#define __PAYLOAD_BUILDER__



// represents the buffer size
#define PAYLOAD_BUFFER_SIZE 512

/**
 * This represents the wrapper for produce the json payload.
 *
 * @author TCSDEVELOPER
 * @version 1.0
 */
class PayloadBuilder {
public:
    // the constructor
    PayloadBuilder() {
        _jsonObj = &_jsonBuffer.createObject();
    }
#ifndef __INDEPENDENT_TEST__    
    // construct the payload string
    String toString() {
        _jsonObj->printTo(_buf, sizeof(_buf));
        return String(_buf);
    }
#else
    char* toString() {
        _jsonObj->printTo(_buf, sizeof(_buf));
        return _buf;
    }
#endif

    // add the key value pair to the payload
    template <typename T>
    void addKeyValue(const char* key, T value) {
        (*_jsonObj)[key] = value;
    }

    // same to addKeyValue, but easier for used like:
    // obj["lat"] = 38.5;
    // obj["lng"] = 127.0;
    JsonVariant& operator[](const char* key) {
        return (*_jsonObj)[key];
    }

private:
    char _buf[PAYLOAD_BUFFER_SIZE]; // represents the buffer to store the string
    JsonObject* _jsonObj;           // represents the json object
    StaticJsonBuffer<PAYLOAD_BUFFER_SIZE> _jsonBuffer; // represents the buffer to store the json object
};

#endif
