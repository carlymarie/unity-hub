#ifndef HELPERS_H_
#define HELPERS_H_
#include <Arduino.h>
#include <configuration.h>
#include <debugger.h>
#ifdef DEBUG_ENABLED
#pragma GCC diagnostic ignored "-Wunused-function"

#define SUCCESS F("\x1b[32m\x1b[1m[OK]\033[0m\033[0m")
#define FAILED F("\x1b[31m[FAILED]\033[0m")
#define ERROR F("\x1b[31m[ERROR]\033[0m")
#define DISABLED F("\x1b[33m[DISABLED]\033[0m")

static String pad(String txt)
{
    int max_len = 50;
    int len = txt.length();
    for (int i = len; i < max_len; i++)
    {
        txt += " ";
    }
    return txt;
}


#define DEBUG_PRINT_S(a, b) b ? Serial.println(a) : Serial.print(a)
#define DEBUG_PRINT(a, b) dbg->send(a, b)
#else
#define DEBUG_PRINTP(x)
#define DEBUG_PRINT(x, y)
#define DEBUG_WRITE(x, y)
#endif
#endif // HELPERS_H_