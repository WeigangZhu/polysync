  #include"ps_queue.h"
  
  /* 
     * some operation about queue 
     * */  
    void initialQueue(LinkQueue* LQ)  
    {  
        LQ->front = (QueueNodePtr)malloc(sizeof(QueueNode));  
        if( !LQ->front )  
        {  
            printf("Queue initial malloc error!");  
            exit(0);  
        }  
        LQ->rear = LQ->front;  
        LQ->rear->next = NULL;  
    }  
    void insertQueue(LinkQueue* LQ, int pointID)  
    {  
        QueueNode* new;  
        new = (QueueNodePtr)malloc(sizeof(QueueNode));  
        if( !new )  
        {  
            printf("insert queue malloc error %d\n", pointID);  
            exit(0);  
        }  
        new->data = pointID;  
        new->next = LQ->rear;  
        LQ->rear->next = new;  
        LQ->rear = new;  
    }  
    void deleteQueue(LinkQueue* LQ, int* pointID)  
    {  
        QueueNode* p = LQ->front->next;  
        *pointID = p->data;  
        LQ->front->next = p->next;  
        if( p == LQ->rear )  
            LQ->rear = LQ->front;  
        free(p);  
    }  
    void printQueue(LinkQueue LQ)  
    {  
        if( 1 == isEmptyQueue(LQ) )  
        {  
            printf("\nqueue is empty\n");  
            return;  
        }  
        LQ.front = LQ.front->next;  
        while( LQ.front != LQ.rear )  
        {  
            printf("%d ", LQ.front->data);  
            LQ.front = LQ.front->next;  
        }  
        printf("%d\n", LQ.front->data);  
    }  
    int isEmptyQueue(LinkQueue LQ)  
    {  
        return LQ.front == LQ.rear ? 1 : 0;  
    }  
    //test  
    void testQueue()  
    {  
        LinkQueue L;  
        initialQueue(&L);  
        insertQueue(&L, 1);  
        insertQueue(&L, 2);  
        insertQueue(&L, 3);  
        insertQueue(&L, 4);  
        insertQueue(&L, 5);  
        printQueue(L);  
        int test;  
        deleteQueue(&L, &test);  
        deleteQueue(&L, &test);  
        deleteQueue(&L, &test);  
        deleteQueue(&L, &test);  
        printf("is empty = %d\n", isEmptyQueue(L));  
        deleteQueue(&L, &test);  
        printf("is empty = %d\n", isEmptyQueue(L));  
        printQueue(L);  
    }  
