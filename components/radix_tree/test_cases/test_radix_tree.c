
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../radix_tree.h"

int WalkCallBack(char *str)
{
    if (NULL != str)
    {
        printf("%s\n", str);
    }
    
    return 0;
}

int ReleaseCallBack(char *str)
{
    return 0;
}




int main(int argc, char *argv[])
{
    RADIX_NODE *root = calloc(sizeof(RADIX_NODE), 1);
    
    RadixInsert(root, "1001");
    RadixInsert(root, "101");
    RadixInsert(root, "001");
    RadixInsert(root, "0001");
    RadixInsert(root, "11001");
    RadixInsert(root, "10011");
    RadixInsert(root, "011001");
    RadixInsert(root, "111001");
    RadixInsert(root, "1111001");
    RadixInsert(root, "abcdef");

    RadixWalkAll(root, WalkCallBack);
    RadixRelease(root, ReleaseCallBack);
        
    return 0;
}

