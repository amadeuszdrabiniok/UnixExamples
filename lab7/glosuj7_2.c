#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MEM_SIZ 4096

struct wspolna_struct
{
    long int glosy[5];
};

int main()
{
    /********************* zmienne *********************/

    int liczbaKomisji = 10;
    int maxLiczbaGlosow = 1000;

    int shmid;
    long int sumaGlosow = 0;
    void *pamiec_wspolna = (void *)0;
    struct wspolna_struct *wspolna;
    char *partie[] = {"FIS", "Rampa", "Kibice", "Czerwoni", "Cookies"};

    /********************* shmget , shmat i zerowanie tablicy *********************/

    shmid = shmget((key_t)1234, MEM_SIZ, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("porazka shmget");
        exit(errno);
    }

    pamiec_wspolna = shmat(shmid, (void *)0, 0);
    if (pamiec_wspolna == (void *)-1)
    {
        perror("porazka shmat");
        exit(errno);
    }

    wspolna = (struct wspolna_struct *)pamiec_wspolna;
    for (int i = 0; i < 5; i++)
    {
        wspolna->glosy[i] = 0;
    }

    /********************* tworzenie komisji *********************/

    for (int i = 0; i < liczbaKomisji; i++)
    {
        if (fork() == 0)
        {
            srand(getpid());
            for (int j = 0; j < maxLiczbaGlosow; ++j)
            {
                wspolna->glosy[rand() % 5]++;
            }
            //printf("umieram %d\n",getpid());
            exit(0);
        }
    }

    /********************* wait i podsumowanie *********************/
    for (int i = 0; i < liczbaKomisji; i++)
    {
        wait(0);
    }
    
    //while (wait(0) > 0);

    for (int i = 0; i < 5; i++)
    {
        sumaGlosow += wspolna->glosy[i];
    }
    printf("Zadana liczba glosow %d\n", maxLiczbaGlosow * liczbaKomisji);
    for (int i = 0; i < 5; i++)
    {
        printf("Partia %s, liczba glosow %ld czyli %0.1f%%\n", partie[i], wspolna->glosy[i], (float)wspolna->glosy[i] / sumaGlosow * 100);
    }
    printf("Suma glosow: %ld\n", sumaGlosow);
}