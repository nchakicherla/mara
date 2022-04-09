#include "mara.h"

int main()
{
    START_LOG();

    hsh* hash1 = hashInit();
    moveKeyToHash(hash1, "Name", input("Please enter your name: ", STR_TYPE));
    hsh* hash2 = duplicate(hash1);
    print(hash1);
    print(hash2);
    changeKey(hash2, "Name", "newKey");
    vec* vec1 = vectorInitV(0);
    moveToVectorEnd(vec1, hash1);
    moveToVectorEnd(vec1, hash2);
    print(vec1);
    destroyV(1, vec1);

    END_LOG();
    return 0;
}