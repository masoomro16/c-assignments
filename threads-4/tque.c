#include <pthread.h> 
#include <stdio.h> 
#include <semaphore.h> 
#include <stdbool.h> 
#include <stdlib.h> 

typedef struct queue{ 
	int val; 
	struct queue* next; 
} queue; //*front = NULL, *rear = NULL; 

typedef struct channel{ 
	struct queue *front, *rear; 
} channel; 

// usage: 
// channel[i].qu[i]->nd = NULL 

typedef struct { 
	int uid; 
	int twoHop; 
	int oneHop; 
	int temp; 
	bool active; // active or relay 
	bool leader; // leader or not 
} node; 

void enqueue (channel* ch, int value); // write 
int dequeue (channel* ch); // read
void display(channel* ch); 

int main() { 

	int num = 5;
	int del; 
	channel* channels; 

	channels = (channel*)calloc(num,sizeof(channel));

	//initalize channel
	int i;

	for (i=0; i<num; i++) { 
		channels[i].front=NULL; 
		channels[i].rear=NULL; 
	} 
	for (i=0; i<num; i++) {

		printf("\n************* CHANNEL #%d *************", i); 
		enqueue(&channels[i], 5); 
		enqueue(&channels[i], 4); 
		enqueue(&channels[i], 3); 

		printf("\n");
		display(&channels[i]); 

		del = dequeue(&channels[i]); 
				del = dequeue(&channels[i]); 
						del = dequeue(&channels[i]); 
		display(&channels[i]); 

		printf("\nDequeued Num: %d ", del);
	}

	return 0; 
}

void enqueue (channel* ch, int value) { 
	struct queue *newNd; 
   	newNd = (struct queue*)malloc(sizeof(struct queue));
   	newNd->val = value;
   	newNd->next = NULL;
   if(ch->front == NULL)
      ch->front = ch->rear = newNd;
   else{
      ch->rear -> next = newNd;
      ch->rear = newNd;
   }

} // write
int dequeue (channel* ch) { 

	int delNum; 

   if(ch->front == NULL)
      return 0; 
   else{
      struct queue *temp = ch->front;
      ch->front = ch->front -> next;
      delNum = temp->val; 
      free(temp);
      return delNum; 
   }

} // read

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