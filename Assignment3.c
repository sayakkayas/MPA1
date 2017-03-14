/*Assignment 3, Multiprocessor architecture :- Matrix multiplication using pthreads
Modified code from assignment 1-Now the threads don't work based on separate rows but elements
created by Sayak Chakraborti , February 2017
Compile-gcc assignment3.c -lpthread -lrt -w
Execute-./a.out num_threads <input > output 
Example-./a.out 2 < mat200x200.txt > output2I2x2.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#define NANO 1000000000

int M,N;
int num_threads=1;
 int ** arr;   //input matrix provided
   int ** out_arr;
bool begin=false;

struct rows_assigned            //data structure to assign start and end rows to the threads 
{
   int index;
  

};

void * threadFunction(void* argument);

void time_difference(struct timespec *start_time, struct timespec *end_time, struct timespec *result)
{
	if (( (end_time->tv_nsec)-(start_time->tv_nsec) )<0) 
        {
		result->tv_sec=(end_time->tv_sec)-(start_time->tv_sec)-1;
		result->tv_nsec=(end_time->tv_nsec)-(start_time->tv_nsec)+NANO;
	}
       else 
       {
		result->tv_sec= (end_time->tv_sec)-(start_time->tv_sec);
		result->tv_nsec= (end_time->tv_nsec)-(start_time->tv_nsec);
	}
}

int main(int argc,char** argv)
{

 if(argc!=2)
 	{ printf("Error in input,terminate\n");
      exit(0);
     }
     else
     {     // printf("%d\n",atoi(argv[1]));
           num_threads=atoi(argv[1]);
     }

     readInput(&M,&N);

      
            
        //printf("All okay\n");

       clockid_t clk_id;
      struct timespec ts1,ts2,diff;

        // printf("Num ops %d\n",num_ops);
        //thread creation
          
         pthread_t* thr;

          
         thr=(pthread_t*)malloc(sizeof(pthread_t)*num_threads);

           begin=false;       
         pthread_attr_t* attr; //Set of thread attributes
         
         attr=(pthread_attr_t*)malloc(num_threads*sizeof(pthread_attr_t));
         
          
         
         struct rows_assigned * r;
         r=(struct rows_assigned*)malloc(num_threads*sizeof(struct rows_assigned));  //data structure passed to the threads
      int i,j;  

          for(i=0;i<num_threads;i++)
           r[i].index=(i+1);
        
          

         for(i=0;i<num_threads;i++)
         pthread_attr_init(&attr[i]);

          clock_gettime(clk_id,&ts1);

       for(i=0;i<num_threads;i++)
         {

        	  if(pthread_create(&thr[i],&attr[i],threadFunction,(void*)&r[i]))
      			{ printf("Error creating thread ");
          			exit(0);
        		}
        		else
        			printf("Thread %d created \n",(i+1));
         }

        begin=true;//so that all threads start at the same time
        //main thread waiting for the threads to terminate
       for(i=0;i<num_threads;i++)
        pthread_join(thr[i],NULL);
   
     clock_gettime(clk_id,&ts2);

     time_difference(&ts1,&ts2,&diff);

      printf("Time spent %lld secs\n",((diff.tv_sec*NANO)+diff.tv_nsec));

    //output commented out
      /*for(i=0;i<M;i++)
      {
         for(j=0;j<N;j++)
         printf("%d ",out_arr[i][j]);
         printf("\n");

      }*/


return 0;	
}

void * threadFunction(void* argument)
{
	  struct rows_assigned* data=argument;

    while(!begin);
     int count =1;  
    int i,j,k;
    for(i=0;i<N;i++)
    {
        int sum=0;
       for(j=0;j<N;j++)
        {
            for(k=0;k<M;k++)
            {
               if(count%(data->index)==0)           //checks whether the index to be served is it's or not
               sum=sum+arr[i][k]*arr[k][j];         //so a thread serves(calculates the product and sum results only if the indexto be served is it's own)
             }
             if(count%(data->index)==0)
             out_arr[i][j]=sum;

             //printf("Count %d\n",count);
             count++;
             sum=0;
        }
    }

}


//read input from file
void readInput(int* M,int * N)
{
   
    int i,j;
    scanf("%d %d",M,N);       //scanning the array dimensions

    //printf("%dx%d\n",(*M),(*N));

    arr=(int**)malloc((*M)*sizeof(int*));            //dynamic declaration of a 2d array based on the dimensions
    out_arr=(int**)malloc((*M)*sizeof(int*));

    for(i=0;i<(*M);i++)
    {arr[i]=(int*)malloc((*N)*sizeof(int));
     out_arr[i]=(int*)malloc((*N)*sizeof(int));
     }
     //matrix populated
    for(i=0;i<(*M);i++)
    {

    	 for(j=0;j<(*N);j++)
    	 {scanf("%d",&arr[i][j]);
    	  //printf("%d\n",arr[i][j]);
         } 
   	 
    }

   



}
