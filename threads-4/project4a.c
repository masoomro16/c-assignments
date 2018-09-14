// Project #4 
// Threads 
// Maira Soomro 

#include <pthread.h> 
#include <stdio.h> 
#include <semaphore.h> 
#include <stdbool.h> 
#include <stdlib.h> 

typedef struct queue{ 
	int val; 
	struct queue* next; 
} queue; 

typedef struct channel{ 
	sem_t* semRace; 
	sem_t* semSync;
	struct queue *front, *rear; 
} channel; 

// usage: 
// channel[i].qu[i]->nd = NULL 

typedef struct node{ 
	int uid; 
	int twoHop; 
	int oneHop; 
	int temp; 
	bool active; // active or relay 
	bool leader; // leader or not 
	channel *left; // link channel left
	channel *right; // link channel right
} node; 

typedef struct threadStruct {

	node *currentNode; 
	channel *chanRead, *chanWrite; 

} threadStruct; 

int read(channel* ch);
void write (channel* ch, int value); 
void findLeader(threadStruct* thread); 
void display(channel* ch);

int main() { 

	int num=0; // stores array size
	int currNode=0; // stores the number of the current node for input
	int count = 0;  
	scanf("%d", &num); 	
	//printf("%d", num); 
	channel* channels[num];//(channel*)malloc(sizeof(channel));
	node* nodes[num]; //= (node*)malloc(sizeof(node));

	int leaderNumber;
	//theadStruct leaderNum; 
	//while (scanf("%d", &currNode) != EOF) {
	int j; 
	for (j=0; j<num; j++) { 
		scanf("%d", &currNode); 
		node* newNode = (node*)malloc(sizeof(node));
		newNode->uid=currNode; 
		newNode->temp=currNode; 
		newNode->oneHop = 0; 
		newNode->twoHop = 0;
		nodes[j] = newNode; 

	} 

	// set nodes to active 
	// leader to false 
	int i; 
	for (i=0; i<num; i++) { 
		//printf("\nthis %i", i);
		nodes[i]->active = (bool*)malloc(sizeof(bool));
		nodes[i]->leader = (bool*)malloc(sizeof(bool));		
		nodes[i]->active = true;
		nodes[i]->leader = false; 
	}	

	// initialize channel 
	for (j=0; j<num; j++) { 
		channel* chn = (channel*)malloc(sizeof(channel));

		chn->semRace = (sem_t*)malloc(sizeof(sem_t)); 
		chn->semSync = (sem_t*)malloc(sizeof(sem_t));
		sem_init(chn->semRace, 0, 1); 
		sem_init(chn->semSync, 0, 0);

		chn->front = (queue*)malloc(sizeof(queue)); 
		chn->rear = (queue*)malloc(sizeof(queue));
		
		chn->front= NULL; 
		chn->rear=NULL;

		channels[j] = chn; 
	} 

	// link nodes to channels
	for (i=0; i<num; i++) { 


		if (i != num-1) { 
			nodes[i]->right = channels[i+1];
			nodes[i]->left = channels[i]; 
		}

		else { 
			nodes[i]->right = channels[0];
			nodes[i]->left = channels[i]; 
		}
	}

	// create threads 

	threadStruct *oneThread[num];

	bool leaderFound = false; 
	int phaseNum = 1; 

		pthread_t threadIds[num];

/*
	for (i=0; i<num; i++ ) { 
		printf("\nNode: %d ", nodes[i]->uid);
	}
*/
	for(i=0;i<num;i++){
		oneThread[i] = (threadStruct*)malloc(sizeof(threadStruct));
		if (i != num-1){
			oneThread[i]->currentNode = nodes[i]; // type node 
			oneThread[i]->chanRead = nodes[i]->left;  // type channel struct
			oneThread[i]->chanWrite = nodes[i]->right; // type channel 
		}

		else { 
			oneThread[i]->currentNode = nodes[i]; // type node 
			oneThread[i]->chanRead = nodes[i]->left;  // type channel struct
			oneThread[i]->chanWrite = nodes[i]->right; // type channel 
			}
	}

	while (leaderFound == false) { 


//		printf("\nWHILEELOOP");
		for(i=0; i<num; i++) { 

//			printf("\nnode %d", nodes[i]->uid);
//			printf("\nthread %d", oneThread[i]->currentNode->uid);

			//printf("Hiiii"); 
			//printf ("temp uid: %d", oneThread->currentNode->temp); 
			pthread_create(&threadIds[i], NULL, (void *)findLeader, (void *)oneThread[i]); 

//			printf("\nnum %d ", num);
		}

		for(i=0; i<num; i++) { 

//			printf("\npthreadddd");
			pthread_join(threadIds[i], NULL); 
			//printf ("temp uid: %d", oneThread->currentNode->temp); 

			if (i !=num-1) {
				nodes[i] = oneThread[i]->currentNode;
				nodes[i]->left = oneThread[i]->chanRead;
				nodes[i]->right = oneThread[i]->chanWrite; 
			}

			else { 
				nodes[i] = oneThread[i]->currentNode;
				nodes[i]->left = oneThread[i]->chanRead;
				nodes[i]->right = oneThread[i]->chanWrite; 
			}		 

			if(oneThread[i]->currentNode->leader == true) {
				leaderNumber = oneThread[i]->currentNode->uid;
				leaderFound = true;
				
			}

			if (oneThread[i]->currentNode->active == true) { 
				printf("\n[%d] ", phaseNum); 
				printf("[%d] ", oneThread[i]->currentNode->uid); 
				printf("[%d] ", oneThread[i]->currentNode->temp); 

//				printf("\n leaderFound %d", leaderFound);
			}
		
		}
		phaseNum++;

}		

	printf("\nleader: %d\n", leaderNumber); 
	//free(channels); 
	//free(nodes);

	return 0; 
}


