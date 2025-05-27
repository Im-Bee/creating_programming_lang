#include "../include/tree.h"

#include <stdlib.h>






TreeNode* GetLeft(TreeNode* pNode)
{
    if (!pNode) {
        return NULL;
    }

    return pNode->Left;
}


TreeNode* GetRight(TreeNode* pNode)
{
    if (!pNode) {
        return NULL;
    }

    return pNode->Right;
}


TreeBool EqTreeType(TreeNode* pNode, TreeTypes type)
{
    if (!pNode) {
        return 0;
    }

    return pNode->Operator == type;
}


TreeBool EqValue(TreeNode* pNode, int value)
{
    if (!pNode) {
        return 0;
    }

    return pNode->Value == value;
}



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

