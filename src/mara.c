#include "mara.h"

int main()
{
    openLog();

    hsh* hash1 = hashInit();
    moveKeyToHash(hash1, "Name", input("Please enter your name: ", STR_TYPE));
    print(hash1);
    destroy(hash1);

    closeLog();
    return 0;
}