#ifndef COMPILER_H
#define COMPILER_H





#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus


int LoadFile(const char *szFilePath, char** pFileContent);


#ifdef __cplusplus
}
#endif // !__cplusplus
       
       
 


int Compile(const char* szFilePath);



#endif // !COMPILER_H
