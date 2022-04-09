#ifndef HSH_H
#define HSH_H

#include "../common.h"

int
destroyKeyValuePair(kv* kvp_in) {

    if(kvp_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    if(kvp_in->key == NULL) {
        setWarning(FN, "Input object invalid: kvp_in->key (NULL). Ignoring...");
    } else {
        free(kvp_in->key);
    }
    destroy(kvp_in->value);
    free(kvp_in);

    setSuccess(FN);
    return 0;
}

kv*
duplicateKeyValuePair(kv* kvp_in) {

    if(kvp_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    kv* kvp_out = NULL;
    if(!(kvp_out = malloc(sizeof(kv)))) {
        fatalExit(FN, "Allocate failed for: kvp_out (malloc)");
    }
    if(!(kvp_out->key = seqDupl(kvp_in->key))) {
        setError(INIT_ERR, FN, "Initialization failed for: kvp_out->key (seqDupl)");
        free(kvp_out);
        return NULL;
    }
    if(!(kvp_out->value = duplicate(kvp_in->value))) {
        setError(INIT_ERR, FN, "Initialization failed for: kvp_out->value (duplicate)");
        free(kvp_out->key);
        free(kvp_out);
        return NULL;
    }
    setSuccess(FN);
    return kvp_out;
}

hsh*
hashInit() {

    hsh* output_hash = NULL;
    if(!(output_hash = malloc(sizeof(hsh)))) {
        fatalExit(FN, "Allocate failed for: output_hash (malloc)");
    }

    output_hash->type = HSH_TYPE;
    output_hash->len = 0;
    output_hash->kvptrs = NULL;

    setSuccess(FN);
    return output_hash;
}

void*
accessHash(hsh* hash_in, const char* key_in) {

    if( hash_in == NULL || 
        key_in == NULL) {
            setError(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return NULL;        
    }
    if(hash_in->len == 0){
        return NULL;
    }

    size_t get_loc = HSH_ERR;
    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(hash_in->kvptrs[i]->key, key_in)) {
            get_loc = i;
            break;
        }
    }
    if(get_loc == HSH_ERR) {
        return NULL;
    }

    setSuccess(FN);
    return hash_in->kvptrs[get_loc]->value;
}

int
addKeyToHash(hsh* hash_in, const char* key_in, void* value_in) {

    if( hash_in == NULL ||
        key_in == NULL ||
        value_in == NULL) {
            setError(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }
    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(hash_in->kvptrs[i]->key, key_in)) {
            setWarning(FN, "key_in already exists in hash_in");
            return 3;
        }
    }

    kv* new_kvp = NULL;
    if(!(new_kvp = malloc(sizeof(kv)))) {
        fatalExit(FN, "Allocate failed for: new_kvp (malloc)");
    }

    size_t key_length = seqLen(key_in);
    if(!key_length) {
        setError(ARG_ERR, FN, "Input argument invalid: key_in (key must contain at least 1 character)");
        free(new_kvp);
        return 5;
    }
    if(!(new_kvp->key = malloc(key_length + 1))) {
        free(new_kvp);
        fatalExit(FN, "Allocate failed for: new_kvp->key (malloc)");
    }
    if(!(new_kvp->value = duplicate(value_in))) {
        setError(MEM_ERR, FN, "Allocate failed for: new_kvp->value (duplicate)");
        free(new_kvp->key);
        free(new_kvp);
        return 7;
    }

    for(size_t i = 0; i < key_length; i++) {
        new_kvp->key[i] = key_in[i];
    }
    new_kvp->key[key_length] = '\0';

    if(hash_in->len == 0) {
        if(!(hash_in->kvptrs = malloc(sizeof(kv**)))) {
            destroyKeyValuePair(new_kvp);
            fatalExit(FN, "Allocate failed for: hash_in->kvptrs (malloc)");
        }
        hash_in->kvptrs[0] = new_kvp;
    } else {
        kv** temp_kvps = NULL;
        if(!(temp_kvps = realloc(hash_in->kvptrs, (hash_in->len + 1) * sizeof(kv**)))) {
            destroyKeyValuePair(new_kvp);
            fatalExit(FN, "Allocate failed for: temp_kvps (realloc)");
        }
        temp_kvps[hash_in->len] = new_kvp;
        hash_in->kvptrs = temp_kvps;
    }
    hash_in->len++;

    setSuccess(FN);
    return 0;
}

