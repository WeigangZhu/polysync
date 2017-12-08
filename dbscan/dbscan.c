#include"dbscan.h"

int main(int argc, char* argv[])  
    {  
        if( argc != 5 )  
        {  
            printf("this program need 5 parematers to run,"  
                    "\n\t\tthe first to indicate the neighborhood"  
                    "\n\t\tthe second to indicate the MinPts"  
                    "\n\t\tthe third to indicate the filename contain data"  
                    "\n\t\tthe fourth to indicate the data size");  
            exit(0);  
        }  
        srand((unsigned)time(NULL));  
        neighborhood = atof(argv[1]);  
        MinPts = atoi(argv[2]);  
        strcat(filename, argv[3]);  
        data_size = atoi(argv[4]);  
      
        Init();  
        ReadData();  
        calculateDistance_BetweenAll();  
        statisticCoreObject();  
        //showInformation();  
        setCoreObject();  
        //testQueue();  
        DBSCAN();  
        return 0;  
    }  
