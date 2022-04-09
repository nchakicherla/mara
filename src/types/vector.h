#ifndef VEC_H
#define VEC_H

#include "../common.h"

int
destroyVector(vec* vec_in) {

    if(vec_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    for(size_t i = 0; i < vec_in->len; i++) {
        destroy(vec_in->ptrs[i]);
    }
    free(vec_in->ptrs);
    free(vec_in);

    SUCCESS(FN);
    return 0;
}

int
removeFromVector(vec* vec_in, size_t pos) {

    if(vec_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: vec_in (NULL)");
        return 1;
    }

    destroy(vec_in->ptrs[pos]);

    for(size_t i = pos; i < vec_in->len - 1; i++) {
        vec_in->ptrs[i] = vec_in->ptrs[i + 1];
    }

    void** temp_ptrs = NULL;
    if(!(temp_ptrs = malloc((vec_in->len - 1) * sizeof(void *)))) {
        throwFatal(FN, "Allocate failed for: temp_ptrs (malloc)");
    }

    for(size_t i = 0; i < vec_in->len - 1; i++) {
        temp_ptrs[i] = vec_in->ptrs[i];
    }
    free(vec_in->ptrs);

    vec_in->ptrs = temp_ptrs;
    vec_in->len--;

    SUCCESS(FN);
    return 0;
}

void*
accessVector(vec* vec_in, size_t loc) {

    if(vec_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: vec_in (NULL)");
    }
    if(loc >= vec_in->len) {
        ERROR(ARG_ERR, FN, "Argument exceeds length of vec_in: loc");
        return NULL;
    }

    return vec_in->ptrs[loc];
}

vec*
vectorInitV(size_t len, ...) {

    vec* vec_out = NULL;
    if(!(vec_out = malloc(sizeof(vec)))) {
        throwFatal(FN, "Allocate failed for: vec_out (malloc)");
    }
    vec_out->type = VEC_TYPE;

    if(len != 0) {
        if(!(vec_out->ptrs = malloc(len * sizeof(void *)))) {
            free(vec_out);
            throwFatal(FN, "Allocate failed for: vec_out->ptrs (malloc)");
        }
        va_list obj_args;
        va_start(obj_args, len);

        for(size_t i = 0; i < len; i++) {
            vec_out->ptrs[i] = duplicate(va_arg(obj_args, void *));
            if(!vec_out->ptrs[i]) {
                destroy(vec_out);
                ERROR(MEM_ERR, FN, "Allocate failed for: vec_out->ptrs[i] (duplicate)");
                return NULL;
            }
        }
        vec_out->len = len;

    } else {
        vec_out->ptrs = NULL;
        vec_out->len = 0;
    }

    SUCCESS(FN);
    return vec_out;
}

int
growVectorPointers(vec* vec_in, size_t new_len) {

    if(vec_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: vec_in (NULL)");
        return 1;
    }
    if(new_len < vec_in->len || new_len == VEC_ERR) {
        WARN(FN, "Argument invalid: new_len should not exceed VEC_MAX or vec_in->len");
        return 3;
    }

    void** new_ptrs = NULL;
    if(!(new_ptrs = realloc(vec_in->ptrs, new_len * sizeof(void *)))) {
        throwFatal(FN, "Allocate failed for: new_ptrs (realloc). Input vecay unchanged");
    }

    for(size_t i = vec_in->len; i < new_len; i++) { //set newly allocated void* to NULL
        new_ptrs[i] = NULL;
    }
    vec_in->len = new_len;
    vec_in->ptrs = new_ptrs;

    SUCCESS(FN);
    return 0;
}

int
truncateVector(vec* vec_in, size_t new_len) {

    if(vec_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid: vec_in (NULL)");
        return 1;
    }
    if(new_len > vec_in->len) {
        ERROR(ARG_ERR, FN, "Argument invalid: new_len should be less than vec_in->len");
        return 2;
    }

    void** temp_ptrs = NULL;
    if(!(temp_ptrs = malloc(new_len * sizeof(void*)))) {
        throwFatal(FN, "Allocate failed for: temp_ptrs (malloc). Input vecay unchanged");
    }

    for(size_t i = new_len; i < vec_in->len; i++) {
        if(vec_in->ptrs[i]) {
            destroy(vec_in->ptrs[i]);
        }
    }
    for(size_t i = 0; i < new_len; i++) {
        temp_ptrs[i] = vec_in->ptrs[i];
    }
    free(vec_in->ptrs);
    vec_in->ptrs = temp_ptrs;
    vec_in->len = new_len;

    SUCCESS(FN);
    return 0;
}

int
appendToVector(vec* target, void* adding) {

    if( adding == NULL ||
        target == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }

    size_t target_old_len = target->len;

    if(TYPE_OF(adding) == VEC_TYPE) {

        vec* adding_cast = (vec*) adding;

        if(VEC_MAX - target->len < adding_cast->len) {
            ERROR(ARG_ERR, FN, "Input objects invalid (combined length exceeds VEC_MAX)");
            return 3;
        }

        if(growVectorPointers(target, target->len + adding_cast->len)) {
            ERROR(INIT_ERR, FN, "Initialization failed for: target (growVectorPointers)");
            return 4;
        }
        for(size_t adding_it = 0, i = target->len - adding_cast->len; i < target->len; adding_it++, i++) {
            void* temp_obj = NULL;
            if(!(temp_obj = duplicate(adding_cast->ptrs[adding_it]))) {
                truncateVector(target, target_old_len);
                ERROR(INIT_ERR, FN, "Initialization failed for: temp_obj (duplicate)");
                return 5;
            }
            target->ptrs[i] = temp_obj;
        }

        SUCCESS(FN);
        return 0;

    } else {
        if(growVectorPointers(target, target->len + 1)) {
            ERROR(INIT_ERR, FN, "Initialization failed for: target (growVectorPointers)");
            return 6;
        }
        void* temp_obj = NULL;
        if(!(temp_obj = duplicate(adding))) {
            truncateVector(target, target_old_len);
            ERROR(INIT_ERR, FN, "Initialization failed for: temp_obj (duplicate)");
            return 7;
        }
        target->ptrs[target->len - 1] = temp_obj;

        SUCCESS(FN);
        return 0;
    }
}

int
moveToVectorEnd(vec* target, void* eating) {

    if( eating == NULL ||
        target == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }

    if(TYPE_OF(eating) == VEC_TYPE) {
        vec* adding_cast = (vec*) eating;
        if(VEC_MAX - target->len < adding_cast->len) {
            ERROR(ARG_ERR, FN, "Input objects invalid (combined length exceeds VEC_MAX)");
            return 4;
        }
        if(growVectorPointers(target, target->len + adding_cast->len)) {
            ERROR(INIT_ERR, FN, "Initialization failed for: target (growVectorPointers)");
            return 5;
        }
        for(size_t adding_it = 0, i = target->len - adding_cast->len; i < target->len; adding_it++, i++) {
            target->ptrs[i] = adding_cast->ptrs[adding_it];
        }
    } else {
        if(growVectorPointers(target, target->len + 1)) { //growVectorPointers will change target->len
            ERROR(INIT_ERR, FN, "Initialization failed for: target (growVectorPointers)");
            return 7;
        }
        target->ptrs[target->len - 1] = eating;
    }

    SUCCESS(FN);
    return 0;
}

int
mergeVectorsAt(vec* target, vec* adding, size_t pos) {

    size_t target_old_len = target->len;

    if(growVectorPointers(target, target->len + adding->len)) {
        ERROR(INIT_ERR, FN, "Initialization failed for: target (growVectorPointers)");
        return 6;
    }
    if(pos == target->len) {
        WARN(FN, "Input argument equals target->len, calling appendToVector");
        return appendToVector(target, adding);
    }

    for(size_t i = pos; i < target_old_len; i++) {
        target->ptrs[i + adding->len] = target->ptrs[i];
    }

    for(size_t adding_it = 0, i = pos; adding_it < adding->len; adding_it++, i++) {
        void* temp_obj = duplicate(adding->ptrs[adding_it]);
        if(!temp_obj) {
            truncateVector(target, target_old_len);
            ERROR(INIT_ERR, FN, "Initialization failed for temp_obj (duplicate)");
            return 7;
        }
        target->ptrs[i] = temp_obj;
    }

    SUCCESS(FN);
    return 0;
}

int
addToVectorAt(vec* target, void* obj_in, size_t pos) {

    if( obj_in == NULL ||
        target == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }

    size_t target_old_len = target->len;

    if(pos > target->len || pos == VEC_ERR) {
        ERROR(ARG_ERR, FN, "Argument invalid: pos (must not exceed target->len)");
        return 4;
    }

    if(TYPE_OF(obj_in) == VEC_TYPE && pos == target->len) {
        if(appendToVector(target, obj_in)) {
            ERROR(INIT_ERR, FN, "Initialization to add obj_in to target (appendToVector)");
            return 5;
        } else {            
            SUCCESS(FN);
            return 0;
        }

    } else {
        if(growVectorPointers(target, target->len + 1)) {
            ERROR(INIT_ERR, FN, "Initialization failed for: target (growVectorPointers)");
            return 6;
        }
        for(size_t i = target->len - 1; i > pos; i--) {
            target->ptrs[i] = target->ptrs[i - 1];
        }

        void* temp_obj = NULL;
        if(!(temp_obj = duplicate(obj_in))) {
            truncateVector(target, target_old_len);
            ERROR(INIT_ERR, FN, "Initialization failed for: temp_obj (duplicate)");
            return 7;
        }
        target->ptrs[pos] = temp_obj;

        SUCCESS(FN);
        return 0;
    }
}

vec*
duplicateVector(vec* vec_in) {

    if(vec_in == NULL) {
        ERROR(ARG_ERR, FN, "Input objects invalid (NULL)");
        return NULL;
    }

    vec* vec_out = NULL;
    if(!(vec_out = malloc(sizeof(vec)))) {
        ERROR(MEM_ERR, FN, "Allocate failed for: vec_out (malloc)");
        return NULL;
    }
    vec_out->type = VEC_TYPE;
    vec_out->len = vec_in->len;

    if(!(vec_out->ptrs = malloc(vec_in->len * sizeof(void *)))) {
        free(vec_out);
        throwFatal(FN, "Allocate failed for: vec_out->ptrs (malloc)");
    }

    for(size_t i = 0; i < vec_in->len; i++) {
        if(!(vec_out->ptrs[i] = duplicate(vec_in->ptrs[i]))) {
            ERROR(INIT_ERR, FN, "Initialization failed for: vec_out->ptrs[i] (duplicate)");
            for(size_t j = 0; j < i; j++) {
                destroy(vec_out->ptrs[j]);
            }
            free(vec_out->ptrs);
            free(vec_out);
            return NULL;
        }
    }
    
    SUCCESS(FN);
    return vec_out;
}

#endif // VEC_H
