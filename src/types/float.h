#ifndef FLT_H
#define FLT_H

#include "../common.h"

flt*
newFloat(double val_in, int prec_in) {

    flt* flt_out = malloc(sizeof(flt));
    if(flt_out == NULL) {
        setError(MEM_ERR, FN, "Allocate failed for: flt_out (malloc)");
        return NULL;
    }

    flt_out->type = FLT_TYPE;
    flt_out->fracdigs = prec_in;
    flt_out->val = val_in;

    setSuccess(FN);
    return flt_out;
}

int
destroyFloat(flt* flt_in) {

    if(flt_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    free(flt_in);
    setSuccess(FN);
    return 0;
}

flt*
duplicateFloat(flt* flt_in) {

    if(flt_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    flt* flt_out = newFloat(flt_in->val, flt_in->fracdigs);
    if(flt_out == NULL) {
        setError(MEM_ERR, FN, "Allocate failed for: flt_out (newFloat)");
        return NULL;
    }

    flt_out->fracdigs = flt_in->fracdigs;
    
    setSuccess(FN);
    return flt_out;
}

#endif // FLT_H
