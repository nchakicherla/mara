#ifndef FLT_H
#define FLT_H

#include "../common.h"

flt*
floatInit(double val_in, int prec_in) {

    flt* flt_out = malloc(sizeof(flt));
    if(flt_out == NULL) {
        throwFatal(FN, "Allocate failed for: flt_out (malloc)");
    }

    flt_out->type = FLT_TYPE;
    flt_out->fracdigs = prec_in;
    flt_out->val = val_in;

    SUCCESS(FN);
    return flt_out;
}

int
destroyFloat(flt* flt_in) {

    if(flt_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    free(flt_in);
    SUCCESS(FN);
    return 0;
}

flt*
duplicateFloat(flt* flt_in) {

    if(flt_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    flt* flt_out = floatInit(flt_in->val, flt_in->fracdigs);
    if(flt_out == NULL) {
        ERROR(INIT_ERR, FN, "Initialization failed for: flt_out (floatInit)");
        return NULL;
    }

    flt_out->fracdigs = flt_in->fracdigs;
    
    SUCCESS(FN);
    return flt_out;
}

#endif // FLT_H
