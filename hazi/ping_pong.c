#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define ROUNDS 10 

typedef struct {
    sem_t sem_parent;
    sem_t sem_child;
} SharedSems;

int main() {

    SharedSems *sems = mmap(NULL, sizeof(SharedSems),
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS,
                            -1, 0);
    
    if (sems == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }
    
    sem_init(&sems->sem_parent, 1, 1); 
    sem_init(&sems->sem_child, 1, 0);
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    
    if (pid == 0) {
        // gyerek
        for (int i = 0; i < ROUNDS; i++) {
            sem_wait(&sems->sem_child); 
            
            printf("PONG #%d\n", i + 1);
            
            sem_post(&sems->sem_parent); 
        }
        exit(0);
    } else {
        // szulo
        for (int i = 0; i < ROUNDS; i++) {
            sem_wait(&sems->sem_parent); 
            
            printf("PING #%d\n", i + 1);
            
            sem_post(&sems->sem_child); 
        }
        
        wait(NULL);
        
        sem_destroy(&sems->sem_parent);
        sem_destroy(&sems->sem_child);
        munmap(sems, sizeof(SharedSems));
    }
    
    return 0;
}
