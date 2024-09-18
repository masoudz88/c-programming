/** multiThreadProgram.c
 * this code uses two threads concurrently to get user input and at the same time calculate
 * the sum of numbers input by user, we need to use mutex locks and semaphores to manage this situation 
 * as we first need to get user input in order to be able to calculate the sum.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
/*initialize global variables*/
int userInput;
int sum;
/*initialize semaphore and mutex lock globally*/
sem_t emptyBuffer;
sem_t fullBuffer;
pthread_mutex_t mutex;
void *inputGetter(void *arg);
void *mySum();
int main(void){
    sem_init(&emptyBuffer, 0, 1);
    sem_init(&fullBuffer, 0, 0);
    pthread_mutex_init(&mutex, NULL);  
    
    pthread_t th[2];    
    while(userInput>=0){/*the condition to exit when a negative number is entered*/
        /*creating concurrent threads*/                
        pthread_create(&th[0], NULL, inputGetter, NULL);                
        pthread_create(&th[1], NULL, mySum, NULL);  
        pthread_join(th[0], NULL);        
        pthread_join(th[1], NULL);        
        printf("current total: %d\n", sum);
     
    }
    sem_destroy(&emptyBuffer);
    sem_destroy(&fullBuffer);
    pthread_mutex_destroy(&mutex);
       
    return 0;
}

void *inputGetter(void *arg){ 
    userInput =(int *)arg;
    sem_wait(&emptyBuffer);/*acquiring the lock*/
    pthread_mutex_lock(&mutex);
    /*start critical section*/   
    printf("Input a number: ");
    scanf("%d",&userInput);
    /*end critical section*/ 
    if( userInput<0){
        printf("The sum of the input numbers is  %d\n", sum);
        exit(0);
    }             
    pthread_mutex_unlock(&mutex);
    sem_post(&fullBuffer);/*releasing the lock for sum to be calculated*/         
}

void *mySum(void *arg){    
   sem_wait(&fullBuffer); /*acquiring the lock*/  
   sum+=userInput; 
   sem_post(&emptyBuffer);/*releasing the lock for userInput to be scanned*/   
}                
