#ifndef ITR_H
#define ITR_H

#include "../common.h"

itr*
newInteger(long val) {

    itr* itr_out = NULL;
    if(!(itr_out = malloc(sizeof(itr)))) {
        setError(MEM_ERR, FN, "Allocate failed for: itr_out (malloc)");
        return NULL;
    }
    itr_out->type = ITR_TYPE;
    itr_out->val = val;

    setSuccess(FN);
    return itr_out;
}

int
destroyInteger(itr* itr_in) {
    if(itr_in == NULL) {
        setError(ARG_ERR, FN, "Input object type invalid (destroyInteger expects ITR_TYPE)");
        return 1;
    }
    free(itr_in);

    setSuccess(FN);
    return 0;
}

itr*
duplicateInteger(itr* itr_in) {

    if(itr_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    itr* itr_out = NULL;
    if(!(itr_out = newInteger(itr_in->val))) {
        setError(MEM_ERR, FN, "Allocate failed for: itr_out (newInteger)");
        return NULL;
    }

    setSuccess(FN);
    return itr_out;
}

#endif // ITR_H
