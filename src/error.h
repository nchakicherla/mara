#ifndef ERROR_H
#define ERROR_H

#include "common.h"

unsigned long
toArbitraryLong(char *str) {

    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

FILE* objopslog = NULL;
char current_success[256];
char current_warning[256];
char current_error[256];
size_t msg_counter = 1;

int
openLog() {

    if(objopslog != NULL) {
        setError(ARG_ERR, FN, "Log file already opened");
        return 1;
    }

    if(!(objopslog = fopen("debug.log", "w"))) {
        printf("Unable to create log file. Exiting...\n");
        exit(1);
    }
    fprintf(objopslog, "\nBEGIN\n\n");

    return 0;
}

int //for more flexible logging in exception cases (e.g. print() or input() functions)
logMessage(int n, ...) {

    if(objopslog) {

        va_list msg_args;
        va_start(msg_args, n);
        char* current_msg = NULL;

        fprintf(objopslog, "%zu > ", msg_counter);

        for(int i = 0; i < n; i++) {
            current_msg = va_arg(msg_args, char *);
            if(current_msg) {
                fprintf(objopslog, "%s", current_msg);
            }
            current_msg = NULL;
        }

        fprintf(objopslog, "\n");
        msg_counter++;
    }

    return 0;
}

int
closeLog() {

    if(objopslog) {
        fprintf(objopslog, "\nEND\n");
        fclose(objopslog);
    } else {
        setError(ARG_ERR, FN, "Log file has not been opened");
        return 1;
    }

    return 0;
}

void
fatalExit(const char* func, const char* desc) {

    if(objopslog) {
        fprintf(objopslog, "\nFATAL ERROR in %s - %s. Aborting...\n", func, desc);
        fclose(objopslog);
    }
    exit(1);
}

int
setSuccess(const char* func) {

    sprintf(current_success, "%sSUCCESS%s in: %s%s%s\n", KGRN, KNRM, KYEL, func, KNRM);

    if(SUCCESS) {
        if(LOGGING) {
            if(PRINTLOG) {
                printf("%s", current_success);
            }
            if(objopslog) {
                fprintf(objopslog, "%zu > SUCCESS in: %s\n", msg_counter, func);
            }
        }
        msg_counter++;
    }

    return 0;
}

int
setWarning(const char* func, const char* desc) {

    sprintf(current_warning, "%sWARNING%s in %s%s%s - %s%s%s\n", KMAG, KNRM, KYEL, func, KNRM, KCYN, desc, KNRM);

    if(WARNING) {
        if(LOGGING) {
            if(PRINTLOG) {
                printf("%s", current_warning);
            }
            if(objopslog) {
                fprintf(objopslog, "%zu > WARNING in %s - %s\n", msg_counter, func, desc);
            }
        }
        msg_counter++;
    }

    return 0;
}

int
setError(char* errtype, const char* func, const char* desc) {

    sprintf(current_error, "%s%s%s in %s%s%s - %s%s%s\n", KRED, errtype, KNRM, KYEL, func, KNRM, KRED, desc, KNRM);

    if(ERROR) {
        if(LOGGING) {
            if(PRINTLOG) {
                printf("%s", current_error);
            }
            if(objopslog) {
                fprintf(objopslog, "%zu > %s in %s - %s\n", msg_counter, errtype, func, desc);
            }
        }
        msg_counter++;
    }

    return 0;
}

#endif // ERROR_H
