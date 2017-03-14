/*Created by Sayak Chakraborti Feb 2017,
to check consistency model,
Check whether sequential consistency breaks ,which does with output of a=0,b=0.
Thus we now confirm that SC is not maintained and check for furthur cases
Compile-gcc MemoryConsistencyModel.c -lpthread -o mem1 -w
Run-./mem1
*/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

int x=0,y=0;
bool start=false;
int a,b;
//thread 1 executes this function
void *threadFunc1()
{
	while(!start); //actual computation in the threads doesn't start until this condition is false(this is so that we can actually have them running parallely)
   x=1;
  y=1;
  a=x;
  
  printf("Thread 1  %d\n",a);
  fflush(stdout);

}

//thread 2 executes this function
void *threadFunc2()
{
	while(!start);    //actual computation in the threads doesn't start until this condition is false(this is so that we can actually have them running parallely)
   x=1;
   b=y;
   
printf("Thread 2 %d\n",b);
fflush(stdout);

}

int main()
{
    pthread_t thr1;
    pthread_t thr2;

    
      //creating two threads
 
     start=false;
     x=0,y=0;
      if(pthread_create(&thr1,NULL,threadFunc1,NULL))  //creating thread 1
      	{ printf("Error creating thread 1");
          exit(0);
        }

      if(pthread_create(&thr2,NULL,threadFunc2,NULL))   //creating thread 2
      	{ printf("Error creating thread 1");
          exit(0);
        }

     start=true;   //now the threads can start actual computation parallely once both of them have been created
   //waiting for the threads to terminate
    pthread_join(thr1,NULL);
    pthread_join(thr2,NULL);
    
	return 0;
}