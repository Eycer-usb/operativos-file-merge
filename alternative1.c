#include <stdlib.h>
#include <stdio.h>
#include<sys/wait.h>

// Para Windows se importa otro header
#if (defined(WINDOWS) || defined(_WIN32))
#include <windows.h>
#else

// Para Basados en Unix
#include <unistd.h>
#endif



int alter1()
{
    FILE* names = fopen("nombres.txt", "r");
    FILE* lastnames = fopen("apellidos.txt", "r");
    int keep_reading = 1;
    fclose(fopen("out.txt", "w"));

    while( keep_reading )
    {
        FILE* names_tmp = tmpfile();
        FILE* lastnames_tmp = tmpfile();
        int pid1 = fork();
        int pid2 = fork();
        
        // Parent Process
        if ( pid1 != 0 && pid2 != 0)
        {
            FILE* outfile = fopen("out.txt", "a");
            int status_names, status_lastnames;
            wait(&status_names);
            wait(&status_lastnames);
            keep_reading = status_names == 0 && status_lastnames == 0;
            if(keep_reading)
            {
                char name[50];
                char lastname[50];
                rewind(names_tmp);
                rewind(lastnames_tmp);
                fscanf(names_tmp, "%[^\n]\n", &name);
                fscanf(lastnames_tmp, "%[^\n]\n", &lastname);
                fprintf(outfile, "%s %s\n", name, lastname);
                fclose(names_tmp);
                fclose(lastnames_tmp);
                fclose(outfile);
            }
            
        }
        // First Child subprocess
        // to read names
        else if ( pid1 == 0 && pid2 != 0)
        {
            // Wait for it's useless child
            wait(NULL);
            char name[50];
            if( fscanf(names, "%[^\n]\n", &name) > 0 )
            {
                fprintf(names_tmp, "%s\n", name);
                exit(0);
            }
            else
            {
                exit(1);
            }
        }
        // Second child subprocess
        // to read lastnames
        else if ( pid1!=0 && pid2 == 0 )
        {
            char lastname[50];
            if ( fscanf(lastnames, "%[^\n]\n", &lastname) > 0)
            {
                fprintf(lastnames_tmp, "%s\n", lastname);
                exit(0);
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            exit(0);
        }
    }
    
    fclose(names);
    fclose(lastnames);
    return 1;
}
