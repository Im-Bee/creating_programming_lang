#ifndef TREE_H
#define TREE_H


typedef unsigned char TreeBool;


typedef enum TreeTypes {
    ENone = 1,
    EOperator = ENone << 1,
    ENumber = EOperator << 1,
} TreeTypes;



typedef struct TreeNode {
    struct TreeNode* Parent;
    struct TreeNode* Left;
    struct TreeNode* Right;

    TreeTypes Operator;
    int       Value;
} TreeNode;




#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus


TreeNode* GetLeft(TreeNode* pNode);


TreeNode* GetRight(TreeNode* pNode);


TreeBool EqTreeType(TreeNode* pNode, TreeTypes type);


TreeBool EqValue(TreeNode* pNode, int value);


void DestroyLeaf(TreeNode** pLeaf);


void DestroyTreeNodes(TreeNode* pHead);


#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !TREE_H
