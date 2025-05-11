#include "../include/allocators.hpp"
#include "../include/assembler.h"
#include "../include/tree.h"

#include <cstdio>
#include <cstdlib>




constexpr int OperatorAdd = 0;
constexpr int OperatorSub = 1;
constexpr int OperatorMul = 2;
constexpr int OperatorDiv = 3;



constexpr size_t TranslatedStringsMaxLenght = 32;



static const char* Registers[4] = {
    "eax",
    "ebx",
    "ecx",
    "edx"
};



// Predeclarations -----------------------------------------------------------------------------------------------------

static char* NodesToAsm(TreeNode** pHead, ELang::RegistersAllocator& allocator);

// ---------------------------------------------------------------------------------------------------------------------






// Functions for checking equality -------------------------------------------------------------------------------------

static inline constexpr bool IsOperator(const char& c) 
{
    return c == '+' || c == '-' || c == '*' || c == '\\';
}



static inline constexpr bool IsNumber(const char& c) 
{
    return c >= '0' && c <= '9';
}



static inline int GetPriority(const int& nodeOperator)
{
    if (nodeOperator == OperatorMul || nodeOperator == OperatorDiv) {
        return 2;
    }

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------






// ELang::RegistersAllocator -------------------------------------------------------------------------------------------

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
        return m_pRegistersNames[m_uAlloced];
    }

    return m_pRegistersNames[m_uAlloced - uN];
}

// ---------------------------------------------------------------------------------------------------------------------





// Nodes creation ------------------------------------------------------------------------------------------------------

static TreeNode* CreateOperatorNode(const char& c, TreeNode* pParent, TreeNode*) 
{
    pParent->Operator = EOperator;

    if (c == '+') {
        pParent->Value = OperatorAdd;
    } else if (c == '-') {
        pParent->Value = OperatorSub;
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



static TreeNode* CreateNumberNode(TreeNode* pParent)  {
    return new TreeNode {
        pParent,
        nullptr,
        nullptr,
        ENumber,
        0
    };
}

// ---------------------------------------------------------------------------------------------------------------------





// Translation to assembly ---------------------------------------------------------------------------------------------

static char* TranslateNumber(TreeNode* pNode, ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "mov %s, %d\n", allocator.AllocRegister(), pNode->Value);

    return pAsm;
}



static char* TranslateAdd(ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "add %s, %s\n", allocator.GetNthFromBack(2), allocator.GetNthFromBack(1));

    allocator.FreeRegister();

    return pAsm;
}



static char* TranslateSub(ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "sub %s, %s\n", allocator.GetNthFromBack(2), allocator.GetNthFromBack(1));

    allocator.FreeRegister();

    return pAsm;
}



static char* TranslateMul(ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "imul %s, %s\n", allocator.GetNthFromBack(2), allocator.GetNthFromBack(1));

    allocator.FreeRegister();

    return pAsm;
}



static char* TranslateDiv(ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];

    sprintf(pAsm, "idiv %s, %s\n", allocator.GetNthFromBack(2), allocator.GetNthFromBack(1));

    allocator.FreeRegister();

    return pAsm;
}

// ---------------------------------------------------------------------------------------------------------------------






static void HandleOperator(TreeNode* pCurrent, 
                           ELang::RegistersAllocator& allocator,
                           ELang::DynamicAlloc<char, 256>& asmContent)
{
    if (pCurrent->Left && 
        pCurrent->Left->Operator == ENumber) 
    {
        asmContent.AddStringSlice(
                TranslateNumber(pCurrent->Left, allocator),
                TranslatedStringsMaxLenght
        );
    }





    if (pCurrent && 
        pCurrent->Right &&
        GetPriority(pCurrent->Right->Value) > GetPriority(pCurrent->Value))
    {
        asmContent.AddStringSlice(
                NodesToAsm(&pCurrent->Right, allocator),
                2048
        );
    }





    if (pCurrent->Right && 
        pCurrent->Right->Left && 
        pCurrent->Right->Left->Operator == ENumber) 
    {
        asmContent.AddStringSlice(
                TranslateNumber(pCurrent->Right->Left, allocator),
                TranslatedStringsMaxLenght
        );
            
        DestroyLeaf(&pCurrent->Right->Left);
    }




    if (pCurrent->Value == OperatorAdd) {
        asmContent.AddStringSlice(
                TranslateAdd(allocator),
                TranslatedStringsMaxLenght
        );
    } else if (pCurrent->Value == OperatorSub) {
        asmContent.AddStringSlice(
                TranslateSub(allocator),
                TranslatedStringsMaxLenght
        );
    } else if (pCurrent->Value == OperatorMul) {
        asmContent.AddStringSlice(
                TranslateMul(allocator),
                TranslatedStringsMaxLenght
        );
    } else if (pCurrent->Value == OperatorDiv) {
        asmContent.AddStringSlice(
                TranslateDiv(allocator),
                TranslatedStringsMaxLenght
        );
    }

    pCurrent->Operator = ENone;
}






static char* NodesToAsm(TreeNode** pHead, ELang::RegistersAllocator& allocator)
{
    ELang::DynamicAlloc<char, 256> asmContent = {};

    TreeNode* pCurrent = *pHead;

    while (pCurrent) {
        if (pCurrent->Operator == EOperator) {
            HandleOperator(pCurrent, allocator, asmContent);

            if (pCurrent && 
                pCurrent->Right &&
                GetPriority(pCurrent->Right->Value) < GetPriority(pCurrent->Value))
            {
                break;
            }

        }
    
        pCurrent = pCurrent->Right;
    }

    *pHead = pCurrent;

    asmContent.Append('\0');
    return asmContent.Leak();
}



char* TranslateToAsm(const char* pFileContent)
{
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

        if (IsOperator(c)) {
            if (!pCurrent->Left) {
                perror("Stacked operators");
                
                return nullptr;
            }

            pCurrent->Right = CreateOperatorNode(c, pCurrent, pHead);
            pCurrent = pCurrent->Right;

            continue;
        }

        if (IsNumber(c)) {
            if (!pCurrent->Left) {
                pCurrent->Left = CreateNumberNode(pCurrent);
            }

            pCurrent->Left->Value = pCurrent->Left->Value * 10 + (c - '0');
            continue;
        }
    }

    pCurrent = pHead;
    ELang::RegistersAllocator allocator(Registers);
    char* pContent = NodesToAsm(&pCurrent, allocator);
    DestroyTreeNodes(pHead);
    
    return pContent;
}

