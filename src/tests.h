#ifndef TESTS_H
#define TESTS_H

#include "common.h"

int
doodleTests() {
    printf("\n%s%s%s\n\n", KCYN, FN , KNRM);
    str* name = input("Enter name: ", STR_TYPE);
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
    print(accessHashValue(hash1, "Name"));
    print(accessHashValue(hash1, "Age"));
    print(hash1);
    print(vec2);
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
    str* str1 = input("Enter name: ", STR_TYPE);
    moveKeyToHash(hash1, "Name", str1);
    moveKeyToHash(hash1, "Major", input("Enter major: ", STR_TYPE));
    moveKeyToHash(hash1, "GPA", input("Enter GPA: ", FLT_TYPE));
    moveKeyToHash(hash1, "Graduation Year", input("Enter graduation year: ", ITR_TYPE));
    print(hash1);
    hsh* hash2 = hashInit();
    moveKeyToHash(hash2, "Student 1", hash1);
    print(hash2);
    vec* vec1 = vectorInitV(0);
    moveToVectorEnd(vec1, hash2);
    print(vec1);
    destroy(vec1);
    return 0;
}

int
hashInArrPrintTest() {
    printf("\n%s%s%s\n\n", KCYN, FN , KNRM);
    hsh* hash1 = hashInit();
    moveKeyToHash(hash1, "Name", input("Enter name: ", STR_TYPE));
    vec* vec1 = vectorInitV(0);
    moveToVectorEnd(vec1, hash1);
    str* str1 = stringInit("test");
    moveToVectorEnd(vec1, str1);
    print(vec1);
    destroy(vec1);
    return 0;
}

int
floatPrintTest() {
    flt* float1 = input("enter float: ", FLT_TYPE);
    print(float1);
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
    moveKeyToHash(hash1, "testseq", input("enter a string: ", STR_TYPE));
    print(hash1);
    printf("testseq: %s\n", ((str*)accessHashValue(hash1, "testseq"))->seq);
    destroy(vec1);
    destroy(hash1);
    free(tmp_name);
    return 0;
}

int loadVectorWithStrings() {

    vec* vec1 = vectorInitV(0);
    while(true) {
        moveToVectorEnd(vec1, input(NULL, STR_TYPE));
        if(seqSame("END", ((str*)vec1->ptrs[vec1->len - 1])->seq))
            break;
    }
    print(vec1);
    truncateVector(vec1, vec1->len - 1);
    print(vec1);
    destroy(vec1);
    return 0;
}

int stringTest() {
    str* str1 = stringInit("test");
    str* str2 = stringInit("test2");
    addString(str1, str2);
    print(str1);
    destroyV(2, str1, str2);
    return 0;
}

#endif // TESTS_H
