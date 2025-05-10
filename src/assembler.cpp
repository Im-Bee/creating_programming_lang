#include "../include/allocators.hpp"
#include "../include/assembler.h"
#include "../include/tree.h"

#include <cstdio>
#include <cstdlib>




constexpr int OperatorAdd = 0;
constexpr int OperatorMinus = 1;
constexpr int OperatorMul = 2;
constexpr int OperatorDiv = 3;



constexpr size_t TranslatedStringsMaxLenght = 32;



static inline constexpr bool is_operator(const char& c) 
{
    return c == '+' || c == '-' || c == '*' || c == '\\';
}



static inline constexpr bool is_number(const char& c) 
{
    return c >= '0' && c <= '9';
}






ELang::RegistersAllocator::~RegistersAllocator() 
{
    if (m_pRegistersNames) {
        for (size_t i = 0; i < m_uAmountOfRegisters; ++i) {
            if (!m_pRegistersNames[i]) {
                continue;
            }

            free(m_pRegistersNames[i]);
        }

        free(m_pRegistersNames);
    }
}



char* ELang::RegistersAllocator::AllocRegister() 
{
    if (m_uAlloced >= m_uAmountOfRegisters) {
        throw; // TODO
    }

    return m_pRegistersNames[m_uAlloced++];   
}




void ELang::RegistersAllocator::FreeRegister()
{
    if (m_uAlloced-- == 0) {
        throw; // TODO
    }
}



char* ELang::RegistersAllocator::GetNthFromBack(const size_t& uN)
{
    if (uN > m_uAlloced) {
        throw; // TODO
    }

    return m_pRegistersNames[m_uAlloced - uN];
}






TreeNode* CreateOperatorNode(const char& c, TreeNode* pParent) 
{
    pParent->Operator = EOperator;


    if (c == '+') {
        pParent->Value = OperatorAdd;
    } else if (c == '-') {
        pParent->Value = OperatorMinus;
    } else if (c == '*') {
        pParent->Value = OperatorMul;
    } else {
        pParent->Value = OperatorDiv;
    }
        

    return new TreeNode {
        pParent,
        nullptr,
        nullptr,
        ENone,
        0
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




char* TranslateNumber(TreeNode* pNode, ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "mov %s, %d\n", allocator.AllocRegister(), pNode->Value);

    return pAsm;
}



char* TranslateAdd(ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "add %s, %s\n", allocator.GetNthFromBack(2), allocator.GetNthFromBack(1));

    allocator.FreeRegister();

    return pAsm;
}



char* TranslateMinus(ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "sub %s, %s\n", allocator.GetNthFromBack(2), allocator.GetNthFromBack(1));

    allocator.FreeRegister();

    return pAsm;
}




char* TranslateToAsm(const char* pFileContent)
{
    ELang::DynamicAlloc<char, 256> asmContent = {};
    const char* registers[4] = {
        "eax",
        "ebx",
        "ecx",
        "edx"
    };
    ELang::RegistersAllocator allocator(registers);

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
        if (pCurrent->Operator == EOperator) {
            if (pCurrent->Left && 
                pCurrent->Left->Operator == ENumber) 
            {
                asmContent.AddStringSlice(
                        TranslateNumber(pCurrent->Left, allocator),
                        TranslatedStringsMaxLenght
                );
            }


            if (pCurrent->Right && 
                pCurrent->Right->Left->Operator == ENumber) 
            {
                asmContent.AddStringSlice(
                        TranslateNumber(pCurrent->Right->Left, allocator),
                        TranslatedStringsMaxLenght
                );

                pCurrent->Right->Left = nullptr;
            }

            if (pCurrent->Value == OperatorAdd) {
                asmContent.AddStringSlice(
                        TranslateAdd(allocator),
                        TranslatedStringsMaxLenght
                        );
            } else if (pCurrent->Value == OperatorMinus) {
                asmContent.AddStringSlice(
                        TranslateMinus(allocator),
                        TranslatedStringsMaxLenght
                        );
            }
        }

        pCurrent = pCurrent->Right;
    }





    DestroyTreeNodes(pHead);
    
    asmContent.Append('\0');
    return asmContent.Leak();
}

