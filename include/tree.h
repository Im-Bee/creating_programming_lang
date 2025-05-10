#ifndef TREE_H
#define TREE_H



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


void DestroyTreeNodes(TreeNode* pHead);


#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !TREE_H
