#include "../include/tree.h"

#include <stdlib.h>




void DestroyLeaf(TreeNode** pLeaf)
{
    if ((*pLeaf)->Left) {
        DestroyLeaf(&(*pLeaf)->Left);
    }
    if ((*pLeaf)->Right) {
        DestroyLeaf(&(*pLeaf)->Right);
    }

    (*pLeaf) = NULL;
    free(*pLeaf);
}



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

