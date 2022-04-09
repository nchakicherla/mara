#ifndef HSH_H
#define HSH_H

#include "../common.h"

int
destroyKeyValuePair(kvp* kvp_in) {

    if(kvp_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    if(kvp_in->key == NULL) {
        WARN(FN, "Input object invalid: kvp_in->key (NULL). Ignoring...");
    } else {
        free(kvp_in->key);
    }
    destroy(kvp_in->value);
    free(kvp_in);

    SUCCESS(FN);
    return 0;
}

kvp*
duplicateKeyValuePair(kvp* kvp_in) {

    if(kvp_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    kvp* kvp_out = NULL;
    if(!(kvp_out = malloc(sizeof(kvp)))) {
        throwFatal(FN, "Allocate failed for: kvp_out (malloc)");
    }
    if(!(kvp_out->key = seqDupl(kvp_in->key))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: kvp_out->key (seqDupl)");
        free(kvp_out);
        return NULL;
    }
    if(!(kvp_out->value = duplicate(kvp_in->value))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: kvp_out->value (duplicate)");
        free(kvp_out->key);
        free(kvp_out);
        return NULL;
    }
    SUCCESS(FN);
    return kvp_out;
}

hsh*
hashInit() {

    hsh* output_hash = NULL;
    if(!(output_hash = malloc(sizeof(hsh)))) {
        throwFatal(FN, "Allocate failed for: output_hash (malloc)");
    }

    output_hash->type = HSH_TYPE;
    output_hash->len = 0;
    output_hash->kvptrs = NULL;

    SUCCESS(FN);
    return output_hash;
}

void*
accessHashValue(hsh* hash_in, const char* key_access) {

    if( hash_in == NULL || 
        key_access == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return NULL;        
    }
    if(hash_in->len == 0) {
        return NULL;
    }

    size_t get_loc = HSH_ERR;
    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(hash_in->kvptrs[i]->key, key_access)) {
            get_loc = i;
            break;
        }
    }
    if(get_loc == HSH_ERR) {
        ERROR(ARG_ERR, FN, "Could not find key in hash");
        return NULL;
    }

    SUCCESS(FN);
    return hash_in->kvptrs[get_loc]->value;
}

kvp*
accessHashPair(hsh* hash_in, const char* key_access) {

    if( hash_in == NULL ||
        key_access == NULL) {
            ERROR(ARG_ERR, FN, "Argument(s) invalid (NULL)");
            return NULL;
    }
    if(hash_in->len == 0) {
        return NULL;
    }

    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(key_access, hash_in->kvptrs[i]->key)) {
            SUCCESS(FN);
            return hash_in->kvptrs[i];
        }
    }

    ERROR(ARG_ERR, FN, "Could not find key in hash");
    return NULL;
}

int
changeKey(hsh* hash_in, const char* key_current, const char* key_new) {

    if( hash_in == NULL ||
        key_current == NULL ||
        key_new == NULL) {
            ERROR(ARG_ERR, FN, "Argument(s) invalid (NULL)");
            return 1;
    }

    kvp* working_kvp = NULL;
    char* temp_key = NULL;

    if(!(working_kvp = accessHashPair(hash_in, key_current))) {
        ERROR(ARG_ERR, FN, "Could not locate key in hash");
        return 2;
    }

    if(!(temp_key = seqDupl(key_new))) {
        throwFatal(FN, "Allocate failed for temp_key (seqDupl)");
        return 3;
    }
    free(working_kvp->key);
    working_kvp->key = temp_key;
    return 0;
}

int
addKeyToHash(hsh* hash_in, const char* key_in, void* value_in) {

    if( hash_in == NULL ||
        key_in == NULL ||
        value_in == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }
    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(hash_in->kvptrs[i]->key, key_in)) {
            WARN(FN, "key_in already exists in hash_in");
            return 3;
        }
    }

    kvp* new_kvp = NULL;
    if(!(new_kvp = malloc(sizeof(kvp)))) {
        throwFatal(FN, "Allocate failed for: new_kvp (malloc)");
    }

    size_t key_length = seqLen(key_in);
    if(!key_length) {
        ERROR(ARG_ERR, FN, "Input argument invalid: key_in (key must contain at least 1 character)");
        free(new_kvp);
        return 5;
    }
    if(!(new_kvp->key = malloc(key_length + 1))) {
        free(new_kvp);
        throwFatal(FN, "Allocate failed for: new_kvp->key (malloc)");
    }
    if(!(new_kvp->value = duplicate(value_in))) {
        ERROR(MEM_ERR, FN, "Allocate failed for: new_kvp->value (duplicate)");
        free(new_kvp->key);
        free(new_kvp);
        return 7;
    }

    for(size_t i = 0; i < key_length; i++) {
        new_kvp->key[i] = key_in[i];
    }
    new_kvp->key[key_length] = '\0';

    if(hash_in->len == 0) {
        if(!(hash_in->kvptrs = malloc(sizeof(kvp**)))) {
            destroyKeyValuePair(new_kvp);
            throwFatal(FN, "Allocate failed for: hash_in->kvptrs (malloc)");
        }
        hash_in->kvptrs[0] = new_kvp;
    } else {
        kvp** temp_kvps = NULL;
        if(!(temp_kvps = realloc(hash_in->kvptrs, (hash_in->len + 1) * sizeof(kvp**)))) {
            destroyKeyValuePair(new_kvp);
            throwFatal(FN, "Allocate failed for: temp_kvps (realloc)");
        }
        temp_kvps[hash_in->len] = new_kvp;
        hash_in->kvptrs = temp_kvps;
    }
    hash_in->len++;

    SUCCESS(FN);
    return 0;
}

