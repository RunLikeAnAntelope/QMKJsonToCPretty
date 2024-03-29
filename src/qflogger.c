#include "qflogger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_LOCATION "QMKJsonToCPretty.log"

void qfSaveToFile(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    FILE* fptr;

    fptr = fopen(LOG_LOCATION, "a");
    if (fptr == NULL) {
        printf("Logging file failed to open");
        exit(1);
    }
    vfprintf(fptr, fmt, ap);
    fclose(fptr);

    va_end(ap);
}

void qfLog(const char* logMsg) { qfSaveToFile("%s\n", logMsg); }
