
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/types.h>

#define GNIAZDKO_SERWERA "/tmp/gniazdko_serwera"

struct Informacje
    {   
        int numerPartiiS;
        long int numerKomisjiS;
    };

int main()
{
    struct tms t;
    struct timeval start, koniec;
    gettimeofday(&start, 0);
    struct Informacje informacje;
    struct Informacje odebrane;

    int tickowNaSekunde;
    tickowNaSekunde = sysconf(_SC_CLK_TCK);


    int liczbaGlosow[5] = {0, 0, 0, 0, 0}, x = 0, y = 0;
    int sock, n = 0, maxLiczbaGlosow = 100000, liczbaKomisji = 100, sumaKontrolna = 0;
    int liczbaGlosowZKomisji[liczbaKomisji];

    for (int i = 0; i < liczbaKomisji; i++)
    {
        liczbaGlosowZKomisji[i]=0;
    }
    
    char *partie[] = {"FIS", "Rampa", "Kibice", "Czerwoni", "Cookies"};
    socklen_t serv_len;
    struct sockaddr_un serv_addrstr;
    struct timespec timeout = {0, 100000000};
    char buf[BUFSIZ];

    sock = socket(PF_UNIX, SOCK_DGRAM, 0);
    serv_addrstr.sun_family = AF_UNIX;
    strcpy(serv_addrstr.sun_path, GNIAZDKO_SERWERA);

    unlink(GNIAZDKO_SERWERA);
    serv_len = sizeof(serv_addrstr);
    if (-1 == bind(sock, (struct sockaddr *)&serv_addrstr, serv_len))
    {
        perror("blad bind");
        exit(-1);
    }

    for (int i = 0; i < liczbaKomisji; i++)
    {
        if (fork() == 0)
        {
            //clock_t startPotomka = clock();
            srand(getpid());
            sleep(1);
            for (int j = 0; j < maxLiczbaGlosow; ++j)
            {
                x = rand() % 5;
                informacje.numerKomisjiS = i;
                informacje.numerPartiiS = x;
                memcpy((void *)buf, (void *)&informacje, sizeof(struct Informacje));
                sendto(sock, buf, sizeof(struct Informacje), 0, (struct sockaddr *)&serv_addrstr, serv_len);
            }
            memcpy((void *)buf, (void *)&informacje, sizeof(struct Informacje));
            sendto(sock, buf, sizeof(struct Informacje), 0, (struct sockaddr *)&serv_addrstr, serv_len);
            //clock_t koniecPotomka = clock();
            //printf("Czas działania komisji %d to: %f\n",i,(float)(koniecPotomka - startPotomka) / CLOCKS_PER_SEC);

            exit(0);
        }
    }

    printf("Serwer: rozpoczynam obsluge klientow\n");

    while (sumaKontrolna < maxLiczbaGlosow * liczbaKomisji)
    {
        n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addrstr, &serv_len);
        if (n == -1)
        {
            perror("blad read");
            printf("Serwer: blad odczytu, kontynuuje.\n");
        }
        else if (n != sizeof(struct Informacje))
        {
            printf("Serwer: odebrane %d bajtow, to nie int???\n", n);
        }
        else
        {
            memcpy((void *)&odebrane, (void *)buf, sizeof(struct Informacje));
            liczbaGlosow[odebrane.numerPartiiS]++;
            liczbaGlosowZKomisji[odebrane.numerKomisjiS]++;
            sumaKontrolna++;
        }
        while (waitpid((pid_t)-1, (int *)0, WNOHANG) > 0);
        //printf("suma %d",sumaKontrolna);
    }
    close(sock);

    for (int i = 0; i < liczbaKomisji; i++)
    {
        printf("Liczba glosow otrzymanych z komisji nr: %d jest rowna %d\n",i,liczbaGlosowZKomisji[i]-1);
    }
    
    printf("\n");

    for (int i = 0; i < 5; i++)
    {
        printf("Partia %s otrzymala %d glosow, czyli %0.1f%%\n", partie[i], liczbaGlosow[i], (float)liczbaGlosow[i] / (maxLiczbaGlosow * liczbaKomisji) * 100);
    }

    gettimeofday(&koniec, 0);
    printf("\nMinelo %.3f s\n",(koniec.tv_sec - start.tv_sec + ((koniec.tv_usec - start.tv_usec) * 0.000001)));

    times(&t);
    printf("Czas uzytkownika:\n");
    printf("Rodzic: %f Potomek:%f\n",((double)t.tms_utime) / tickowNaSekunde, ((double)t.tms_cutime) / tickowNaSekunde);
    printf("Czas systemu:\n");
    printf("Rodzic: %f Potomek:%f\n",(((double)t.tms_stime) / tickowNaSekunde),(((double)t.tms_cstime) / tickowNaSekunde));
    return 0;
}
