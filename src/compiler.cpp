#include "../include/program.h"
#include "../include/compiler.h"
#include "../include/assembler.h"

#include <cstdio>
#include <cstdlib>




int Compile(const char* szFilePath)
{
    int   returnCode;
    char* pFileContent;
    char* pAsmCode = nullptr;
    
    if ((returnCode = LoadFile(szFilePath, &pFileContent)) != EXIT_SUCCESS) {
        return returnCode;
    }

    if ((pAsmCode = TranslateToAsm(pFileContent)) == nullptr) {
        free(pFileContent);

        perror("Couldn't compile");
        return EXIT_COMPILATION_ERROR;
    }
    
    
    printf("%s", pAsmCode);





    free(pAsmCode);
    free(pFileContent);


    return EXIT_SUCCESS;
}

