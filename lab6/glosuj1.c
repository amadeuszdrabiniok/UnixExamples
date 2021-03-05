#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>

int main()
{
    srand(getpid());

    long int liczbaGlosow[5] = {0,0,0,0,0};
    int maxLiczbaGlosow = 1000;
    char *partie[] = {"FIS", "Rampa", "Kibice", "Czerwoni", "Cookies"};

    for (int i = 0; i < maxLiczbaGlosow; i++)
    {
        liczbaGlosow[rand()%5]++;

    }
    printf("Zadana liczba glosow %d\n",maxLiczbaGlosow);
    for (int i = 0; i < 5; i++)
    {
        printf("Partia %s, liczba glosow %d czyli %0.1f%%\n",partie[i],liczbaGlosow[i],(float)liczbaGlosow[i]/maxLiczbaGlosow*100);
    }
}