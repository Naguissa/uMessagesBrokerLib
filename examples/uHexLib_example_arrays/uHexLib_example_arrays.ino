/**
 * Arduino (standard C) tiny and cross-device compatible hexadecimal coding/decoding library.
 *
 * You have this new type used as return type of encoding, which has 2 bytes, named 'a' and 'b':
 *      * uHexLib2Bytes: { char a; char b; }
 *
 * You have uHexLib class with following static methods:
 *	* *uHexLib2Bytes uHexLib::encode(const char);* : Given one char it encodes it to 2 base64 chars and returns the struct
 *	* *char uHexLib::decode(const char inA, const char inB);* : Given two base64 chars it decodes and returns them as 1 char
 *
 *	* *uint16_t uHexLib::encode(const char in[], char out[]);* : Given one char it encodes it to 2 base64 chars and returns the struct
 *	* *uint16_t  uHexLib::decode(const char in[], char out[]);* : Given two base64 chars it decodes and returns them as 1 char
 *
 * @copyright Naguissa
 * @author Naguissa
 * @see <a href="https://github.com/Naguissa/uHexLib">https://github.com/Naguissa/uHexLib</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 */

#include "Arduino.h"
#include "uHexLib.h"

char string[50];
char encoded[100];
char decoded[50];


void doTest() {
    uint16_t i;

    // Encode
    uHexLib::encode(string, encoded);
    Serial.print("Hexadecimal coding of string \"");
    Serial.print(string);
    Serial.print("\" is \"");
    Serial.print(encoded);
    Serial.println("\"");

    delay(500);

    // Decode
    uHexLib::decode(encoded, decoded);
    Serial.print("Hexadecimal decoding of string \"");
    Serial.print(encoded);
    Serial.print("\" is \"");
    Serial.print(decoded);
    Serial.println("\"");

    Serial.println();
}







void setup() {
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.println(" - SETUP DONE -");
    Serial.println();
}


void loop() {
    Serial.println(" - START -");
    Serial.println();

    strcpy(string, "THE,QUICK,BROWN,FOX,0123456789");
    doTest();

    strcpy(string, "https://github.com/Naguissa");
    doTest();

    strcpy(string, "https://www.foroelectro.net");
    doTest();



    Serial.println(" - END -");
    Serial.println();

    delay(3000);
}

