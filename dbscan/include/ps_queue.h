#ifndef PS_QUEUE_H_
#define PS_QUEUE_H_

#include<stdio.h>  
#include<stdlib.h> 
#include<math.h>

//sequence queue  
typedef struct QueueNode  
{  
	int data;  
	struct QueueNode* next;  
}QueueNode, *QueueNodePtr;  

typedef struct LinkQueue  
{  
	QueueNodePtr front;  
	QueueNodePtr rear;  
}LinkQueue;  
      
void initialQueue(LinkQueue*);  
void insertQueue(LinkQueue*, int);  
void deleteQueue(LinkQueue*, int*);  
void printQueue(LinkQueue);  
void testQueue();  
int isEmptyQueue(LinkQueue);  
      
    //sequence queue END  

#endif

