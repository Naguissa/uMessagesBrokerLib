/**
 * \class uMessagesBrokerLib
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
*	* *uMessagesBrokerLib::encode(const char index, const char message[], char output[], uint16_t messageLength = 0);* : Given a plain message, encodes it in base64 prepending the index and a '-' as header. Optionally you can specify the input message length.
 *	* *char uMessagesBrokerLib::decode(const char message[], char output[]);* : Decodes a message previously encoded
 *	* *uMessagesBrokerLib::process(const char buffer[]);* : Given a encoded message, decodes it and calls its pertinent handler or default handler if defined and no other handler matches or message has not the correct header.
 *
 *
 *
 * @file uMessagesBrokerLib.h
 * @copyright Naguissa
 * @author Naguissa
 * @see <a href="https://github.com/Naguissa/uMessagesBrokerLib">https://github.com/Naguissa/uMessagesBrokerLib</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 * @version 1.2.0
 */

 #ifndef _uMessagesBrokerLib_h_
    #define _uMessagesBrokerLib_h_

    #include <Arduino.h>
    #include "uHexLib.h"

    struct uMessagesBrokerLibList {
        uMessagesBrokerLibList() : fn(NULL), index(0), next(NULL) {};
        void (*fn) (const char*);
        char index;
        uMessagesBrokerLibList *next;
    };

    class uMessagesBrokerLib {
        public:
            static void set(const char, void (*) (const char*));
            static bool remove(const char);
            static void setDefault(void (*) (const char*));
            static void removeDefault();
            static void encode(const char, const char [], char [], uint16_t = 0);
            static char decode(const char [], char [], uint16_t = 0);
            static void process(const char [], uint16_t = 0);
        private:
            // Purely static methods
            uMessagesBrokerLib();
            static uMessagesBrokerLibList *list;
            static void (*fallback) (const char*);
    };

#endif