int
moveKeyToHash(hsh* hash_in, const char* key_in, void* eating) {

    if( !hash_in ||
        !key_in ||
        !eating) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }
    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(key_in, hash_in->kvptrs[i]->key)) {
            WARN(FN, "key_in already exists in hash_in");
            return 3;
        }
    }

    kvp* new_kvp = NULL;
    if(!(new_kvp = malloc(sizeof(kvp)))) {
        ERROR(MEM_ERR, FN, "Allocate failed for: new_kvp (malloc)");
        return 4;
    }

    size_t key_length = seqLen(key_in);
    if(key_length == 0) {
        ERROR(ARG_ERR, FN, "Input argument invalid: key_in (key must contain at least 1 character)");
        free(new_kvp);
        return 5;
    }
    if(!(new_kvp->key = seqDupl(key_in))) {
        ERROR(INIT_ERR, FN, "Initialization failed for: new_kvp->key (seqDupl)");
        free(new_kvp);
        return 6;
    }
    new_kvp->value = eating;

    if(hash_in->len == 0) {
        if(!(hash_in->kvptrs = malloc(sizeof(kvp**)))) {
            destroyKeyValuePair(new_kvp);
            throwFatal(FN, "Allocate failed for: hash_in_cast->kvptrs (malloc)");
        }
        hash_in->kvptrs[0] = new_kvp;
    } else {
        kvp** temp_kvps = NULL;
        if(!(temp_kvps = realloc(hash_in->kvptrs, (hash_in->len + 1) * sizeof(kvp**)))) {
            destroyKeyValuePair(new_kvp);
            throwFatal(FN, "Allocate failed for: temp_kvps (realloc)");
        }
        temp_kvps[hash_in->len] = new_kvp;
        hash_in->kvptrs = temp_kvps;
    }
    hash_in->len++;

    SUCCESS(FN);
    return 0;
}

int
removeFromHash(hsh* hash_in, const char* key_delete) {

    if( hash_in == NULL||
        key_delete == NULL) {
            ERROR(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }
    if(hash_in->len == 0) {
        WARN(FN, "Length of hash_in is zero. Aborting lookup");
        return 2;
    }

    size_t del_location = HSH_ERR;
    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(key_delete, hash_in->kvptrs[i]->key)) {
            destroyKeyValuePair(hash_in->kvptrs[i]);
            del_location = i;
            break;
        }
    }
    if(del_location == HSH_ERR) {
        WARN(FN, "Key key_delete not found in hash_in");
        return 3;
    }

    if(hash_in->len == 1) { //if hash only has 1 key
        free(hash_in->kvptrs);
        hash_in->len = 0;
        return 4;
    }
    if(del_location < hash_in->len - 1) { //continues in all other cases
        for(size_t i = del_location; i < hash_in->len - 1; i++) {
            hash_in->kvptrs[i] = hash_in->kvptrs[i + 1];
        }
    }

    kvp** temp_kvps = NULL;
    if(!(temp_kvps = realloc(hash_in->kvptrs, (hash_in->len - 1) * sizeof(kvp**)))) {
        throwFatal(FN, "Allocate failed for: temp_kvps (realloc)");
    }
    hash_in->kvptrs = temp_kvps;
    hash_in->len--;

    SUCCESS(FN);
    return 0;
}

int
destroyHash(hsh* hash_in) {

    if(hash_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    if(hash_in->len == 0) { //if empty hash
        free(hash_in);
        SUCCESS(FN);
        return 0;
    }

    for(size_t i = 0; i < hash_in->len; i++) {
        destroyKeyValuePair(hash_in->kvptrs[i]);
    }
    free(hash_in->kvptrs);
    free(hash_in);

    SUCCESS(FN);
    return 0;
}

hsh*
duplicateHash(hsh* hash_in) {

    if(hash_in == NULL) {
        ERROR(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    hsh* hash_out = NULL;
    if(!(hash_out = hashInit())) {
        ERROR(INIT_ERR, FN, "Initialization failed for: hash_out (hashInit)");
        return NULL;
    }
    if(!(hash_out->kvptrs = malloc(hash_in->len * sizeof(kvp**)))) {
        free(hash_out);
        throwFatal(FN, "Allocate failed for: hash_out->kvptrs (malloc)");
    }

    for(size_t i = 0; i < hash_in->len; i++) { //duplicate all pairs
        if(!(hash_out->kvptrs[i] = duplicateKeyValuePair(hash_in->kvptrs[i]))) {
            for(size_t j = 0; j < i; j++) {
                ERROR(INIT_ERR, FN, "Initialization failed for hash_out->kvptrs[i] (duplicateKeyValuePair)");
                destroyKeyValuePair(hash_out->kvptrs[i]);
            }
            destroyHash(hash_out);
            return NULL;
        }
    }
    hash_out->len = hash_in->len;

    SUCCESS(FN);
    return hash_out;
}

#endif // HSH_H
