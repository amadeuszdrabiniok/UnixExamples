#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>



#define MEM_SIZ 4096

struct wspolna_struct
{
    pthread_mutex_t mutex;
    long int glosy[5];
};

int main()
{
    struct tms t1, t2;
    times(&t1);
    struct timeval start, koniec;
    gettimeofday(&start, 0);
    int tickowNaSekunde;
    tickowNaSekunde = sysconf(_SC_CLK_TCK);

    /********************* zmienne *********************/

    int liczbaKomisji = 10;
    int maxLiczbaGlosow = 1000000;
    int shmid;
    long int sumaGlosow = 0;
    void *pamiec_wspolna = (void *)0;
    pthread_mutexattr_t mattr;
    //pthread_mutex_t *mutex;
    struct wspolna_struct *wspolna;
    char *partie[] = {"FIS", "Rampa", "Kibice", "Czerwoni", "Cookies"};

    /********************* shmget , shmat i zerowanie tablicy *********************/

    shmid = shmget((key_t)249180, MEM_SIZ, 0666 | IPC_CREAT);
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

    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(wspolna->mutex), &mattr);
 
    /********************* tworzenie komisji *********************/

    for (int i = 0; i < liczbaKomisji; i++)
    {
        if (fork() == 0)
        {
            srand(getpid());
            for (int j = 0; j < maxLiczbaGlosow; ++j)
            {
                pthread_mutex_lock(&wspolna->mutex);
                wspolna->glosy[rand() % 5]++;
                pthread_mutex_unlock(&(wspolna->mutex));
            }
            //printf("umieram %d\n", getpid());
            exit(0);
        }
    }

    /********************* wait i podsumowanie *********************/

    for (int i = 0; i < liczbaKomisji; i++)
    {
        wait(0) > 0;
    }

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

    /********************* obliczenie czasu dzialania *********************/

    times(&t2);
    gettimeofday(&koniec, 0);
    printf("\nMinelo %.3f s\n",(koniec.tv_sec - start.tv_sec + ((koniec.tv_usec - start.tv_usec) * 0.000001)));

    printf("Czas uzytkownika:\n");
    printf("Rodzic: %f Potomek:%f\n",((double)t2.tms_utime-t1.tms_utime) / tickowNaSekunde, ((double)t2.tms_cutime-t1.tms_cutime) / tickowNaSekunde);
    printf("Czas systemu:\n");
    printf("Rodzic: %f Potomek:%f\n",(((double)t2.tms_stime-t1.tms_stime) / tickowNaSekunde),(((double)t2.tms_cstime-t1.tms_cstime) / tickowNaSekunde));

    //pthread_mutex_destroy(mutex);
    pthread_mutex_destroy(&wspolna->mutex);
    return 0;
}
