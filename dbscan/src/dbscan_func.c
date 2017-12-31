#include"dbscan.h"

 /* 
     * initialization 
     * */  
    void Init()  
    {  
        point = (Point*)malloc(sizeof(struct Point) * (data_size + 1));  
        if( !point )  
        {  
            printf("point malloc error");  
            exit(0);  
        }  
      
        coreObject_Collection = (CoreObject*)malloc(sizeof(struct CoreObject) * (data_size + 1));  
        if( !coreObject_Collection )  
        {  
            printf("coreObject_Collection malloc error!");  
            exit(0);  
        }  
        int coreObject;         //traverse  
        for( coreObject = 1; coreObject <= data_size; coreObject++ )  
        {  
            coreObject_Collection[coreObject].coreObjectID = 0;             //if the value equal 0 denote it's not core object  
            coreObject_Collection[coreObject].reachableSize = 0;                //INITIALASSIGN_DIRECTLYDENSITYREACHABLE  
            coreObject_Collection[coreObject].capacity = INITIALASSIGN_DIRECTLYDENSITYREACHABLE;  
            coreObject_Collection[coreObject].directlyDensityReachable = (int*)malloc(sizeof(int) * (INITIALASSIGN_DIRECTLYDENSITYREACHABLE + 1));  
            if( !coreObject_Collection[coreObject].directlyDensityReachable )  
            {  
                printf("coreObject_Collection malloc error: %d", coreObject);  
                exit(0);  
            }  
        }  
      
    }  
      
    /* 
     * read data from file; 
     *  set the value of point 
     * */  
    void ReadData()  
    {  
        FILE* fread;  
        if( NULL == (fread = fopen(filename, "r")))  
        {  
            printf("open file(%s) error!", filename);  
            exit(0);  
        }  
        int i;  
        for( i = 1; i <= data_size; i++ )  
        {  
            if( 2 != fscanf(fread, "%lf\t%lf", &point[i].x, &point[i].y))  
            {  
                printf("scanf error: %d", i);  
                exit(0);  
            }  
        }  
    }  
      
    /* 
     * calculate distance between two point 
     * */  
    double calculateDistance_BetweenTwo(int firstPoint, int secondPoint)  
    {     
        double temp = sqrt( pow((double)(point[firstPoint].x - point[secondPoint].x), 2) + pow((double)(point[firstPoint].y - point[secondPoint].y), 2));  
        return temp;  
    }  
      
    /* 
     * calculate distance bewteen specifed point to all others points 
     * and seek the directly_density_reachable of the specified point &pointID 
     * */  
    void calculateDistance_BetweenOneToAll(int pointID)  
    {  
        int i;  
        for( i = 1; i <= data_size; i++ )  
        {  
            if( i != pointID )  
            {  
                if( calculateDistance_BetweenTwo(pointID, i) <= neighborhood )  
                {  
                    coreObject_Collection[pointID].reachableSize++;  
                    if( coreObject_Collection[pointID].reachableSize > coreObject_Collection[pointID].capacity )  
                    {  
                        printf("\nrealloc\n\n");  
                        coreObject_Collection[pointID].directlyDensityReachable = (int*)realloc(coreObject_Collection[pointID].directlyDensityReachable, sizeof(int) * (coreObject_Collection[pointID].capacity + INCREASEMENT_DIRECTLYDENSITYREACHABLE));  
                        if( !coreObject_Collection[pointID].directlyDensityReachable )  
                        {  
                            printf("coreObject_Collection[%d].directlyDensityReachable realloc error", i);  
                            exit(0);  
                        }  
                        coreObject_Collection[pointID].capacity += INCREASEMENT_DIRECTLYDENSITYREACHABLE;  
                    }  
                    coreObject_Collection[pointID].directlyDensityReachable[coreObject_Collection[pointID].reachableSize] = i;  
                }  
            }  
        }  
    }  
      
    /* 
     * calculate distance between all points 
     * */  
    void calculateDistance_BetweenAll()  
    {  
        int i;          //traverse all the data_size  
        for( i = 1; i <= data_size; i++ )  
        {  
            calculateDistance_BetweenOneToAll(i);  
        }  
    }  
      
    /* 
     * specify the core object by statisticing the number of directly_density_reachable for all points 
     * the value of coreObject in the struct of coreObject_Collection be used to denote whether or not a core object 
     * */  
    void statisticCoreObject()  
    {  
        int i;  
        for( i = 1; i <= data_size; i++ )  
        {  
            if( coreObject_Collection[i].reachableSize >= MinPts - 1 )           //core object  
            {  
                size_of_core_object++;  
                coreObject_Collection[i].coreObjectID = i;          //ueing non_zero value to denote this point is a core_object  
            }  
        }  
    }  
      
    /* 
     * show the struct of the directly_density_reachable of all coreObject 
     * */  
    void showInformation()  
    {  
        int direct_reachable;  
        int coreObject;  
        for( coreObject = 1; coreObject <= data_size ; coreObject++ )  
        {  
            printf("%d---", coreObject_Collection[coreObject].coreObjectID);  
            for( direct_reachable = 1; direct_reachable <= coreObject_Collection[coreObject].reachableSize; direct_reachable++ )  
            {  
                printf("%d ", coreObject_Collection[coreObject].directlyDensityReachable[direct_reachable]);  
            }  
            printf("\n");  
        }  
    }  
      
    /* 
     * set the struct of @coreObject in term of the result of coreObject_Collection 
     * */  
    void setCoreObject()  
    {  
        coreObject = (CoreObject*)malloc(sizeof(struct CoreObject) * (size_of_core_object + 1));  
        if( !coreObject )  
        {  
            printf("coreObject malloc error!");  
            exit(0);  
        }  
        int i;  
        int j;  
        int count = 1;  
        for( i = 1; i <= data_size; i++ )  
        {  
            if( coreObject_Collection[i].reachableSize >= MinPts - 1 )  
            {  
                coreObject[count].coreObjectID = i;  
                coreObject[count].directlyDensityReachable = (int*)malloc(sizeof(int) * (coreObject_Collection[i].reachableSize + 1));  
                if( !coreObject[count].directlyDensityReachable )  
                {  
                    printf("coreObject[%d].directlyDensityReachable malloc error!");  
                    exit(0);  
                }  
                for( j = 1; j <= coreObject_Collection[i].reachableSize; j++ )  
                {  
                    coreObject[count].directlyDensityReachable[j] = coreObject_Collection[i].directlyDensityReachable[j];  
                }  
                coreObject[count].capacity = 0;     //change its function to flag whether this core object has beed selected  
                coreObject[count].reachableSize = coreObject_Collection[i].reachableSize;  
                count++;  
            }  
        }  
    }  
      
    /* 
     * some preparatory for the algorithem DBSCAN 
     *  create the set of Un-accessed data 
     * */  
    int* preparatory_DBSCAN()  
    {  
        //initial the Un-accessed data  
        int* UnaccessedData;  
        UnaccessedData = (int*)malloc(sizeof(int) * (data_size + 1));  
        if( !UnaccessedData )  
        {  
            printf("UnaccessedData malloc error!");  
            exit(0);  
        }  
        int i;  
        for( i = 0; i <= data_size; i++ )  
            UnaccessedData[i] = 0;          //0 denote haven't been visited  
        //seek the noise  
        for( i = 1; i <= data_size; i++ )  
        {  
            if( 0 == coreObject_Collection[i].reachableSize )  
            {  
                UnaccessedData[i] = -1;     //uses non-zero to denote the noise  
            }  
        }  
      
        return UnaccessedData;  
    }  
    
    /******************************************************************************************************************** 
     ******************************************************************************************************************** 
     * 
     *                          DBSCAN 
     * 
     ******************************************************************************************************************** 
     ********************************************************************************************************************/  
    void DBSCAN()  
    {  
        int* un_accessed_data = preparatory_DBSCAN();  
        int* old_unAccessedData;                                //save the original information of un_accessed_data  
        int i;  
        old_unAccessedData = (int*)malloc(sizeof(int) * (data_size + 1));  
        if( !old_unAccessedData )  
        {  
            printf("old_unAccessedData malloc error!");  
            exit(0);  
        }  
        for( i = 1; i <= data_size; i++ )  
            old_unAccessedData[i] = un_accessed_data[i];  
      
        //initial the queue for containing the directly_density_reachable  
        LinkQueue workQueue;  
        initialQueue(&workQueue);  
      
        int cluster_count = 0;  
        int randomCoreObjectID;  
        int pop_Queue_ID = 0;  
        int test_counter_1 = 1;  
        int test_counter_2 = 1;  
      
        while( existCoreObject() != 0 )                             //still exist core object in the @coreObject  
        {  
            printf("\n---------%d\n", test_counter_1);  
            refreshOld_unAccessed_Set(un_accessed_data, old_unAccessedData);  
            randomCoreObjectID = getRandomCoreObject();  
            addToQueue_baseCoreObject(&workQueue, randomCoreObjectID);  
            updateUnaccessSet(un_accessed_data, randomCoreObjectID);  
            test_counter_2 = 1;  
            while( !isEmptyQueue(workQueue) )  
            {  
                printf("\n\t++++++++++++%d\n", test_counter_2++);  
                deleteQueue(&workQueue, &pop_Queue_ID);  
                if( coreObject_Collection[pop_Queue_ID].reachableSize >= MinPts - 1 )  
                {  
                    addToQueue_intersectionBased(&workQueue, un_accessed_data, pop_Queue_ID);  
                }  
            }  
            cluster_count += 1;  
            printf("\ncluster_count is %d\n", cluster_count);  
            getCluster(un_accessed_data, old_unAccessedData, cluster_count);  
            updateCoreObject(un_accessed_data);  
            test_counter_1++;  
        }  
        saveNoise(un_accessed_data);  
    }  
      
      
    /* 
     * the purpose of this function is to judeg whether or not exist core_object in the @coreObject 
     *  the component in the struct of coreObject is to determin the existence of the corresponding core object  
     *      return 0: non-exist 
     *      return 1: exist 
     * */  
    int existCoreObject()  
    {  
        int core;  
        for( core = 1; core <= size_of_core_object; core++ )  
        {  
            if( 0 == coreObject[core].capacity )  
            {  
                return 1;  
            }  
        }  
        return 0;  
    }  
    /* 
     * 
     * */  
    void refreshOld_unAccessed_Set(int* un_accessed_data, int* old_unAccessedData)  
    {  
        int i;  
        for( i = 1; i <= data_size; i++ )  
        {  
            old_unAccessedData[i] = un_accessed_data[i];  
        }  
    }  
    /* 
     * select a core_object randomly 
     *  the retuen value is the ID of selected core_object 
     * */  
    int getRandomCoreObject()  
    {  
        //select a core object randomly, and insert the directly_density_reachable of it into to queue.  
        int i, j;  
        int core_object_count = 0;  
        for( i = 1; i <= size_of_core_object; i++ )  
        {  
            if( coreObject[i].capacity == 0 )  
                core_object_count += 1;  
        }  
        int* auxiliaryArray;  
        auxiliaryArray = (int*)malloc(sizeof(int) * (core_object_count + 1));  
        if( !auxiliaryArray )  
        {  
            printf("auxiliaryArray malloc error!\n");  
            exit(0);  
        }  
          
        int counter_au = 1;  
        for( i = 1; i <= size_of_core_object; i++ )  
        {  
            if( coreObject[i].capacity == 0 )       //still have not been selected  
            {  
                auxiliaryArray[counter_au++] = coreObject[i].coreObjectID;  
            }  
        }  
        int randomCoreObjectID;  
        int randomIndex;  
        int length = core_object_count;  
        randomIndex = rand() % length + 1;  
        randomCoreObjectID = auxiliaryArray[randomIndex];  
        auxiliaryArray[randomIndex] = auxiliaryArray[length--];  
        return randomCoreObjectID;  
    }  
    /* 
     * after selected a random core_object, we need to add the directly_density_reachable of this core object to the queue 
     *  particular note: instead use the coreObject, we need to use the original struct coreObject_Collection, 
     *           because of the incomplete in the index of @coreObject. 
     * 
     * */  
    void addToQueue_baseCoreObject(LinkQueue* LQ, int coreObjectID)  
    {  
        int i;  
        //printf("add to queue, the reachable of coreObject is %d\n", coreObject_Collection[coreObjectID].reachableSize);  
        for( i = 1; i <= coreObject_Collection[coreObjectID].reachableSize; i++ )  
        {  
            insertQueue(LQ, coreObject_Collection[coreObjectID].directlyDensityReachable[i]);  
        }  
    }  
    /* 
     * after selected a random core_object. we need to update the information about un-accessed-set 
     *  particular note: instead use the coreObject, we need to use the original struct coreObject_Collection, 
     *           because of the incomplete in the index of @coreObject. 
     * */  
    void updateUnaccessSet(int* un_accessed_data, int randomCoreObjectID)  
    {  
        int i;  
        for( i = 1; i <= coreObject_Collection[randomCoreObjectID].reachableSize; i++ )  
        {  
            un_accessed_data[coreObject_Collection[randomCoreObjectID].directlyDensityReachable[i]] = coreObject_Collection[randomCoreObjectID].directlyDensityReachable[i];  
        }  
        un_accessed_data[randomCoreObjectID] = randomCoreObjectID;      //core object has visited  
    }  
    /* 
     * if exist the core_object in the list of directly_density_reachable of other core_object 
     * add the element in the @workqueue which is not even dealed with of the core_object 
     * and update the @un_accessed_set. 
     * */  
    void addToQueue_intersectionBased(LinkQueue* LQ, int* un_accessed_set, int pop_Queue_ID)  
    {  
        int core_DDR;           //trverse the core_directly_reachable of pop_Queue_ID  
        for( core_DDR = 1; core_DDR <= coreObject_Collection[pop_Queue_ID].reachableSize; core_DDR++ )  
        {  
            if( 0 == un_accessed_set[coreObject_Collection[pop_Queue_ID].directlyDensityReachable[core_DDR]] )  
            {  
                insertQueue(LQ, coreObject_Collection[pop_Queue_ID].directlyDensityReachable[core_DDR]);  
                un_accessed_set[coreObject_Collection[pop_Queue_ID].directlyDensityReachable[core_DDR]] = coreObject_Collection[pop_Queue_ID].directlyDensityReachable[core_DDR];  
            }  
        }  
    }  
    /* 
     * get cluster based on a core object 
     * */  
    void getCluster(int* un_accessed_data, int* old_unAccessedData, int clusterID)  
    {  
        char filename[200];  
        sprintf(filename, ".//DBSCAN_cluster//cluster_%d.data", clusterID);  
        FILE* fwrite;  
        if( NULL == (fwrite = fopen(filename, "w")))  
        {  
            printf("open file(%s) error", filename);  
            exit(0);  
        }  
        int i;  
        for( i = 1; i <= data_size; i++ )  
        {  
            if( un_accessed_data[i] != old_unAccessedData[i] )  
            {  
                fprintf(fwrite, "%f\t%f\n", point[i].x, point[i].y);  
            }  
        }  
        fclose(fwrite);  
    }  
    /* 
     * 
     * */  
    void updateCoreObject(int* un_accessed_data)  
    {  
        int i;  
        for( i = 1; i <= size_of_core_object; i++ )  
        {  
            if( 0 != un_accessed_data[coreObject[i].coreObjectID] )  
            {  
                coreObject[i].capacity = 1;         //denote this core object has been dealed  
            }  
        }  
    }  
    void saveNoise(int* un_accessed_data)  
    {  
        FILE* fwriteNoise;  
        if( NULL == (fwriteNoise = fopen(".//DBSCAN_cluster//noise.data", "w")))  
        {  
            printf("open file(nosie.data) error!");  
            exit(0);  
        }  
        int i;  
        printf("\nshow the noise data:\n");  
        for( i = 1; i <= data_size; i++ )  
        {  
            if( un_accessed_data[i] == -1 || un_accessed_data[i] == 0 )  
            {  
                fprintf(fwriteNoise, "%f\t%f\n", point[i].x, point[i].y);  
                printf("%f\t%f\n", point[i].x, point[i].y);  
            }  
        }  
    }  
    
