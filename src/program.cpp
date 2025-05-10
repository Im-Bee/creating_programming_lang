#include "../include/compiler.h"
#include "../include/program.h"


#include <cstdio>
#include <cstdlib>



int main(int argc, char* argv[]) 
{
    if (argc < 2) {
        printf("Invalid argument, no file to compile\n"
               "%s <FILE>\n"
               "\n",
               argv[0]
        );

        return EXIT_INVALID_ARGS_PASSED;
    }
    


    int status;
    
    for (int i = 1; i < argc; ++i) 
    {
        if ((status = Compile(argv[i])) != EXIT_SUCCESS) {
            return status;
        }
    }



    return EXIT_SUCCESS;
}
