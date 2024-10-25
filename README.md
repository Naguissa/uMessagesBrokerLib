# Arduino (and standard C) tiny and cross-device compatible hexadecimal coding/decoding library.

## What is this repository for?

Arduino (and standard C) tiny and cross-device compatible messaging system library.

Supports any microcontroller.

You can use this library to communicate internally or externally (serial, I2C, MQTT, requests...) any program.
This library lets you define any handler for any message index, encode the message to send and parse, check, decode and manage received messages.

 Messages are encoded in hexadecimal using <a href="https://github.com/Naguissa/uHexLib">uHexLib</a>, A char index is used to differentiate message types.
 
 Message structure is:
 <INDEX CHAR>-<HEXADECIMAL ENCODED MESSAGE>


## Important: Dependencies

This library requires uHexLib libary; it's available on IDE Library Manager and also here:

https://github.com/Naguissa/uHexLib

https://github.com/Naguissa/uHexLib/releases



## Usage ##

You have this new type used for handler functions, called uMessagesBrokerLibFunction, which really is void callñbackFunctionName(const char message[]). This means that callback functions must be defined as callñbackFunctionName is done. See example for details.


You have uMessagesBrokerLib class with following static methods:

 - **uMessagesBrokerLib::set(const char index, const void (*)(const char*) fn);** : Sets the function handler for a message index.
 - **uMessagesBrokerLib::remove(const char index);** : Removes the function handler for a message index.
 - ***uMessagesBrokerLib::setDefault(const void (*)(const char*) fn);** : Sets the function handler any message which index has no handler defined
 - ***uMessagesBrokerLib::removeDefault();** : Removes the function handler any message which index has no handler defined
 - ***uMessagesBrokerLib::encode(const char index, const char message[], char output[]);** : Given a plain message, encodes it in base64 prepending the index and a '-' as header
 - ***char uMessagesBrokerLib::decode(const char message[], char output[]);** : Decodes a message previously encoded
 - ***uMessagesBrokerLib::process(const char buffer[]);** : Given a encoded message, decodes it and calls its pertinent handler or default handler if defined and no other handler matches or message has not the correct header.



## How do I get set up?

You can get it from Arduino libraries directly, searching by uMessagesBrokerLib.

For manual installation:

 * Get the ZIP from releases link: https://github.com/Naguissa/uMessagesBrokerLib/releases
 * Rename to uHexLib.zip
 * Install library on Arduino

## Example

Included on example folder, available on Arduino IDE.


## Who do I talk to?

 * [Naguissa](https://github.com/Naguissa)
 * https://www.foroelectro.net


## Contribute

Any code contribution, report or comment are always welcome. Don't hesitate to use GitHub for that.


 * You can sponsor this project using GitHub's Sponsor button: https://github.com/Naguissa/uMessagesBrokerLib
 * You can make a donation via PayPal: https://paypal.me/foroelectro


Thanks for your support.


Contributors hall of fame: https://www.foroelectro.net/hall-of-fame-f32/contributors-contribuyentes-t271.html
