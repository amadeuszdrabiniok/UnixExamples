#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/utsname.h>
#include <fcntl.h>
#include<string.h>

int main ()
{
    char buf[4];
    char polecenie[1];
    while (1) {
        printf("Podaj polecenie do wykonania [u,s,c,e,q]:\n");
        fgets(buf, sizeof(buf), stdin);
        sscanf(buf, "%s", polecenie);

        //Sekcja potomka
        if (fork()==0) {
            printf("Tu potomek pid=%d\n", getpid());
            /* ... wykonanie polecenia potomka ... */
            switch (polecenie[0]) 
            {
                case 'u':
                {
                    struct utsname result;
                    uname(&result);
                    printf("Korzystasz z systemu: %s, na komputerze: %s\n", result.sysname, result.nodename);
                    break;
                } 
                case 's':
                {
                    execlp("sh","sh",NULL);
                    break;
                }
                case 'c':
                {
                    execlp("xclock","xclock","-update","1",NULL);
                    break;
                }
                case 'e':
                {
                    char outputBuffer[128];
                    FILE *output;

                    output = popen( "zenity --file-selection", "r" );
                    while (fgets(outputBuffer,sizeof outputBuffer, output)>0)
                    {
                        outputBuffer[strcspn(outputBuffer, "\n")] = 0; 
                    }
                    pclose(output);
                    execlp("xedit","xedit",outputBuffer,NULL);
                    break;
                }
                case 'q':
                {
                    exit(0);
                }
                default:
                {
                    printf("Podany parametr nie spełnia wymagan\n");
                    exit(0);
                }
            }
            exit(0); /* obowiazkowe zakonczenie potomka */
        }

        printf("\nTu rodzic po utworzeniu potomka.\n");
        //Sekcja rodzica
        switch (polecenie[0]) 
        {
            case 'u':
            {   
                wait(0);
                break;
            } 
            case 's':
            {
                wait(0);
                break;
            }
            case 'c':
            {
                break;
            }
            case 'e':
            {
                break;
            }
            case 'q':
            {
                exit(0);
            }
            default:
            {
                wait(0);
                printf("Podany parametr nie spełnia wymagan\n");
            }
        }
        
        /* ... czekanie na potomka terminalowego ... */
        /* ... lub sprzatanie zombie okienkowego ... */
        while (waitpid((pid_t)-1,(int *)0,WNOHANG)>0);
    }   /* nieskonczona petla rodzica */
}
