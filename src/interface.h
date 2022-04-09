#ifndef INTERFACE_H
#define INTERFACE_H

#include "common.h"

#define INP_BUF_LEN         512

char*
inputSeq(const char* prompt) {

    char* input_buf = NULL;
    char* input_end = NULL;

    if( !(input_buf = calloc(INP_BUF_LEN, sizeof(char)) )) {
        setError(MEM_ERR, FN, "Allocate failed for: input_buf (calloc)");
        return NULL;
    }

    if(prompt == NULL) {
        logMessage(1, "[inputrequest] (no prompt)");
    } else {
        logMessage(2, "[inputrequest] ", prompt);
        printf("%s", prompt);
    }

    while(!(input_end = seqChar(input_buf, '\n', INP_BUF_LEN))) {
        input_buf = fgets(input_buf, INP_BUF_LEN, stdin);
    }
    *input_end = '\0';

    return input_buf;
}

void* // max length of user input is INP_BUF_LEN - 1
input(const char* prompt, int TYPE_OUT) {
    
    char* input_buf = inputSeq(prompt);
    if(input_buf == NULL) {
        setError(MEM_ERR, FN, "Allocate failed for: input_buf (inputSeq)");
        return NULL;   
    }

    logMessage(4, "[input] ", "\"", input_buf, "\"");

    void* obj_out = NULL;

    switch(TYPE_OUT) {
        case STR_TYPE: {
            if(!(obj_out = stringInit(input_buf))) {
                setError(MEM_ERR, FN, "Allocate failed for: obj_out (stringInit)");
            }
            break;
        }
        case ITR_TYPE: {
            if(!(obj_out = seqToInteger(input_buf))) {
                setError(MEM_ERR, FN, "Allocate failed for: obj_out (integerInit)");
            }
            break;
        }
        case FLT_TYPE: {
            if(!(obj_out = seqToFloat(input_buf))) {
                setError(MEM_ERR, FN, "Allocate failed for: obj_out (seqToFloat)");
            }
            break;
        }
        default: {
            setError(ARG_ERR, FN, "Input argument invalid: TYPE_OUT (Type code invalid)");
            break;
        }
    }
    free(input_buf);

    if(obj_out == NULL) {
        setError(ARG_ERR, FN, "Output object blank (NULL); input invalid for provided TYPE_OUT");
    } else {
        setSuccess(FN);
    }

    return obj_out;
}

int
print(void* obj_in) {

    char* print_buf = NULL;
    bool oneline = false;

    if(obj_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    if(TYPE_OF(obj_in) == VEC_TYPE) {
        oneline = true;
    }

    if(!(print_buf = objectAsChars(obj_in, false, oneline))) {
        setError(MEM_ERR, FN, "Allocate failed for: print_buf (objectAsChars)");
        return 2;
    }

    printf("%s\n", print_buf);

    if(seqLen(print_buf) < 1000) {
        logMessage(2, "[print] ", print_buf);
    } else {
        logMessage(1, "[print] Large object");
    }
    
    free(print_buf);

    setSuccess(FN);
    return 0;
}

#endif // INTERFACE_H
