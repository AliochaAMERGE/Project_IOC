#include <Arduino.h>

void (*callback)(char *msgTopic, byte *msgPayload, unsigned int msgLength);