int
moveKeyToHash(hsh* hash_in, const char* key_in, void* eating) {

    if( !hash_in ||
        !key_in ||
        !eating) {
            setError(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }
    for(size_t i = 0; i < hash_in->len; i++) {
        if(seqSame(key_in, hash_in->kvptrs[i]->key)) {
            setWarning(FN, "key_in already exists in hash_in");
            return 3;
        }
    }

    kv* new_kvp = NULL;
    if(!(new_kvp = malloc(sizeof(kv)))) {
        setError(MEM_ERR, FN, "Allocate failed for: new_kvp (malloc)");
        return 4;
    }

    size_t key_length = seqLen(key_in);
    if(key_length == 0) {
        setError(ARG_ERR, FN, "Input argument invalid: key_in (key must contain at least 1 character)");
        free(new_kvp);
        return 5;
    }
    if(!(new_kvp->key = seqDupl(key_in))) {
        setError(INIT_ERR, FN, "Initialization failed for: new_kvp->key (seqDupl)");
        free(new_kvp);
        return 6;
    }
    new_kvp->value = eating;

    if(hash_in->len == 0) {
        if(!(hash_in->kvptrs = malloc(sizeof(kv**)))) {
            destroyKeyValuePair(new_kvp);
            fatalExit(FN, "Allocate failed for: hash_in_cast->kvptrs (malloc)");
        }
        hash_in->kvptrs[0] = new_kvp;
    } else {
        kv** temp_kvps = NULL;
        if(!(temp_kvps = realloc(hash_in->kvptrs, (hash_in->len + 1) * sizeof(kv**)))) {
            destroyKeyValuePair(new_kvp);
            fatalExit(FN, "Allocate failed for: temp_kvps (realloc)");
        }
        temp_kvps[hash_in->len] = new_kvp;
        hash_in->kvptrs = temp_kvps;
    }
    hash_in->len++;

    setSuccess(FN);
    return 0;
}

int
removeFromHash(hsh* hash_in, const char* key_delete) {

    if( hash_in == NULL||
        key_delete == NULL) {
            setError(ARG_ERR, FN, "Input object(s) invalid (NULL)");
            return 1;
    }
    if(hash_in->len == 0) {
        setWarning(FN, "Length of hash_in is zero. Aborting lookup");
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
        setWarning(FN, "Key key_delete not found in hash_in");
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

    kv** temp_kvps = NULL;
    if(!(temp_kvps = realloc(hash_in->kvptrs, (hash_in->len - 1) * sizeof(kv**)))) {
        fatalExit(FN, "Allocate failed for: temp_kvps (realloc)");
    }
    hash_in->kvptrs = temp_kvps;
    hash_in->len--;

    setSuccess(FN);
    return 0;
}

int
destroyHash(hsh* hash_in) {

    if(hash_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return 1;
    }

    if(hash_in->len == 0) { //if empty hash
        free(hash_in);
        setSuccess(FN);
        return 0;
    }

    for(size_t i = 0; i < hash_in->len; i++) {
        destroyKeyValuePair(hash_in->kvptrs[i]);
    }
    free(hash_in->kvptrs);
    free(hash_in);

    setSuccess(FN);
    return 0;
}

hsh*
duplicateHash(hsh* hash_in) {

    if(hash_in == NULL) {
        setError(ARG_ERR, FN, "Input object invalid (NULL)");
        return NULL;
    }

    hsh* hash_out = NULL;
    if(!(hash_out = hashInit())) {
        setError(INIT_ERR, FN, "Initialization failed for: hash_out (hashInit)");
        return NULL;
    }
    if(!(hash_out->kvptrs = malloc(hash_in->len * sizeof(kv**)))) {
        free(hash_out);
        fatalExit(FN, "Allocate failed for: hash_out->kvptrs (malloc)");
    }

    for(size_t i = 0; i < hash_in->len; i++) { //duplicate all pairs
        if(!(hash_out->kvptrs[i] = duplicateKeyValuePair(hash_in->kvptrs[i]))) {
            for(size_t j = 0; j < i; j++) {
                setError(INIT_ERR, FN, "Initialization failed for hash_out->kvptrs[i] (duplicateKeyValuePair)");
                destroyKeyValuePair(hash_out->kvptrs[i]);
            }
            destroyHash(hash_out);
            return NULL;
        }
    }
    hash_out->len = hash_in->len;

    setSuccess(FN);
    return hash_out;
}

#endif // HSH_H
