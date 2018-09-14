#include <pthread.h> 
#include <stdio.h> 
#include <semaphore.h> 
#include <stdbool.h> 
#include <stdlib.h> 

struct queue{ 
	int val; 
	struct queue* next; 
} *front = NULL, *rear = NULL; 

typedef struct { 
	//queue qu;
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

void enqueue (int val); // write 
int dequeue (); // read
void display(); 

int main() { 

	int num = 5;
	int del; 

	//channels = (channel*)calloc(num,sizeof(channel));

	//struct queue qu; 
	enqueue(5); 
	enqueue(4); 
	enqueue(3); 

	display(); 

	del = dequeue(); 
	display(); 

	printf("\nDequeued Num: %d ", del);

	del = dequeue(); 
	display(); 

	printf("\nDequeued Num: %d ", del);

	return 0; 
}

void enqueue (int value) { 
	struct queue *newNd; 
   	newNd = (struct queue*)malloc(sizeof(struct queue));
   	newNd->val = value;
   	newNd->next = NULL;
   if(front == NULL)
      front = rear = newNd;
   else{
      rear -> next = newNd;
      rear = newNd;
   }

} // write
int dequeue () { 

	int delNum; 

   if(front == NULL)
      return 0; 
   else{
      struct queue *temp = front;
      front = front -> next;
      delNum = temp->val; 
      free(temp);
      return delNum; 
   }

} // read

void display()
{
   if(front == NULL)
      printf("\nQueue is Empty!!!\n");
   else{
      struct queue *temp = front;
      while(temp->next != NULL){
		printf("%d--->",temp->val);
	 	temp = temp -> next;
      }
      printf("%d\n",temp->val);
   }
}  