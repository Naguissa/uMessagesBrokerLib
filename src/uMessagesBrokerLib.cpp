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
 * @file uMessagesBrokerLib.cpp
 * @copyright Naguissa
 * @author Naguissa
 * @see <a href="https://github.com/Naguissa/uMessagesBrokerLib">https://github.com/Naguissa/uMessagesBrokerLib</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 * @version 1.1.0
 */
 #ifndef _uMessagesBrokerLib_cpp_
    #define _uMessagesBrokerLib_cpp_

    #include "uMessagesBrokerLib.h"
    #include "uHexLib.h"

    uMessagesBrokerLibList * uMessagesBrokerLib::list = 0;
    uMessagesBrokerLibFunction uMessagesBrokerLib::fallback = 0;

    uMessagesBrokerLib::uMessagesBrokerLib() {}

    void uMessagesBrokerLib::set(const char index, const uMessagesBrokerLibFunction fn) {
        if (list == 0) {
            list = new uMessagesBrokerLibList;
            list->index = index;
            list->fn = fn;
            list->next = 0;
            return;
        }
        
        uMessagesBrokerLibList * prev = list;
        for (uMessagesBrokerLibList * slot = list; slot != 0; slot = slot->next) {
            if (slot->index == index) {
                slot->fn = fn;
                return;
            }
            prev = slot;
        }
        
        prev->next = new uMessagesBrokerLibList;
        prev->next->index = index;
        prev->next->fn = fn;
        prev->next->next = 0;
    }

    bool uMessagesBrokerLib::remove(const char index) {
        uMessagesBrokerLibList *slot, *prev;
        for (slot = list, prev = 0; slot != 0; slot = slot->next) {
            if (slot->index == index) {
                if (prev == 0 && slot->next == 0) { // Only element of the list
                    list = 0;
                }
                if (prev != 0) {
                    prev->next = slot->next;
                }
                free(slot);
                return true;
            }
            prev = slot;
        }
        return false;
    }


    void uMessagesBrokerLib::setDefault(const uMessagesBrokerLibFunction fn) {
        fallback = fn;
    }
    
    void uMessagesBrokerLib::removeDefault() {
        fallback = 0;
    }

    void uMessagesBrokerLib::encode(const char index, const char message[], char output[], uint16_t messageLength) {
        output[0] = index;
        output[1] = '-';
        uHexLib::encode(message, &output[2], messageLength);
    }


    char uMessagesBrokerLib::decode(const char message[], char output[]) {
        uint16_t size = strlen(message);
        if (size < 2 || message[1] != '-') {
            return 0;
        }
        if (size > 2) {
            uHexLib::decode(&message[2], output);
        }
        return message[0];
    }


    void uMessagesBrokerLib::process(const char buffer[]) {
        uint16_t size = strlen(buffer);
        if (size > 2 && list != 0) {
            for (uMessagesBrokerLibList *slot = list; slot != 0; slot = slot->next) {
                if (slot->index == buffer[0]) {
                    char decoded[size * 2 - 4];
                    uHexLib::decode(&buffer[2], decoded);            
                    slot->fn(decoded);
                    return;
                }
            }
            if (fallback != 0) {
                char decoded[size * 2 - 4];
                uHexLib::decode(&buffer[2], decoded);            
                fallback(decoded);
                return;
            }
        }
        if (fallback != 0) {
            fallback(buffer);
        }
    }
    
#endif
    
