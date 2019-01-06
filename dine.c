/****************************************************************************************
 *** Author: Anugesh Balasubramaniam
 *** Date: 03/27/2017
 *** Description: The dining philosopher program is an attempt to simulate the dining philosopher problem.
 The program takes 2 arguments excluding the executable file. The first argument is the
 total number of philosophers to sit at the table and the second argument is the number
 of times each philosopher should eat to feel satisfied.
 ***********************************************************************************************/

//function library headers
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//global variables to be referenced throughout the program
pthread_t philo_thread[50];
pthread_mutex_t forks[50];
int philoState[50] = {0};
int mealsToeat;
int numPhilo;

//function prototypes
void *philo_func(void *i);
int pickupFork(int leftFork, int rightFork, int threadID);

//INPUT: Arguments from the command line
//OUTPUT: Necessary output for each command line argument given by the user
//FUNCTION: Takes arguments from the command line arguments and displays necessary output to the screen
int main(int argc, char **argv)
{
    int *arg, i, j;
    
    if (argc != 3)
    {
        printf("Invalid number of arguments. Expecting 3\n");
    }
    else
    {
        numPhilo = atoi(argv[1]);
        
        if (numPhilo <= 2)
        {
            printf("Invalid number of philosophers. Expecting 3 or more\n");
            return 0;
        }
        mealsToeat = atoi(argv[2]);
        
        for (i = 0; i < numPhilo; i++)
        {
            arg = (int*)malloc(sizeof(int));
            *arg = i;
            pthread_create(&philo_thread[i], NULL, philo_func, (void*)arg);
        }
        
        for (j = 0; j < numPhilo; j++)
        {
            pthread_join(philo_thread[j], NULL);
        }
    }
    
    return 0;
}


//thread function where each philosopher thread sits down at the table and thinks.
//each philosopher eats while they are not satisfied
//outputs the appropriate states of the philosopher, if philosopher is thinking or eating
void *philo_func(void *i)
{
    void *ptr = i;
    int threadID = *((int*)i);
    int mealsEaten = 0;
    int leftFork = threadID;
    int rightFork = threadID + 1;
    
    printf("Philosopher %d is thinking\n", threadID+1);
    
    if (threadID == (numPhilo-1))
    {
        rightFork = 0;
    }
    
    while(philoState[threadID] != 2)
    {
        mealsEaten += pickupFork(leftFork, rightFork, threadID);
        if (mealsEaten == mealsToeat)
        {
            philoState[threadID] = 2;
        }
    }
    
    free(ptr);
    pthread_exit(NULL);
    
}

//this function executes when the philosopher is ready to pick up the forks and eat
//outputs the appropriate states of the philosopher, if philosopher is thinking or eating
int pickupFork(int leftFork, int rightFork, int threadID)
{
    if ((philoState[leftFork] != 1) && (philoState[rightFork] != 1))
    {
        philoState[threadID] = 1;
        
        printf("Philosopher %d is eating\n", threadID+1);
        
        usleep(1000);
        
        pthread_mutex_lock(&forks[leftFork]);
        pthread_mutex_lock(&forks[rightFork]);
        
        pthread_mutex_unlock(&forks[leftFork]);
        pthread_mutex_unlock(&forks[rightFork]);
        
        philoState[threadID] = 0;
        
        printf("Philosopher %d is thinking\n", threadID+1);
        
        return 1;
    }
    
    return 0;
}

