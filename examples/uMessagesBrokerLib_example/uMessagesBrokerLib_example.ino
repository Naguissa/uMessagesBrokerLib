/**
 * \brief Arduino (and standard C) tiny and cross-device compatible messaging system library.
 *
 * You can use this library to communicate internally or externally (serial, I2C, MQTT, requests...) any program.
 * This library lets you define any handler for any message index, encode the message to send and parse, check, decode and manage received messages.
 *
 * Messages are encoded in hexadecimal using <a href="https://github.com/Naguissa/uHexLib">uHexLib</a>, A char index is used to differentiate message types. Message structure is:
 *  <INDEX CHAR>-<HEXADECIMAL ENCODED MESSAGE>
 *
 * You have this new type used for handler functions, called uMessagesBrokerLibFunction, which really is void callñbackFunctionName(const char message[]). This means that callback functions must be defined as callñbackFunctionName is done. See example for details.
 *
 * You have uMessagesBrokerLib class with following static methods:
 *
 *	* *uMessagesBrokerLib::set(const char index, const void (*)(const char*) fn);* : Sets the function handler for a message index.
 *	* *uMessagesBrokerLib::remove(const char index);* : Removes the function handler for a message index.
 *	* *uMessagesBrokerLib::setDefault(const void (*)(const char*) fn);* : Sets the function handler any message which index has no handler defined
 *	* *uMessagesBrokerLib::removeDefault();* : Removes the function handler any message which index has no handler defined
 *	* *uMessagesBrokerLib::encode(const char index, const char message[], char output[]);* : Given a plain message, encodes it in base64 prepending the index and a '-' as header
 *	* *char uMessagesBrokerLib::decode(const char message[], char output[]);* : Decodes a message previously encoded
 *	* *uMessagesBrokerLib::process(const char buffer[]);* : Given a encoded message, decodes it and calls its pertinent handler or default handler if defined and no other handler matches or message has not the correct header.
 *
 *
 *
 * @file uMessagesBrokerLib_example.ino
 * @copyright Naguissa
 * @author Naguissa
 * @see <a href="https://github.com/Naguissa/uMessagesBrokerLib">https://github.com/Naguissa/uMessagesBrokerLib</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 * @version 1.0.0
 */

#include "Arduino.h"
#include "uMessagesBrokerLib.h"

char string[50];
char encoded[100];
char decoded[50];


void callbackA(const char message[]) {
    Serial.print("Callback A: ");
    Serial.print('"');
    Serial.print(message);
    Serial.println('"');
}

void callbackB(const char message[]) {
    Serial.print("Callback B: ");
    Serial.print('"');
    Serial.print(message);
    Serial.println('"');
}

void callbackC(const char message[]) {
    Serial.print("Callback C: ");
    Serial.print('"');
    Serial.print(message);
    Serial.println('"');
}

void callbackDefault(const char message[]) {
    Serial.print("Default callback: ");
    Serial.print('"');
    Serial.print(message);
    Serial.println('"');
}



void doTest(char index) {
    
    // Encode
    uMessagesBrokerLib::encode(index, string, encoded);
    Serial.print("Coding of string \"");
    Serial.print(string);
    Serial.print("\" with index \"");
    Serial.print(index);
    Serial.print("\" is \"");
    Serial.print(encoded);
    Serial.println("\"");

    delay(500);

    // Decode
    index = uMessagesBrokerLib::decode(encoded, decoded);
    Serial.print("Decoding of string \"");
    Serial.print(encoded);
    Serial.print("\" is \"");
    Serial.print(decoded);
    Serial.print("\" with index \"");
    Serial.print(index);
    Serial.println("\"");

    // Broker
    Serial.println(" -- calling broker --");
    uMessagesBrokerLib::process(encoded);
    Serial.println();
}







void setup() {
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.println(" - SETUP -");
    
    uMessagesBrokerLib::set('A', callbackA);
    uMessagesBrokerLib::set('B', callbackB);
    uMessagesBrokerLib::set('C', callbackC);
    uMessagesBrokerLib::set('D', callbackA);
    uMessagesBrokerLib::remove('D');    

    // Remove and setting again 'A'
    uMessagesBrokerLib::remove('A');    
    uMessagesBrokerLib::set('A', callbackA);

    // Setting again 'B'; old one will be overwritten    
    uMessagesBrokerLib::set('B', callbackB);

    Serial.println(" - SETUP DONE -");
    Serial.println();
    
    Serial.println(" - START -");
    Serial.println();
    
    uMessagesBrokerLib::setDefault(callbackDefault);    

    strcpy(string, "THE,QUICK,BROWN,FOX,0123456789");
    doTest('A');

    strcpy(string, "https://github.com/Naguissa");
    doTest('B');

    strcpy(string, "https://www.foroelectro.net");
    doTest('C');

    strcpy(string, "Deleted, will fallback");
    doTest('D');

    strcpy(string, "Fallback");
    doTest('E');


    uMessagesBrokerLib::removeDefault();    
    strcpy(string, "No fallback, skipped");
    doTest('F');

    Serial.println(" - END -");
    Serial.println();
    
}


void loop() {
}

