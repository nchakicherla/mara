#ifndef STR_H
#define STR_H

#include "../common.h"

size_t
seqLen(const char* seq_in) {

    size_t len = 0;
    const char* it = seq_in;
    while(*it != '\0' && len <= STR_MAX) {
        len++, it++;
    }

    return len;
}

char*
seqChar(char* seq_in, char c, size_t len) {

    for(size_t index = 0; index < len; index++) {
        if(seq_in[index] == c) {

            return &seq_in[index];
        }
    }

    return NULL;
}

char*
seqDupl(const char* seq_in) {

    if(seq_in == NULL) {
        ERROR(ARG_ERR, FN, "Input sequence invalid (NULL)");
        return NULL;
    }

    size_t seq_in_len = seqLen(seq_in);
    char* seq_out = NULL;

    if(!(seq_out = malloc(seq_in_len + 1))) {
        throwFatal(FN, "Allocate failed for: seq_out (malloc)");
    }
    for(size_t i = 0; i < seq_in_len; i++) {
        seq_out[i] = seq_in[i];
    }
    seq_out[seq_in_len] = '\0';

    SUCCESS(FN);
    return seq_out;
}

itr*
seqToInteger(char* seq_in) {

    if(seq_in == NULL) {
        ERROR(ARG_ERR, FN, "Input sequence invalid (NULL)");
        return NULL;
    }

    for(size_t i = 0; i < seqLen(seq_in); i++) {
        if(!isdigit(seq_in[i])) {
            ERROR(ARG_ERR, FN, "Input contains a non-numeric character");
            return NULL;
        }
    }

    char* end_ptr = seq_in;
    long val = strtol(seq_in, &end_ptr, 10);

    if(end_ptr == seq_in) {
        ERROR(ARG_ERR, FN, "Input sequence invalid: failed conversion (strtol)");
        return NULL;
    }

    void* itr_out = NULL;
    if(!(itr_out = integerInit(val))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: itr_out (integerInit)");
        return NULL;
    }

    SUCCESS(FN);
    return itr_out;
}

flt*
seqToFloat(char* seq_in) {

    if(seq_in == NULL) {
        ERROR(ARG_ERR, FN, "Input sequence invalid (NULL)");
        return NULL;
    }

    for(size_t i = 0; i < seqLen(seq_in); i++) { // check for any illegal characters
        if(!isdigit(seq_in[i]) && seq_in[i] != '.' && seq_in[i] != 'e' && seq_in[i] != '-') {
            ERROR(ARG_ERR, FN, "Input contains a non-number");
            return NULL;
        }
    }

    char* end_ptr = seq_in;
    double val = strtod(seq_in, &end_ptr);
    int fracdigs = 0;

    if(end_ptr == seq_in) {
        ERROR(ARG_ERR, FN, "Input object invalid: str_in->seq failed conversion (strtod)");
        return NULL;
    }

    char* e_loc = seqChar(seq_in, 'e', seqLen(seq_in));
    char* decimal_loc = seqChar(seq_in, '.', seqLen(seq_in));

    if(decimal_loc) {
        if(*(decimal_loc + 1) != '\0') { // check for second decimal
            if(seqChar(decimal_loc + 1, '.', seqLen(seq_in) - (size_t)(decimal_loc - seq_in + 1))) {
                ERROR(ARG_ERR, FN, "Input sequence invalid for FLT_TYPE: multiple decimal points");
                return NULL;
            }
        }
    }
    if(e_loc) {
        if(*(e_loc + 1) != '\0') {
            if(seqChar(e_loc + 1, 'e', seqLen(seq_in) - (size_t)(e_loc - seq_in + 1))) {
                ERROR(ARG_ERR, FN, "Input sequence invalid for FLT_TYPE: multiple 'e' characters");
                return NULL;
            }            
        }
    }
    if(e_loc && *(e_loc + 1) == '-') {           
        char* it = e_loc;
        if(decimal_loc) {
            while(*(it - 1) != '.') {
                it--;
                fracdigs++;
            }
        }
        fracdigs += (int)strtol(e_loc + 2, NULL, 10);
    } else if (!e_loc && decimal_loc) {
        fracdigs += (seqLen(seq_in) - (int)(decimal_loc - seq_in + 1));
    }
    if(fracdigs > 15) {
        ERROR(ARG_ERR, FN, "Input precision exceeds max 15 fractional digits");
        return NULL;
    }
    if(val > 1e20) {
        ERROR(ARG_ERR, FN, "Input value exceeds max 3e33");
        return NULL;
    }

    flt* flt_out = NULL;
    if(!(flt_out = floatInit(val, fracdigs))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: flt_out (floatInit)");
        return NULL;
    }

    SUCCESS(FN);
    return flt_out;
}

