#ifndef DBSCAN_H_
#define DBSCAN_H_
    
#include<stdio.h>  
#include<stdlib.h>  
#include<math.h>  
#include<time.h>  
#include<string.h>  
      
//#define INITIALASSIGN_COREOBJECT      100  
//#define INCREASEMENT_COREOBJECT       100       
#define INITIALASSIGN_DIRECTLYDENSITYREACHABLE  100  
#define INCREASEMENT_DIRECTLYDENSITYREACHABLE   10  

      
typedef struct Point  
{  
	double x;  
	double y;  
}Point;  

typedef struct CoreObject  
{  
	int coreObjectID;  
	int* directlyDensityReachable;  //store the directly density_reachable point of corePointID  
	int reachableSize;      //the number of directly density reachable  
	int capacity;           //the current capacity of the dynamic array @directlyDensityReachable  
}CoreObject;  

      
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
      
void Init();  
void ReadData();  
double calculateDistance_BetweenTwo(int, int);  
void calculateDistance_BetweenOneToAll(int);  
void calculateDistance_BetweenAll();  
void statisticCoreObject();  
void showInformation();  
void setCoreObject();  
int* preparatory_DBSCAN();  
void DBSCAN();  
void refreshOld_unAccessed_Set(int*, int*);  
int existCoreObject();  
int getRandomCoreObject();  
void addToQueue_baseCoreObject(LinkQueue*, int);  
void updateUnaccessSet(int*, int);  
void addToQueue_intersectionBased(LinkQueue*, int*, int);  
void getCluster(int*, int*, int);  
void updateCoreObject(int*);  
void saveNoise(int*);  
    
#endif
      
  
