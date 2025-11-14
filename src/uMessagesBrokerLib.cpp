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
 * @version 1.2.0
 */
 #ifndef _uMessagesBrokerLib_cpp_
    #define _uMessagesBrokerLib_cpp_

    #include "uMessagesBrokerLib.h"
    #include "uHexLib.h"

    uMessagesBrokerLibList * uMessagesBrokerLib::list = NULL;
    void (*uMessagesBrokerLib::fallback) (const char*) = NULL;

    uMessagesBrokerLib::uMessagesBrokerLib() {}

    void uMessagesBrokerLib::set(const char index, void (*fn) (const char*)) {
        if (list == NULL) {
            list = new uMessagesBrokerLibList;
            list->index = index;
            list->fn = fn;
            list->next = NULL;
            return;
        }
        uMessagesBrokerLibList * prev = list;
        for (uMessagesBrokerLibList * slot = list; slot != NULL; slot = slot->next) {
            if (slot->index == index) {
                slot->fn = fn;
                return;
            }
            prev = slot;
        }

        prev->next = new uMessagesBrokerLibList;
        prev->next->index = index;
        prev->next->fn = fn;
        prev->next->next = NULL;
    }

    bool uMessagesBrokerLib::remove(const char index) {
        uMessagesBrokerLibList *slot, *prev;
        for (slot = list, prev = NULL; slot != NULL; slot = slot->next) {
            if (slot->index == index) {
                if (prev == NULL) { // 1st element of the list
                    if (slot->next == NULL) { // Only element of the list
                        free(list);
                        list = NULL;
                        return true;
                    } 
                    // Not only element
                    list = list->next;
                    free(slot);
                    return true;
                }
                // Middle of the list
                prev->next = slot->next;
                free(slot);
                return true;
            }
            prev = slot;
        }
        return false;
    }


    void uMessagesBrokerLib::setDefault(void (*fn) (const char*)) {
        fallback = fn;
    }
    
    void uMessagesBrokerLib::removeDefault() {
        fallback = NULL;
    }

    void uMessagesBrokerLib::encode(const char index, const char message[], char output[], uint16_t messageLength) {
        output[0] = index;
        output[1] = '-';
        uHexLib::encode(message, &output[2], messageLength);
    }


    char uMessagesBrokerLib::decode(const char message[], char output[], uint16_t size) {
        if (size == 0) {
            size = strlen(message) / 2 - 1;
        }
        
        if (size < 2 || message[1] != '-') {
            return 0;
        }
        if (size > 2) {
            uHexLib::decode(&message[2], output, size);
        }
        return message[0];
    }


    void uMessagesBrokerLib::process(const char buffer[], uint16_t size) {
        if (size == 0) {
            size = strlen(buffer) / 2 - 1;
        }
        if (size > 2 &&  buffer[1] == '-' && list != 0) {
            for (uMessagesBrokerLibList *slot = list; slot != NULL; slot = slot->next) {
                if (slot->index == buffer[0]) {
                    char decoded[size + 1];
                    uMessagesBrokerLib::decode(buffer, decoded, size);
                    slot->fn(decoded);
                    return;
                }
            }
            if (fallback != NULL) {
                char decoded[size + 1];
                uMessagesBrokerLib::decode(buffer, decoded, size);
                fallback(decoded);
                return;
            }
        }
        if (fallback != NULL) {
            fallback(buffer);
        }
    }
    
#endif
    
