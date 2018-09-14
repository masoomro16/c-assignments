// Project #5 
// Dining Philosophers Problem
// Maira Soomro 

#include <pthread.h> 
#include <stdio.h> 
#include <semaphore.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <unistd.h>

typedef struct philosopher { 
	int phil_id; // 0 1 2 3 4
	sem_t* leftFork; 
	sem_t* rightFork; 
	sem_t* roomPtr; 
} philosopher; 

/*
 * Deadlock free situation
 * Limits number of philosophers to 4
 * Parameters: struct philosopher* ph
 */
void fourPhilosophers(philosopher* ph);

/*
 * Deadlock situation
 * Parameters: struct philosopher* ph
 */ 
void fivePhilosophers(philosopher* ph); 

int main (int argc, char* argv[]) { 

	sem_t* fork[5]; 
	sem_t* room;
	pthread_t threadIds[5];
	philosopher* phils[5]; 
	int i=0; 
	int max = 0; // limit times a philosopher can eat 
 
	// initialize room to 4 
	// so that at most only 4 can enter at one time
	room = (sem_t*)malloc(sizeof(sem_t)); 
	sem_init(room, 0, 4); 	

	// initialize fork to 1
	for(i = 0; i<5; i++) { 
		fork[i] = (sem_t*)malloc(sizeof(sem_t)); 
		sem_init(fork[i], 0, 1);
	}



	for(i=0; i<5; i++) { 
		philosopher* ph = (philosopher*)malloc(sizeof(philosopher));
		ph->phil_id = i; // assign id to phil 
		ph->leftFork = fork[i]; 

		// if last one, set right fork to 0
		if (i==4) { 
			ph->rightFork = fork[0]; 
		}
		else { 
			ph->rightFork = fork[i+1]; 
		}
		ph->leftFork = fork[i]; 
		ph->roomPtr = room; 

		//finally...
		phils[i] = ph; 
	}


	// 0 is deadlock free FOUR PHIL
	// 1 is deadlock FIVE PHIL 
	if(strcmp(argv[1], "0")==0) { 

		// philosophers can only eat up to 5 times 
		while (max!= 5) { 
			// create threads 
			for(i=0; i<5; i++) {
				pthread_create(&threadIds[i], NULL, (void *)fourPhilosophers, (void *)phils[i]);
			}

			// join 
			for(i=0; i<5; i++) { 
				pthread_join(threadIds[i], NULL); 
			}
			max++; 
		}
	}

	else if (strcmp(argv[1], "1")==0) { 

		for(i=0; i<5; i++) {
			pthread_create(&threadIds[i], NULL, (void *)fivePhilosophers, (void *)phils[i]);
		}

		for(i=0; i<5; i++) { 
			pthread_join(threadIds[i], NULL); 
		}		

	} 

	else { 
		printf("Invalid argument\n"); 
		exit(1); 
	}

	return 0; 

}


/*
 * Deadlock free situation for a  
 * single philospher 
 */ 
void fourPhilosophers (philosopher* ph) {

	int j = ph->phil_id; 

	printf("Philosopher %d is done thinking and ready to eat\n", j); 

	sem_wait(ph->roomPtr); 

	if( sem_wait(ph->leftFork) == 0 ) // left fork  
		printf("Philosopher %d: taking left fork %d\n", j, j); 

	// sleep to delay
	sleep(1);

	if (sem_wait(ph->rightFork) == 0) //right fork  
	{
		if(j==4)
			printf("Philosopher %d: taking right fork %d\n", j, 0); 
		else 
			printf("Philosopher %d: taking right fork %d\n", j, j+1);
	}
  
	printf("PHILOSOPHER %d: EATING\n", ph->phil_id); 

	if (sem_post(ph->leftFork) == 0){ // drop left fork 
		printf("Philosopher %d: putting down left fork %d\n", j, j); 
	}

	if (sem_post(ph->rightFork) == 0){  
		if(j==4)
			printf("Philosopher %d: putting down right fork %d\n", j, 0); 
		else 
			printf("Philosopher %d: putting down right fork %d\n", j, j+1);
	} 

	sem_post(ph->roomPtr); 

	pthread_exit(NULL);
}

/*
 * Potential deadlock situation
 *
 */

void fivePhilosophers (philosopher* ph) {

	int j = ph->phil_id; 

	while(1) { 

		printf("Philosopher %d is done thinking and ready to eat\n", j); 


		if( sem_wait(ph->leftFork) == 0 ) // left fork  
			printf("Philosopher %d: taking left fork %d\n", j, j); 

		// sleep to delay
		sleep(1);

		if (sem_wait(ph->rightFork) == 0) //right fork  
		{
			if(j==4)
				printf("Philosopher %d: taking right fork %d\n", j, 0); 
			else 
				printf("Philosopher %d: taking right fork %d\n", j, j+1);
		}
		//phil->isEating = true;  
		printf("PHILOSOPHER %d: EATING\n", ph->phil_id); 

		if (sem_post(ph->leftFork) == 0){ // drop left fork 
			printf("Philosopher %d: putting down left fork %d\n", j, j); 
		}

		if (sem_post(ph->rightFork) == 0){  
			if(j==4)
				printf("Philosopher %d: putting down right fork %d\n", j, 0); 
			else 
				printf("Philosopher %d: putting down right fork %d\n", j, j+1);
		} 		


	}
		pthread_exit(NULL);

}
 