bool
seqSame(const char* target, const char* comp) {

    if( target == NULL ||
        comp == NULL) {
            ERROR(ARG_ERR, FN, "Input sequence(s) invalid (NULL)");
            return false;
    }

    size_t target_len = seqLen(target);
    if(target_len != seqLen(comp)) {
        return false;
    }

    for(size_t i = 0; i < target_len; i++) {
        if(target[i] != comp[i]) {
            return false;
        }
    }

    SUCCESS(FN);
    return true;
}

int
setString(str* str_in, const char* seq_in) {

    if( str_in == NULL ||
        seq_in == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }
    if(str_in->seq) {
        free(str_in->seq);
    }

    if(!(str_in->seq = malloc(seqLen(seq_in) + 1))) {
        throwFatal(FN, "Allocate failed for: str_in->seq (malloc)");
    }

    for(size_t i = 0; i <= seqLen(seq_in); i++) {
        str_in->seq[i] = seq_in[i];   
    }

    SUCCESS(FN);
    return 0;
}

str*
stringInit(const char* seq_in) {

    if(seq_in == NULL) {
        ERROR(ARG_ERR, FN, "Input sequence invalid (NULL)");
        return NULL;
    }
    if(seqLen(seq_in) == STR_ERR) {
        ERROR(ARG_ERR, FN, "Input sequence length invalid (length exceeds STR_MAX)");
        return NULL;
    }
    if(seqLen(seq_in) == 0) {
        ERROR(ARG_ERR, FN, "Input sequence blank");
        return NULL;
    }

    str* output_str = NULL;
    if(!(output_str = malloc(sizeof(str)))) {
        throwFatal(FN, "Allocate failed for: output_str (malloc)");
    }

    if(!(output_str->seq = malloc(seqLen(seq_in) + 1))) {
        free(output_str);
        throwFatal(FN, "Allocate failed for: output_str->seq (malloc)");
    }
    output_str->type = STR_TYPE;
    output_str->len = seqLen(seq_in);

    for(size_t seq_ind = 0; seq_ind < seqLen(seq_in) + 1; seq_ind++) {
        output_str->seq[seq_ind] = seq_in[seq_ind];
    }

    SUCCESS(FN);
    return output_str;
}

int
destroyString(str* str_in) {

    if(str_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }
    if(str_in->seq) {
        free(str_in->seq);
    }
    free(str_in);

    SUCCESS(FN);
    return 0;
}

int
growString(str* str_in, size_t new_len) {

    if(str_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }
    if(new_len < str_in->len) {
        ERROR(ARG_ERR, FN, "Input length invalid (new_len is less than current length)");
        return 2;
    }
    if(new_len == STR_ERR) {
        ERROR(ARG_ERR, FN, "Input length invalid (new_len exceeds STR_MAX)");
        return 3;
    }

    if(str_in->len == new_len) {
        return 0;
    }

    char *new_seq = NULL;
    if(!(new_seq = realloc(str_in->seq, new_len + 1))) {
        throwFatal(FN, "Allocate failed for: new_seq (realloc)");
    }
    str_in->len = new_len;
    str_in->seq = new_seq;
    str_in->seq[new_len] = '\0';

    SUCCESS(FN);
    return 0;
}

