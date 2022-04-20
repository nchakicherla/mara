#include "mara.h"

int main()
{
    START_LOG();

    loadVectorWithStrings();

    END_LOG();
    return 0;
}

/*
    hsh* hash1 = hashInit();
    moveKeyToHash(hash1, "Name", mInput("Please enter your name: ", STR_TYPE));
    hsh* hash2 = duplicate(hash1);
    mPrint(hash1);
    mPrint(hash2);
    changeKey(hash2, "Name", "newKey");
    vec* vec1 = vectorInitV(0);
    moveToVectorEnd(vec1, hash1);
    moveToVectorEnd(vec1, hash2);
    mPrint(vec1);
    mPrint("this is a test");
    mPrintV(3, NAME(vec1), ": ", vec1);
    destroyV(1, vec1);
*/
