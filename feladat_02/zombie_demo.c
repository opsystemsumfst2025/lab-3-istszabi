#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("[Szülő] Indulás... PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // GYEREK folyamat
        printf("[Gyerek] Elindultam, PID: %d\n", getpid());
        printf("[Gyerek] Dolgozom 2 másodpercig...\n");
        sleep(2);
        printf("[Gyerek] Kész vagyok, kilépek.\n");
        exit(0);
    } else {
        // SZÜLŐ folyamat
        printf("[Szülő] A gyerekem PID-je: %d\n", pid);
        
        // Itt hívjuk meg a wait-et:
        printf("[Szülő] Megvárom a gyereket (wait)... \n");
        wait(NULL); 
        printf("[Szülő] A gyerek befejeződött, begyűjtöttem a státuszát. Nincs zombi!\n");
        
        printf("[Szülő] Alszok még 10 másodpercig a teszt kedvéért...\n");
        sleep(10);
        
        printf("[Szülő] Felébredtem, vége.\n");
    }

    return 0;
}