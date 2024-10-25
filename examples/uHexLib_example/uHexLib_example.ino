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
char result[100];


void doTest() {
    uint16_t i;
    uHexLib2Bytes res;

    // Encode
    for (i = 0; string[i] != 0; i++) {
        res = uHexLib::encode(string[i]);
        result[2*i] = res.a;
        result[2*i + 1] = res.b;
    }
    result[2*i] = 0;

    Serial.print("Hexadecimal coding of string \"");
    Serial.print(string);
    Serial.print("\" is \"");
    Serial.print(result);
    Serial.println("\"");

    delay(500);

    // Decode
    for (i = 0; result[2*i] != 0; i++) {
        string[i] = uHexLib::decode(result[2*i], result[2*i + 1]);
    }
    string[i] = 0;

    Serial.print("Hexadecimal decoding of string \"");
    Serial.print(result);
    Serial.print("\" is \"");
    Serial.print(string);
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

    strcpy(string, "THE,QUICK,BROWN,FOX,0123456789\0");
    doTest();

    strcpy(string, "https://github.com/Naguissa\0");
    doTest();

    strcpy(string, "https://www.foroelectro.net\0");
    doTest();



    Serial.println(" - END -");
    Serial.println();

    delay(3000);
}

