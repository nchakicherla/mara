#ifndef ITR_H
#define ITR_H

#include "../common.h"

itr*
integerInit(long val) {

    itr* itr_out = NULL;
    if(!(itr_out = malloc(sizeof(itr)))) {
        throwFatal(FN, "Allocate failed for: itr_out (malloc)");
        return NULL;
    }
    itr_out->type = ITR_TYPE;
    itr_out->val = val;

    SUCCESS(FN);
    return itr_out;
}

int
destroyInteger(itr* itr_in) {

    if(itr_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object type invalid (destroyInteger expects ITR_TYPE)");
        return 1;
    }
    free(itr_in);

    SUCCESS(FN);
    return 0;
}

itr*
duplicateInteger(itr* itr_in) {

    if(itr_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    itr* itr_out = NULL;
    if(!(itr_out = integerInit(itr_in->val))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: itr_out (integerInit)");
        return NULL;
    }

    SUCCESS(FN);
    return itr_out;
}

#endif // ITR_H
