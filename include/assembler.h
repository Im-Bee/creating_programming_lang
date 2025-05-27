#ifndef ASSEMBLER_H
#define ASSEMBLER_H



char* TranslateToAsm(const char* pFileContent);


#ifdef __cplusplus

#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstddef>



namespace ELang {
class RegistersAllocator 
{
    public:
        template<size_t Size>
        RegistersAllocator(const char* (&pRegistersStrings)[Size])
            : m_uAmountOfRegisters(Size),
            m_uAlloced(0)
        {   
            m_pRegistersNames = (char**) malloc(sizeof(char*) * m_uAmountOfRegisters);
            if (!m_pRegistersNames) {
                throw; // TODO
            }
            
            for (size_t i = 0; i < m_uAmountOfRegisters; ++i) 
            {
                m_pRegistersNames[i] = (char*) malloc(sizeof(char) * 32);
                if (!m_pRegistersNames[i]) {
                    throw; // TODO
                }
    

                strcpy(m_pRegistersNames[i], pRegistersStrings[i]);
            }
        }


        ~RegistersAllocator();

    public:
        char* AllocRegister();

        void FreeRegister();

        char* GetCurrentRegister();

        char* GetNthFromBack(const size_t& uN);

    private:
        char* PutOnStack();

    private:
        uint8_t m_uAmountOfRegisters;
        char** m_pRegistersNames;

        uint8_t m_uAlloced;

};
}   // !ELang

#endif // !__cplusplus
#endif // !ASSEMBLER_H
