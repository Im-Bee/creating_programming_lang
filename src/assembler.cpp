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



static inline int GetPriority(TreeNode* pNode)
{
    if (!pNode) {
        return -1;
    }

    if (pNode->Value == OperatorMul || pNode->Value == OperatorDiv) {
        return 2;
    } else {
        return 0;
    }
}

// ---------------------------------------------------------------------------------------------------------------------






// ELang::RegistersAllocator -------------------------------------------------------------------------------------------

ELang::RegistersAllocator::~RegistersAllocator() 
{
    if (m_pRegistersNames) {
        for (size_t i = 0; i < m_uAmountOfRegisters; ++i) 
        {
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
        return PutOnStack();
    }

    char* pAsm = new char[TranslatedStringsMaxLenght];
    pAsm[0] = 0;

    ++m_uAlloced;

    return pAsm;
}



void ELang::RegistersAllocator::FreeRegister()
{
    if (!m_uAlloced--) {
        throw; // TODO
    }
}


char* ELang::RegistersAllocator::GetCurrentRegister()
{
    if (!m_uAlloced) {
        throw; // TODO
    }

    return m_pRegistersNames[m_uAlloced % m_uAmountOfRegisters - 1];
}

char* ELang::RegistersAllocator::GetNthFromBack(const size_t& uN)
{
    if (uN > m_uAlloced) {
        return m_pRegistersNames[m_uAlloced];
    }

    return m_pRegistersNames[m_uAlloced % m_uAmountOfRegisters - uN];
}


char* ELang::RegistersAllocator::PutOnStack()
{
    char* pAsm = new char[TranslatedStringsMaxLenght];
    pAsm[0] = 0;

    



    return pAsm;
}


// ---------------------------------------------------------------------------------------------------------------------





// Nodes creation ------------------------------------------------------------------------------------------------------

static TreeNode* CreateOperatorNode(const char&     c, 
                                          TreeNode* pParent) 
{
    pParent->Operator = EOperator;

    switch (c) {
        case '+':
            pParent->Value = OperatorAdd;
            break;

        case '-':
            pParent->Value = OperatorSub;
            break;

        case '*':
            pParent->Value = OperatorMul;
            break;

        case '/':
            pParent->Value = OperatorDiv;
            break;

        default:
            throw; // TODO
    
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

static char* TranslateNumberAlloc(TreeNode* pNode, ELang::RegistersAllocator& allocator) 
{
    char* pAsm = new char[TranslatedStringsMaxLenght];
    
    char* pRegisterAllocation = allocator.AllocRegister();
    sprintf(pAsm, "%smov %s, %d\n", pRegisterAllocation, allocator.GetCurrentRegister(), pNode->Value);

    delete [] pRegisterAllocation;

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
    if (EqTreeType(pCurrent->Left, ENumber)) 
    {
        delete [] asmContent.AddStringSlice(TranslateNumberAlloc(pCurrent->Left, allocator),
                                            TranslatedStringsMaxLenght);
    }





    if (GetPriority(GetRight(pCurrent)) > GetPriority(pCurrent))
    {
        delete [] asmContent.AddStringSlice(NodesToAsm(&pCurrent->Right, allocator),
                                            2048);
    }





    if (EqTreeType(GetLeft(GetRight(pCurrent)), ENumber)) 
    {
        delete [] asmContent.AddStringSlice(TranslateNumberAlloc(pCurrent->Right->Left, allocator),
                                            TranslatedStringsMaxLenght);
            
        DestroyLeaf(&pCurrent->Right->Left);
    }




    switch (pCurrent->Value) {
        case OperatorAdd:
            delete [] asmContent.AddStringSlice(TranslateAdd(allocator),
                                                TranslatedStringsMaxLenght);
            break;
        case OperatorSub:
            delete [] asmContent.AddStringSlice(TranslateSub(allocator),
                                                TranslatedStringsMaxLenght);
            break;
        case OperatorMul:
            delete [] asmContent.AddStringSlice(TranslateMul(allocator),
                                                TranslatedStringsMaxLenght);
            break;
        case OperatorDiv:
            delete [] asmContent.AddStringSlice(TranslateDiv(allocator),
                                                TranslatedStringsMaxLenght);
            break;

        default:
            throw; // TODO:
    }


    pCurrent->Operator = ENone;
}






static char* NodesToAsm(TreeNode** pHead, ELang::RegistersAllocator& allocator)
{
    ELang::DynamicAlloc<char, 256> asmContent = {};

    TreeNode* pCurrent = *pHead;

    while (pCurrent) 
    {
        if (pCurrent->Operator == EOperator) 
        {
            HandleOperator(pCurrent, allocator, asmContent);

            if (GetPriority(GetRight(pCurrent)) < GetPriority(pCurrent)) {
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

            pCurrent->Right = CreateOperatorNode(c, pCurrent);
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

