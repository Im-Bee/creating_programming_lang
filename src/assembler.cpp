#include "../include/allocators.hpp"
#include "../include/tree.h"

#include <cstdio>
#include <string.h>




constexpr int OperatorAdd = 0;
constexpr int OperatorMinus = 1;
constexpr int OperatorMul = 2;
constexpr int OperatorDiv = 3;




static inline constexpr bool is_operator(const char& c) 
{
    return c == '+' || c == '-' || c == '*' || c == '\\';
}



static inline constexpr bool is_number(const char& c) 
{
    return c >= '0' && c <= '9';
}




TreeNode* CreateOperatorNode(const char& c, TreeNode* pParent) 
{
    if (c == '+') {
        return new TreeNode {
            pParent,
            nullptr,
            nullptr,
            EOperator,
            OperatorAdd
        };
    } else if (c == '-') {
        return new TreeNode {
            pParent,
            nullptr,
            nullptr,
            EOperator,
            OperatorMinus
        };
    } else if (c == '*') {
        return new TreeNode {
            pParent,
            nullptr,
            nullptr,
            EOperator,
            OperatorMul
        };
    }
        
    return new TreeNode {
        pParent,
        nullptr,
        nullptr,
        EOperator,
        OperatorDiv
    };
}





TreeNode* CreateNumberNode(TreeNode* pParent)  {
    return new TreeNode {
        pParent,
        nullptr,
        nullptr,
        ENumber,
        0
    };
}




char* TranslateNumber(TreeNode*) 
{
    char* pAsm = new char[128];

    strcpy(pAsm, "xor eax, eax\n");
    strcat(pAsm, "mov eax, 69\n");

    return pAsm;
}




char* TranslateAdd(TreeNode*) 
{
    char* pAsm = new char[128];

    pAsm[0] = 0;

    return pAsm;
}




char* TranslateToAsm(const char* pFileContent)
{
    ELang::DynamicAlloc<char, 256> asmContent = {};

    TreeNode* pHead = new TreeNode {
        nullptr,
        nullptr,
        nullptr,
        ENone,
        0
    };

    TreeNode* pCurrent = pHead;



    for (size_t i = 0; pFileContent[i] != '\0'; ++i) 
    {
        const char& c = pFileContent[i];

        if (is_operator(c)) {
            if (!pCurrent->Left) {
                perror("Stacked operators");
                
                return nullptr;
            }

            pCurrent->Right = CreateOperatorNode(c, pCurrent);
            pCurrent = pCurrent->Right;

            continue;
        }

        if (is_number(c)) {
            if (!pCurrent->Left) {
                pCurrent->Left = CreateNumberNode(pCurrent);
            }

            pCurrent->Left->Value = pCurrent->Left->Value * 10 + (c - '0');
            continue;
        }
    }

    pCurrent = pHead;

    while (pCurrent) {
        if (pCurrent->Left->Operator == ENumber) {
            asmContent.AddStringSlice(TranslateNumber(pCurrent->Left), 128);
            
            pCurrent = pCurrent->Right;
            continue;
        }

        asmContent.AddStringSlice(TranslateAdd(pCurrent), 128);

        pCurrent = pCurrent->Right;
    }





    DestroyTreeNodes(pHead);
    
    asmContent.Append('\0');
    return asmContent.Leak();
}

