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
    /********* Pomiar czasu *********/
    struct timeval start, koniec;
    gettimeofday(&start, 0);
    struct tms t;
    int tickowNaSekunde = sysconf(_SC_CLK_TCK);

    /********* Deklaracja zmiennych i ustalenie parametrow programu *********/
    struct sockaddr_un addr_str;
    struct Informacje informacje, odebrane;
    int zamknietePolaczenia = 0, n = 0, maxLiczbaGlosow = 10, liczbaKomisji = 10, sumaKontrolna = 0, liczbaGlosow[5] = {0, 0, 0, 0, 0}, liczbaGlosowZKomisji[liczbaKomisji], x = 0, y = 0, ser_sock, cli_sock, addr_len, result;
    char *partie[] = {"FIS", "Rampa", "Kibice", "Czerwoni", "Cookies"};
    char buf[BUFSIZ];
    fd_set readfds, testfds;

    /********* Zerowanie tablicy glosow z poszczegolnych komisji *********/
    for (int i = 0; i < liczbaKomisji; i++)
    {
        liczbaGlosowZKomisji[i] = 0;
    }

    /********* Przygotowanie gniazdka i struktury adresowej *********/
    ser_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    addr_str.sun_family = AF_UNIX;
    strcpy(addr_str.sun_path, GNIAZDKO_SERWERA);

    /********* Utworzenie gniazdka i bindowanie *********/
    unlink(GNIAZDKO_SERWERA);
    addr_len = sizeof(addr_str);
    if (bind(ser_sock, (struct sockaddr *)&addr_str, addr_len) == -1)
    {
        perror("Blad bindowania: ");
        exit(-1);
    }
    if (listen(ser_sock, 5) == -1)
    {
        perror("Blad listen: ");
        exit(-1);
    }

    /********* Tworzenie potomkow *********/
    for (int i = 0; i < liczbaKomisji; i++)
    {
        if (fork() == 0)
        {
            struct timespec timeout = {0, 1000000000};
            int sock;
            sock = socket(AF_UNIX, SOCK_STREAM, 0);
            struct sockaddr_un addr_str_cli;
            addr_str_cli.sun_family = AF_UNIX;
            strcpy(addr_str_cli.sun_path, GNIAZDKO_SERWERA);
            srand(getpid());
            sleep(2);

            /** Polaczenie z gniazdkiem serwera **/
            if (connect(sock, (const struct sockaddr *) &addr_str_cli, sizeof(addr_str_cli)) == -1)
            {
                perror("blad connect");
                return -1;
            }

            /** Losowanie glosow i ich wysylanie **/
            for (int j = 0; j < maxLiczbaGlosow; ++j)
            {
                x = rand() % 5;
                informacje.numerKomisjiS = i;
                informacje.numerPartiiS = x;
                memcpy((void*)buf, (void*)&informacje, sizeof(struct Informacje));
                write(sock, (void *)buf, sizeof(struct Informacje));
                nanosleep((struct timespec *) &timeout,(struct timespec *) 0);
            }
            exit(0);
        }
    }

    /********* Obsluga klientow i liczenie glosow *********/
    while (sumaKontrolna < maxLiczbaGlosow*liczbaKomisji)
    {
        printf("Serwer: czeka na nowe polaczenie\n");
        cli_sock = accept(ser_sock,0,0);
        if (cli_sock==-1) 
        {
            perror("Blad accept: "); 
            exit(-1);
        }
 
        while ((n=read(cli_sock,&buf,sizeof(struct Informacje)))>0)
        {
            memcpy((void *)&odebrane, (void *)buf, sizeof(struct Informacje));
            liczbaGlosow[odebrane.numerPartiiS]++;
            liczbaGlosowZKomisji[odebrane.numerKomisjiS]++;
            sumaKontrolna++;
            printf("Serwer: odebral numer partii %d oraz nr komisji %ld\n",odebrane.numerPartiiS, odebrane.numerKomisjiS);
        }
        if (n==0) 
        {
            printf("Serwer: klient zamknal polaczenie.\n");
        }
        else 
        {
            perror("Blad read: ");
            printf("Serwer: blad odczytu, rozlacza klienta\n");
        }
        close(cli_sock);

        /** Usuwanie zombie **/
        while (waitpid((pid_t)-1, (int *)0, WNOHANG) > 0);
    }
    close(ser_sock);

    /********* Podsumowanie wynikow glosowania *********/
    // for (int i = 0; i < liczbaKomisji; i++)
    // {
    //     printf("Liczba glosow otrzymanych z komisji nr: %d jest rowna %d\n", i, liczbaGlosowZKomisji[i] );
    // }

    printf("\n");

    for (int i = 0; i < 5; i++)
    {
        printf("Partia %s otrzymala %d glosow, czyli %0.1f%%\n", partie[i], liczbaGlosow[i], (float)liczbaGlosow[i] / (maxLiczbaGlosow * liczbaKomisji) * 100);
    }

    /********* Podsumowanie czasu dzialania *********/
    gettimeofday(&koniec, 0);
    times(&t);

    printf("\nMinelo %.3f s\n", (koniec.tv_sec - start.tv_sec + ((koniec.tv_usec - start.tv_usec) * 0.000001)));
    return 0;
}
