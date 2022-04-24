#ifndef TESTS_H
#define TESTS_H

#include "common.h"

int
doodleTests() {
    printf("\n%s%s%s\n\n", KCYN, FN , KNRM);
    str* name = mInput("Enter name: ", STR_TYPE);
    vec* vec1 = vectorInitV(1, name);
    vec* vec2 = duplicateVector(vec1);
    appendToVector(vec2, name);
    hsh* hash1 = hashInit();
    itr* itr1 = integerInit(26);
    addKeyToHash(hash1, "Name", name);
    moveKeyToHash(hash1, "Age", itr1); // no need to call destroy on itr1
    addKeyToHash(hash1, "test", name);
    removeFromHash(hash1, "test");
    printf("Value of \"Name\" key in hash1: %s\n", ((str*)accessHashValue(hash1, "Name"))->seq);
    printf("Value of \"Age\" key in hash1: %ld\n", ((itr*)accessHashValue(hash1, "Age"))->val);
    mPrint(accessHashValue(hash1, "Name"));
    mPrint(accessHashValue(hash1, "Age"));
    mPrint(hash1);
    mPrint(vec2);
    mat* mat1 = matrixInit(FLT_TYPE, 100, 100, "unit");
    destroyV(5,  name, 
                        vec1, 
                        vec2,  
                        hash1, 
                        mat1);
    return 0;
}

int
studentRecordTest() {
    printf("\n%s%s%s\n\n", KCYN, FN , KNRM);
    hsh* hash1 = hashInit();
    str* str1 = mInput("Enter name: ", STR_TYPE);
    moveKeyToHash(hash1, "Name", str1);
    moveKeyToHash(hash1, "Major", mInput("Enter major: ", STR_TYPE));
    moveKeyToHash(hash1, "GPA", mInput("Enter GPA: ", FLT_TYPE));
    moveKeyToHash(hash1, "Graduation Year", mInput("Enter graduation year: ", ITR_TYPE));
    mPrint(hash1);
    hsh* hash2 = hashInit();
    moveKeyToHash(hash2, "Student 1", hash1);
    mPrint(hash2);
    vec* vec1 = vectorInitV(0);
    moveToVectorEnd(vec1, hash2);
    mPrint(vec1);
    destroy(vec1);
    return 0;
}

int
hashInArrPrintTest() {
    printf("\n%s%s%s\n\n", KCYN, FN , KNRM);
    hsh* hash1 = hashInit();
    moveKeyToHash(hash1, "Name", mInput("Enter name: ", STR_TYPE));
    vec* vec1 = vectorInitV(0);
    moveToVectorEnd(vec1, hash1);
    str* str1 = stringInit("test");
    moveToVectorEnd(vec1, str1);
    mPrint(vec1);
    destroy(vec1);
    return 0;
}

int
floatPrintTest() {
    flt* float1 = mInput("enter float: ", FLT_TYPE);
    mPrint(float1);
    destroy(float1);
    return 0;
}

int hashStressTest() {
    vec* vec1 = vectorInitV(0);
    for(int i = 0; i < 100; i++) {
        moveToVectorEnd(vec1, integerInit(i));
    }
    hsh* hash1 = hashInit();
    char* tmp_name = calloc(16, sizeof(char));
    sprintf(tmp_name, "key");
    vec* temp_vec = NULL;
    for(int i = 0; i < 100; i++) {
        sprintf(tmp_name + 3, "%d", i);
        temp_vec = duplicateVector(vec1);
        moveKeyToHash(hash1, tmp_name, temp_vec);
    }
    moveKeyToHash(hash1, "testseq", mInput("enter a string: ", STR_TYPE));
    mPrint(hash1);
    printf("testseq: %s\n", ((str*)accessHashValue(hash1, "testseq"))->seq);
    destroy(vec1);
    destroy(hash1);
    free(tmp_name);
    return 0;
}

int loadVectorWithStrings() {

    vec* vec1 = vectorInitV(0);
    flt* temp_flt = NULL;

    while(NULL != (temp_flt = mInput("Enter a float: ", FLT_TYPE))) {
        moveToVectorEnd(vec1, temp_flt);
        temp_flt = NULL;
    }
    
    mPrint(vec1);
        
    destroy(vec1);
    return 0;
}

int stringTest() {
    str* str1 = stringInit("test");
    str* str2 = stringInit("test2");
    addString(str1, str2);
    mPrint(str1);
    destroyV(2, str1, str2);
    return 0;
}

#endif // TESTS_H
