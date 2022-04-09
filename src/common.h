#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdarg.h>
#include <float.h>
#include <time.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KGRN  "\x1B[32m"
#define KCYN  "\x1B[36m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// INTERFACE LIMITS



//  DEBUG OPTIONS

#ifdef ENABLE_LOG
# define START_LOG()        openLog()
# define END_LOG()          closeLog()
#else
# define START_LOG()        //skip function call
# define END_LOG()          //skip function call
#endif

#ifdef PRINTLOG
# define PRINTLOG           true
#else
# define PRINTLOG           false
#endif

#ifdef LOG_WARNING
# define WARN(a, b) setWarning(a, b)
#else
# define WARN(a, b) //skip function call
#endif

#ifdef LOG_ERROR
# define ERROR(a, b, c) setError(a, b, c)
#else
# define ERROR(a, b, c) //skip function call
#endif

#ifdef LOG_SUCCESS
# define SUCCESS(a) setSuccess(a)
#else
# define SUCCESS(a) //skip function call
#endif


// ERROR LOGGING

#define ARG_ERR             "ARG_ERR"
#define ACC_ERR             "ACC_ERR"
#define INIT_ERR            "INIT_ERR"
#define MEM_ERR             "MEM_ERR"

#define FN                  __func__

//  TYPES AND BOUNDS

#define TYPE_OF(arg)       *(int *)arg

#define STR_TYPE            1
#define ITR_TYPE            2
#define FLT_TYPE            3
#define HSH_TYPE            4
#define VEC_TYPE            5
#define ARR_TYPE            6
#define MAT_TYPE			7

#define OBJ_TYPE_MAX        OBJ_TYPE_ERR - 1
#define OBJ_TYPE_ERR        INT_MAX

#define STR_MAX             STR_ERR - 1
#define STR_ERR             SIZE_MAX

#define ITR_MIN             LONG_MIN
#define ITR_MAX             LONG_MAX

#define VEC_MAX             VEC_ERR - 1
#define VEC_ERR             SIZE_MAX

#define ARR_MAX             ARR_ERR - 1
#define ARR_ERR             SIZE_MAX

#define HSH_MAX             HSH_ERR - 1
#define HSH_ERR             SIZE_MAX

//  TYPE_OF DEFINITIONS

typedef struct
    str {
        int         type;
        size_t      len;
        char*       seq;
}   str;

typedef struct
    itr {
        int         type;
        long        val;
}   itr;

typedef struct
    flt {
        int         type;
        double      val;
        int         fracdigs; //number of digits after decimal when printed, max 15
}   flt;

typedef struct //key-value pair, unique to DCT, no NO TYPE_OF MEMBER
    kvp {
        char*       key;
        void*       value;
}   kvp;

typedef struct
    hsh {
        int         type;
        size_t      len;
        kvp**       kvptrs;
}   hsh;

typedef struct
    vec {
        int         type;
        size_t      len;
        void**      ptrs;
}   vec;

typedef struct
    arr {
        int         type;
        int         subtype;
        void*       data;
}   arr;

typedef struct
	mat {
		int 		type;
		int 		datatype;
		size_t		m;
		size_t		n;
		void*		data;
}	mat;


////  FUNCTION DECLARATIONS
////


// ERROR

int
openLog();

int
logMessage(int n, ...);

int
closeLog();

void
throwFatal(const char* func, const char* desc);

int
setSuccess(const char* func);

int
setWarning(const char* func, const char* desc);

int
setError(char* errtype, const char* func, const char* desc);


// STR

size_t
seqLen(const char* seq_in);

char*
seqChar(char* seq_in, char c, size_t len);

char*
seqDupl(const char* seq_in);

itr*
seqToInteger(char* seq_in);

flt*
seqToFloat(char* seq_in);

bool
seqSame(const char* target, const char* comp);

int
setString(str* str_in, const char* seq_in);

str*
stringInit(const char* seq_in);

int
destroyString(str* str_in);

int
growString(str* str_in, size_t new_len);

int
addString(str* target, str* adding);

int
insertString(str* target, str* adding, size_t pos);

size_t
findString(str* target, str* find);

str*
duplicateString(str* input_str);

str*
sliceFromString(str* input_str, size_t start, size_t end);

// ITR

itr*
integerInit(long val);

int
destroyInteger(itr* itr_in);

itr*
duplicateInteger(itr* itr_in);

// FLT

flt*
floatInit(double val_in, int prec_in);

int
destroyFloat(flt* flt_in);

flt*
duplicateFloat(flt* flt_in);

// DCT

int
destroyKeyValuePair(kvp* kvp_in);

kvp*
duplicateKeyValuePair(kvp* kvp_in);

hsh*
hashInit();

void*
accessHashValue(hsh* hash_in, const char* key_access);

kvp*
accessHashPair(hsh* hash_in, const char* key_access);

int
changeKey(hsh* hash_in, const char* key_current, const char* key_new);

int
addKeyToHash(hsh* hash_in, const char* key_in, void* value_in);

int
moveKeyToHash(hsh* hash_in, const char* key_in, void* eating);

int
removeFromHash(hsh* hash_in, const char* key_delete);

int
destroyHash(hsh* hash_in);

hsh*
duplicateHash(hsh* hash_in);

// ARR

size_t
vecayLength(void* vec_in);

int
destroyVector(vec* vec_in);

int
removeFromVector(vec* vec_in, size_t pos);

void*
accessVector(vec* vec_in, size_t loc);

vec*
vectorInitV(size_t len, ...);

int
growVectorPointers(vec* vec_in, size_t new_len);

int
truncateVector(vec* vec_in, size_t new_len);

int
appendToVector(vec* target, void* adding);

int
moveToVectorEnd(vec* target, void* eating);

int
mergeVectorsAt(vec* target, vec* adding, size_t pos);

int
addToVectorAt(vec* target, void* obj_in, size_t pos);

vec*
duplicateVector(vec* vec_in);

// MAT

mat*
matrixInit(int data_type_in, size_t rows, size_t columns, const char* options);

int
destroyMatrix(mat* mat_in);

mat*
duplicateMatrix(mat* mat_in);

// GENERIC

int
destroy(void* obj_in);

int
destroyV(uint32_t n, ...);

void*
duplicate(void* obj_in);

size_t
printedLength(void* obj_in, size_t nested_level, bool oneline);

char*
objectAsChars(void* obj_in, size_t nested_level, bool oneline);

// INTERFACE

char*
inputSeq(const char* prompt);

void*
input(const char* prompt, int TYPE_OUT);

int
print(void* obj_in);

#endif // COMMON_H
