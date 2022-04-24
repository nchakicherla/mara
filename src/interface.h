#ifndef INTERFACE_H
#define INTERFACE_H

#include "common.h"

#define INPUT_MAX_LEN         512

char*
mInputSeq(const char* prompt) {

    char* input_buf = NULL;
    char* input_end = NULL;

    if( !(input_buf = calloc(INPUT_MAX_LEN, sizeof(char)) )) {
        throwFatal(FN, "Allocate failed for: input_buf (calloc)");
    }

    if(prompt == NULL) {
        logMessage(1, "[inputrequest] (no prompt)");
    } else {
        logMessage(3, "[inputrequest] (\"", prompt, "\")");
        printf("%s", prompt);
    }

    while(!(input_end = seqChar(input_buf, '\n', INPUT_MAX_LEN))) {
        input_buf = fgets(input_buf, INPUT_MAX_LEN, stdin);
    }
    *input_end = '\0';

    return input_buf;
}

void* // max length of user mInput is INPUT_MAX_LEN - 1
mInput(const char* prompt, int TYPE_OUT) {
    
    char* input_buf = mInputSeq(prompt);
    if(input_buf == NULL) {
        ERROR(INIT_ERR, FN, "Initialization failed for: input_buf (mInputSeq)");
        return NULL;   
    }

    logMessage(4, "[mInput] ", "\"", input_buf, "\"");

    void* obj_out = NULL;

    switch(TYPE_OUT) {
        case STR_TYPE: {
            if(NULL == (obj_out = stringInit(input_buf))) {
                ERROR(INIT_ERR, FN, "Initialization failed for: obj_out (stringInit)");
            }
            break;
        }
        case ITR_TYPE: {
            if(NULL == (obj_out = seqToInteger(input_buf))) {
                ERROR(INIT_ERR, FN, "Initialization failed for: obj_out (integerInit)");
            }
            break;
        }
        case FLT_TYPE: {
            if(NULL == (obj_out = seqToFloat(input_buf))) {
                ERROR(INIT_ERR, FN, "Initialization failed for: obj_out (seqToFloat)");
            }
            break;
        }
        default: {
            ERROR(ARG_ERR, FN, "Input argument invalid: TYPE_OUT (Type code invalid)");
            break;
        }
    }
    
    free(input_buf);

    if(obj_out == NULL) {
        ERROR(ARG_ERR, FN, "Output object blank (NULL)");
    } else {
        SUCCESS(FN);
    }

    return obj_out;
}

int
mPrint(void* obj_in) {

    char* print_buf = NULL;
    bool oneline = false;

    if(obj_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    if(TYPE_OF(obj_in) == VEC_TYPE) {
        oneline = true;
    }

    if(!(print_buf = objectAsChars(obj_in, false, oneline))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: print_buf (objectAsChars)");
        return 2;
    }

    printf("%s\n", print_buf); //single object so printing newline

    if(seqLen(print_buf) < 1024) {
        if(TYPE_OF(obj_in) != HSH_TYPE) {
            logMessage(2, "[mPrint] ", print_buf);
        } else {
            logMessage(2, "[mPrint]\n", print_buf);            
        }
    } else {
        logMessage(1, "[mPrint] ( LONG OBJECT )");
    }

    free(print_buf);

    SUCCESS(FN);
    return 0;
}

int
mPrintV(int n, ...) {

    va_list obj_args;
    va_start(obj_args, n);
    void* current_obj = NULL;
    char* print_buf = NULL;
    bool oneline = false;

    for(int i = 0; i < n; i++) {
        current_obj = va_arg(obj_args, void *);

        if(TYPE_OF(current_obj) == VEC_TYPE) {
            oneline = true;
        }

        if(!(print_buf = objectAsChars(current_obj, false, oneline))) {
            ERROR(INIT_ERR, FN, "Initialization failed for: print_buf (objectAsChars)");
            return 1;
        }

        printf("%s", print_buf); //skipping newline in case multiple arguments

        if(seqLen(print_buf) < 1024) {
            if(TYPE_OF(current_obj) != HSH_TYPE) {
                logMessage(2, "[mPrintV] ", print_buf);
            } else {
                logMessage(2, "[mPrintV]\n", print_buf);            
            }
        } else {
            logMessage(1, "[mPrintV] ( LONG OBJECT )");
        }

        oneline = false;
        free(print_buf);
    }

    printf("\n"); //only printing one newline

    SUCCESS(FN);
    return 0;
}

#endif // INTERFACE_H
