#include "../include/compiler.h"
#include "../include/program.h"

#include <stdio.h>
#include <stdlib.h>




long int GetFileSize(FILE* pFile)
{
    long int fileSize;

    if (fseek(pFile, 0L, SEEK_END) == -1) {
        perror("Coulnd't seek the source file.");
        return EXIT_FILE_CANT_SEEK;
    }


    if ((fileSize = ftell(pFile)) == -1L) {
        perror("Coulnd't seek the source file.");
        return EXIT_FILE_CANT_SEEK;
    }


    if ((fseek(pFile, 0L, SEEK_SET)) == -1) {
        perror("Coulnd't seek the source file.");
        return EXIT_FILE_CANT_SEEK;
    }

    return fileSize;
}





int LoadFile(const char *szFilePath, char** pFileContent)
{
    FILE*    pFile;
    long int fileSize;


    pFile = fopen(szFilePath, "r");

    if (!pFile) {
        perror("Coulnd't open the source file.");
        return EXIT_FILE_CANT_OPEN;
    }


    if ((fileSize = GetFileSize(pFile)) == EXIT_FILE_CANT_SEEK) {
        fclose(pFile);

        return fileSize;
    }


    if (((*pFileContent) = (char*) malloc(sizeof(char) * fileSize)) == NULL) {
        fclose(pFile);

        perror("Coulnd't allocate the memory.");
        return EXIT_ALLOC_ERROR;
    }
        
    
    if (fread((*pFileContent), sizeof(char), fileSize, pFile) != (size_t)fileSize) {
        fclose(pFile);

        perror("Coulnd't read the file.");
        return EXIT_FILE_CANT_READ;
    }




    fclose(pFile);

    return EXIT_SUCCESS;
}




