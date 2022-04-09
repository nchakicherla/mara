#ifndef GEN_H
#define GEN_H

#include "../common.h"

#define NUM_CHAR_BUF_LEN 128

int
destroy(void* obj_in) {

    switch(TYPE_OF(obj_in)) {

        case STR_TYPE: {
            destroyString(obj_in);
            break;
        }
        case ITR_TYPE: {
            free(obj_in);
            break;
        }
        case FLT_TYPE: {
            free(obj_in);
            break;
        }
        case HSH_TYPE: {
            destroyHash(obj_in);
            break;
        }
        case VEC_TYPE: {
            destroyVector(obj_in);
            break;
        }
        case MAT_TYPE: {
            destroyMatrix(obj_in);
            break;
        }
        default: {
            WARN(FN, "Input object type invalid: not defined");
        }
    }

    SUCCESS(FN);
    return 0;
}

int
destroyV(uint32_t n, ...) {

    va_list obj_args;
    va_start(obj_args, n);
    void* current_obj = NULL;

    for(uint32_t i = 0; i < n; i++) {
        current_obj = va_arg(obj_args, void *);
        destroy(current_obj);
    }

    return 0;
}

void*
duplicate(void* obj_in) {

    void* obj_out = NULL;

    switch(TYPE_OF(obj_in)) {

        case STR_TYPE: {
            obj_out = duplicateString(obj_in);
            break;
        }
        case ITR_TYPE: {
            obj_out = duplicateInteger(obj_in);
            break;
        }
        case FLT_TYPE: {
            obj_out = duplicateFloat(obj_in);
            break;
        }
        case HSH_TYPE: {
            obj_out = duplicateHash(obj_in);
            break;
        }
        case VEC_TYPE: {
            obj_out = duplicateVector(obj_in);
            break;
        }
        case MAT_TYPE: {
            obj_out = duplicateMatrix(obj_in);
            break;
        }
    }

    SUCCESS(FN);
    return obj_out;
}

size_t
printedLength(void* obj_in, size_t nested_level, bool oneline) {

    char* num_char_buf = NULL; //only used in ITR or FLT cases
    int spf_ret = 0;
    size_t output_count = 0;

    switch(TYPE_OF(obj_in)) {

        case STR_TYPE: {
            output_count += ((str*)obj_in)->len + 2; //including ""
            break;
        }
        case ITR_TYPE: {
            if(!(num_char_buf = calloc(NUM_CHAR_BUF_LEN, sizeof(char)))) { // for safety if malloc fails
                return 32;
            }
            spf_ret = sprintf(num_char_buf, "%ld", ((itr*)obj_in)->val);
            if(spf_ret < 0) {
                return 0;
            } else {
                output_count += spf_ret;
            }
            break;
        }

        case FLT_TYPE: {
            if( !(num_char_buf = calloc(NUM_CHAR_BUF_LEN, sizeof(char))) ) { // for safety if malloc fails
                return 128;
            }
            if( ((flt*)obj_in)->val < 0.001 && ((flt*)obj_in)->fracdigs < 20 ) { //handle small numbers

                spf_ret = sprintf(num_char_buf, "%e", ((flt*)obj_in)->val);
                char* e_loc = seqChar(num_char_buf, 'e', NUM_CHAR_BUF_LEN);
                int exponent = strtol(e_loc + 2, NULL, 10);
                char* decimal_loc = seqChar(num_char_buf, '.', NUM_CHAR_BUF_LEN);
                char* char_it = NULL;
                size_t extra_chars = 0;
                
                if(*(e_loc + 1) == '-') {
                    char_it = e_loc + 2;
                } else {
                    char_it = e_loc + 1;
                }
                while(*char_it == '0') {
                    extra_chars++;
                    char_it++;
                }
                char_it = decimal_loc + (size_t)(((flt*)obj_in)->fracdigs - exponent); //start at first redundant 0
                while(*(char_it + 1) != 'e') {
                    extra_chars++;
                    char_it++;
                }
                spf_ret -= extra_chars;
            } else {
                spf_ret = sprintf(num_char_buf, "%.*f", ((flt*)obj_in)->fracdigs, ((flt*)obj_in)->val);
            }
            if(spf_ret < 0) {
                return 0;
            } else {
                output_count += spf_ret;
                if(((flt*)obj_in)->fracdigs == 0) {
                    output_count++; //include room for decimal to indicate float type
                }
            }
            break;
        }

        case HSH_TYPE: {
            if(((hsh*)obj_in)->len == 0) {
                return 9; // { EMPTY }
            }
            output_count++; //include {
            for(size_t i = 0; i < ((hsh*)obj_in)->len; i++) {
                kvp* kvp_cast = (kvp*) ((hsh*)obj_in)->kvptrs[i];
                if(!oneline) {
                    output_count += 3; //include leading newline and 2 leading spaces if default
                } else {
                    output_count++; //only include 1 space if oneline override is true
                }
                if(nested_level > 0 && !oneline) { //add 2 additional spaces before key for each nest level
                    output_count += 2 * (nested_level);
                }
                output_count += seqLen(kvp_cast->key); //no need to include quotation space for kvp->key
                output_count += 4; //space, separator, space, comma
                output_count += printedLength(kvp_cast->value, nested_level + 1, oneline);
            }
            if(nested_level > 0 && !oneline) { //include additional indentation for close bracket if nested
                for(size_t i = 0; i < nested_level; i++) {
                    output_count += 2;
                }
            } else {
                output_count++; //include space
            }
            output_count++; //include }
            break;
        }

        case VEC_TYPE: {
            if(!((vec*)obj_in)->ptrs) {
                return 0;
            }
            output_count += 2; //including [ and ] and final space, exclude last comma
            for(size_t i = 0; i < ((vec*)obj_in)->len; i++) {
                output_count += printedLength(((vec*)obj_in)->ptrs[i], false, true);
                output_count += 2; // including leading space, comma
            }
            break;    
        }
    }
    if(num_char_buf) {
        free(num_char_buf);
    }
    return output_count;
}

