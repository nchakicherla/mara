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

FILE* type_log = NULL;
char current_success[256];
char current_warning[256];
char current_error[256];
size_t msg_counter = 1;

int
openLog() {
    if( !(type_log = fopen("objops.log", "w")) ) {
        printf("Unable to create log file. Exiting...\n");
        exit(1);
    }
    fprintf(type_log, "\nBEGIN\n\n");
    return 0;
}

int
logMessage(int n, ...) {
    if(type_log) {
        va_list msg_args;
        va_start(msg_args, n);
        fprintf(type_log, "%zu > ", msg_counter);
        char* current_msg = NULL;
        for(int i = 0; i < n; i++) {
            current_msg = va_arg(msg_args, char *);
            if(current_msg) {
                fprintf(type_log, "%s", current_msg);
            }
            current_msg = NULL;
        }
        fprintf(type_log, "\n");
        msg_counter++;
    }
    return 0;
}

int
closeLog() {
    if(type_log) {
        fprintf(type_log, "\nEND\n");
        fclose(type_log);
    } else {
        return 1;
    }
    return 0;
}

int
setSuccess(const char* func) {
    sprintf(current_success, "%sSUCCESS%s in: %s%s%s\n", KGRN, KNRM, KYEL, func, KNRM);
    if(SUCCESS) {
        if(LOGGING) {
            printf("%s", current_success);
            if(type_log) {
                fprintf(type_log, "%zu > SUCCESS in: %s\n", msg_counter, func);
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
            printf("%s", current_warning);
            if(type_log) {
                fprintf(type_log, "%zu > WARNING in %s - %s\n", msg_counter, func, desc);
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
            printf("%s", current_error);
            if(type_log) {
                fprintf(type_log, "%zu > %s in %s - %s\n", msg_counter, errtype, func, desc);
            }
        }
        msg_counter++;
    }
    return 0;
}

#endif // ERROR_H