int read(channel* ch) { 	
	int v; // new node 
	sem_wait(ch->semSync); 
	sem_wait(ch->semRace); 
	//v = value removed from queue;

    	struct queue *temp = ch->front;
    	ch->front = ch->front -> next;
    	v = temp->val; 
    	//free(temp);

	sem_post(ch->semRace); 
	return v; 
}

void write (channel* ch, int value) { 	

//	printf("\nVal %d", value);
	sem_wait(ch->semRace); 

	// v = put value on the queue of the channel 
	queue *newNd; 
   	newNd = (queue*)malloc(sizeof(queue));
   	newNd->val = value;

 //  	printf("\nVal2 %d", newNd->val);
   	newNd->next = NULL;
   if(ch->front == NULL)
      ch->front = ch->rear = newNd;
   else{
      ch->rear -> next = newNd;
      ch->rear = newNd;
   }

//   	printf("\nValFront %d", ch->front->val);


	sem_post(ch->semRace); 
	sem_post(ch->semSync); 
 
 //return; 
}

void findLeader(threadStruct* thread) { 

//	printf("\n node UID: %d", thread->currentNode->uid);
	if(thread->currentNode->active==true) { 


		// write tempuid
		write(thread->chanWrite, thread->currentNode->temp); 
					
		// read one hop temp uid 
		thread->currentNode->oneHop = read(thread->chanRead);

//		printf("Maijiaa");
		// write one hop temp uid
		write(thread->chanWrite, thread->currentNode->oneHop);

//		printf("Thjwjeoiqw");
		// read two hop temp uid 
		thread->currentNode->twoHop = read(thread->chanRead);

//		printf("insanity");		

		if(thread->currentNode->oneHop == thread->currentNode->temp) { 
			thread->currentNode->leader = true; 
		}

		else if ((thread->currentNode->oneHop > thread->currentNode->twoHop) && (thread->currentNode->oneHop > thread->currentNode->temp)) {
//			printf("exits thread");
			thread->currentNode->temp = thread->currentNode->oneHop; 
		}

		else { 
			// this node becomes a relay node 
			thread->currentNode->active = false; 
		}


	}
	
	else { 

		// read temp uid
		thread->currentNode->temp = read(thread->chanRead);

		// write temp uid 
		write(thread->chanWrite, thread->currentNode->temp); 

//printf("ihhhuewewewuty");	
		// read temp uid
		thread->currentNode->temp = read(thread->chanRead);

//printf("#########ihhhuuty");	
		// write temp uid 
		write(thread->chanWrite, thread->currentNode->temp); 		

//		printf("ihhhuuty");	
	}

	//printf("exits thread");

	pthread_exit(NULL);

}

void display(channel* ch)
{
   if(ch->front == NULL)
      printf("\nQueue is Empty!!!\n");
   else{
      struct queue *temp = ch->front;
      while(temp->next != NULL){
		printf("%d--->",temp->val);
	 	temp = temp -> next;
      }
      printf("%d\n",temp->val);
   }
}  