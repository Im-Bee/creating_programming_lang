#include "../include/tree.h"

#include <stdlib.h>



void DestroyTreeNodes(TreeNode* pHead)
{
    if (!pHead) {
        return;
    }

    if (pHead->Right) {
        DestroyTreeNodes(pHead->Right);
    }
    
    if (pHead->Left) {
        DestroyTreeNodes(pHead->Left);
    }
    
    free(pHead);
}