char*
objectAsChars(void* obj_in, size_t nested_level, bool oneline) {

    size_t output_seq_size = printedLength(obj_in, nested_level, oneline);  // updates to printing should be reflected
                                                                            // in both printedLength and this fn
    char* output_seq = NULL; // dynamically allocated and returned, freed outside scope 
    if(!(output_seq = calloc(output_seq_size + 1, sizeof(char)))) {
        throwFatal(FN, "Allocate failed for: output_seq (calloc)");
    }
    char* write_tracker = output_seq;
    char* nested_obj_seq = NULL; // dynamically allocated, freed within scope
    int spr_ret = 0;

    switch(TYPE_OF(obj_in)) {

        case STR_TYPE: {
            if(!((str*)obj_in)->seq) {
                ERROR(ARG_ERR, FN, "Input object invalid: ((str*)obj_in)->seq = NULL");
                free(output_seq);
                return NULL;
            }
            *write_tracker = '\"';
            write_tracker++;
            if(((str*)obj_in)->len > 0) {
                spr_ret = sprintf(write_tracker, "%s", ((str*)obj_in)->seq);
                if(spr_ret > 0) {
                    write_tracker += spr_ret;
                }                
            } 
            *write_tracker = '\"';
            break;
        }
        case ITR_TYPE: {
            spr_ret = sprintf(output_seq, "%ld", ((itr*)obj_in)->val);
            break;
        }

        case FLT_TYPE: {

            if( ((flt*)obj_in)->val < 0.001) { //handle small numbers
                char* num_char_buf = NULL;
                if(!(num_char_buf = calloc(NUM_CHAR_BUF_LEN, sizeof(char)))) {
                    free(output_seq);
                    throwFatal(FN, "Allocate failed for: num_char_buf (calloc)");
                }
                spr_ret = sprintf(num_char_buf, "%e", ((flt*)obj_in)->val); //
                char* e_loc = seqChar(num_char_buf, 'e', NUM_CHAR_BUF_LEN);
                char* char_it = e_loc + 2;
                int exponent = strtol(char_it, NULL, 10);
                sprintf(char_it, "%d", exponent);
                char_it = seqChar(num_char_buf, '.', NUM_CHAR_BUF_LEN) + (size_t)(((flt*)obj_in)->fracdigs - exponent) + 1;
                size_t remaining_extra_chars = e_loc - char_it;
                
                while(*(char_it + remaining_extra_chars) != '\0') {
                    *char_it = *(char_it + remaining_extra_chars);
                    char_it++;
                }
                sprintf(output_seq, "%s", num_char_buf);
                free(num_char_buf);
            } else {
                spr_ret = sprintf(output_seq, "%.*f", ((flt*)obj_in)->fracdigs, ((flt*)obj_in)->val);
                if(spr_ret > 0) {
                    if(((flt*)obj_in)->fracdigs == 0) {
                        sprintf(output_seq + (size_t)spr_ret, ".");
                    }
                } 
            }
            break;
        }

        case HSH_TYPE: {

            if(((hsh*)obj_in)->len == 0) { //empty hash
                sprintf(write_tracker, "%s", "{ EMPTY }");
                break;
            }

            *write_tracker = '{';
            write_tracker++;

            for(size_t i = 0; i < ((hsh*)obj_in)->len; i++) {

                if(!oneline) {
                    sprintf(write_tracker, "%s", "\n  ");
                    write_tracker += 3;
                } else {
                    sprintf(write_tracker, "%s", " ");
                    write_tracker++;
                }

                if(nested_level > 0 && !oneline) {
                    for(size_t i = 0; i < nested_level; i++) {
                        sprintf(write_tracker, "%s", "  "); //add 2 * nested_level additional spaces if nested in another DCT
                        write_tracker += 2;
                    }
                }

                spr_ret = sprintf(write_tracker, "%s", ((hsh*)obj_in)->kvptrs[i]->key);

                if(spr_ret > 0) {
                    write_tracker += spr_ret;
                } else {
                    write_tracker++;
                }

                sprintf(write_tracker, "%s", " : ");
                write_tracker += 3;

                if((nested_obj_seq = objectAsChars(((hsh*)obj_in)->kvptrs[i]->value, nested_level + 1, oneline))) {
                    spr_ret = sprintf(write_tracker, "%s", nested_obj_seq);
                    free(nested_obj_seq);                    
                } else {
                    WARN(FN, "Failed to create seq from ((kvp*)((hsh*)obj_in)->kvptrs[i])->value");
                    spr_ret = sprintf(write_tracker, "%s", "");
                }
                write_tracker += spr_ret;
                *write_tracker = ',';
                write_tracker++;
            }

            if(!oneline) {
                if(nested_level > 0) { //maintain nested indentation if normal printing
                    write_tracker -= 1;
                    *write_tracker = '\n';
                    write_tracker++;
                    for(size_t i = 0; i < nested_level; i++) {
                        *write_tracker = ' ';
                        *(write_tracker + 1) = ' ';
                        write_tracker += 2;
                    }
                    *write_tracker = '}';
                } else {
                    *(write_tracker - 1) = '\n';
                    *write_tracker = '}';
                }
            } else { //don't indent or add newlines if oneline
                if(*(write_tracker - 1) != '{') {
                    write_tracker--;
                }
                *write_tracker = ' ';
                *(write_tracker + 1) = '}';
            }
            break;
        }
        
        case VEC_TYPE: {

            *write_tracker = '[';
            write_tracker++;

            for(size_t i = 0; i < ((vec*)obj_in)->len; i++) {
                *write_tracker = ' '; //leading space
                write_tracker++;
                if((nested_obj_seq = objectAsChars(((vec*)obj_in)->ptrs[i], nested_level, oneline))) {
                    spr_ret = sprintf(write_tracker, "%s", nested_obj_seq);
                    free(nested_obj_seq);
                } else {
                    WARN(FN, "Failed to create seq from ((vec*)obj_in)->ptrs[i]");
                    spr_ret = sprintf(write_tracker, "%s", "");
                }
                write_tracker += spr_ret;

                *write_tracker = ',';
                write_tracker++;
            }
            *(write_tracker - 1) = ' '; //overwrite last comma
            *write_tracker = ']';
            break;
        }
    }
    return output_seq;
    output_seq[output_seq_size] = '\0'; // for security, should already be reserved for \0
}

#endif // GEN_H