int
addString(str* target, str* adding) {

    if(target == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: target (NULL)");
        return 1;
    }
    if(adding == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: adding (NULL)");
        return 2;
    }
    if(target->len + adding->len == STR_ERR) {
        ERROR(ARG_ERR, FN, "Input objects invalid (combined length exceeds STR_MAX)");
        return 4;
    }

    if(adding->len == 0) {
        WARN(FN, "Input object has length of zero: adding");
        return 0;
    }

    size_t target_old_len = target->len;

    if( 0 != growString(target, target->len + adding->len) ) {
        ERROR(INIT_ERR, FN, "Initialization failed for: target (growString)");
        return 5;
    }

    char* target_it = &target->seq[target_old_len];
    const char* adding_it = adding->seq;
    while(*adding_it != '\0') {
        *target_it = *adding_it;
        target_it++;
        adding_it++;
    }

    SUCCESS(FN);
    return 0;
}

int
insertString(str* target, str* adding, size_t pos) {

    if(target == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: target (NULL)");
        return 1;
    }
    if(adding == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: adding (NULL)");
        return 2;
    }
    if(STR_MAX - adding->len < target->len) {
        ERROR(ARG_ERR, FN, "Input object(s) invalid: combined length exceeds STR_MAX");
        return 4;
    }

    if(adding->len == 0) {
        WARN(FN, "Input object has length of zero: adding");
        return 0;
    }
    if(pos == target->len) {
        return addString(target, adding);
    }

    size_t target_old_len = target->len;

    if(!(growString(target, target->len + adding->len))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: target (growString)");
        return 5;
    }
    for(size_t i = pos; i < target_old_len; i++) {
        target->seq[i + adding->len] = target->seq[i];
    }

    char* adding_it = adding->seq;

    for(size_t i = pos; *adding_it != '\0'; i++) {
        target->seq[i] = *adding_it;
        adding_it++;
    }
    SUCCESS(FN);
    return 0;
}

size_t
findString(str* target, str* find) {

    if( target == NULL ||
        find == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return STR_ERR;
    }
    if(TYPE_OF(target) != STR_TYPE || TYPE_OF(find) != STR_TYPE) {
        ERROR(ARG_ERR, FN, "Input object(s) type invalid (findString expects STR_TYPE)");
        return STR_ERR;
    }
    if(find->len > target->len) {
        ERROR(ARG_ERR, FN, "Input object length exceeds length of target: find");
        return STR_ERR;
    }

    for(size_t i = 0; i <= target->len - find->len; i++) {
        size_t chars_same = 0;
        for(size_t j = 0; j < find->len; j++) {
            if(find->seq[j] == target->seq[i + j])
                chars_same++;
        }
        if(chars_same == find->len) {
            return i;
        }
    }

    return STR_ERR;
}

str*
duplicateString(str* input_str) {

    if(input_str == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    void* output_str = NULL;
    if(!(output_str = stringInit(input_str->seq))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: output_str (stringInit)");
    }

    SUCCESS(FN);
    return output_str;
}

str*
sliceFromString(str* input_str, size_t start, size_t end) {

    if(input_str == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: input_str (NULL)");
        return NULL;
    }
    if(start >= input_str->len || end >= input_str->len) {
        ERROR(ARG_ERR, FN, "Argument(s) invalid: start, end exceed bounds of input_str");
        return NULL;
    }

    str* output_str = NULL;
    if(!(output_str = stringInit(""))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: output_str (stringInit)");
        return NULL;
    }

    size_t target_ind = 0;

    if (start < end) {
        if(growString(output_str, end - start + 1)) {
            ERROR(INIT_ERR, FN, "Initialization failed for: output_str (growString)");
            return NULL;
        }
        for(size_t i = start; i <= end; i++) {
            output_str->seq[target_ind] = input_str->seq[i];
            target_ind++;
        }
        output_str->len = end - start + 1;
    } else if (start > end) {
        if(growString(output_str, start - end + 1)) {
            ERROR(INIT_ERR, FN, "Initialization failed for: output_str (growString)");
            return NULL;
        }
        for(size_t i = start; i > end; --i) {
            output_str->seq[target_ind] = input_str->seq[i];
            target_ind++;
        }
        output_str->seq[target_ind] = input_str->seq[end];
        output_str->len = start - end + 1;
    } else {
        if(growString(output_str, 1)) {
            ERROR(INIT_ERR, FN, "Initialization failed for: output_str (growString)");
            return NULL;
        }
        output_str->seq[0] = input_str->seq[start];
    }
    
    SUCCESS(FN);
    return output_str;
}

#endif // STR_H
