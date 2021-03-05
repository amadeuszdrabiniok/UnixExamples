#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    srand(getpid());

    long int liczbaGlosow[5] = {0,0,0,0,0};
    long int sumaGlosow = 0;
    int maxLiczbaGlosow = 1000;
    char *partie[] = {"FIS", "Rampa", "Kibice", "Czerwoni", "Cookies"};

    for (int i = 0; i < maxLiczbaGlosow; i++)
    {
        liczbaGlosow[rand()%5]++;
    }

    printf("Zadana liczba glosow %d\n",maxLiczbaGlosow);
    for (int i = 0; i < 5; i++)
    {
        sumaGlosow += liczbaGlosow[i];
        printf("Partia %s, liczba glosow %ld czyli %0.1f%%\n",partie[i],liczbaGlosow[i],(float)liczbaGlosow[i]/maxLiczbaGlosow*100);
    }
    printf("Suma glosow: %ld\n",sumaGlosow);
}