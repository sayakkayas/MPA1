/*Created by Sayak Chakraborti Feb 2017,
to check consistency model of Total Store Order(TSO),
it breaks with output Thread 1 rd1=1 rd2=0 and Thread2 rd1=1 rd2=0
Compile-gcc MemoryConsistencyModel2.c -lpthread -o mem2 -w
Run-./mem2
*/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

int x=0,y=0;
bool start=false;
int a=0,b=0;

//this is the function thread1 executes
void *threadFunc1()
{
	while(!start); //spin wait
  a=1;   //write 1 
  x=1;   //write 2
  int rd1=x;
  int rd2=b;
  printf("Thread1  rd1 %d  rd2 %d\n",rd1,rd2);
  fflush(stdout);

}

//this is the function thread2 executes
void *threadFunc2()
{
	while(!start); //spin wait
   b=1;   //write 1 
   y=1;   //write 2
  int  rd1=y;
   int rd2=a;
  printf("Thread2  rd1 %d  rd2 %d\n",rd1,rd2);
  fflush(stdout);

}

int main()
{
    pthread_t thr1;
    pthread_t thr2;

    
      //creating two threads
 
     start=false;
     x=0,y=0;
      if(pthread_create(&thr1,NULL,threadFunc1,NULL))  //create thread 1
      	{ printf("Error creating thread 1");
          exit(0);
        }

      if(pthread_create(&thr2,NULL,threadFunc2,NULL)) //create thread 2
      	{ printf("Error creating thread 1");
          exit(0);
        }

     start=true;      //now everyone starts
   //waiting for the threads to terminate
    pthread_join(thr1,NULL);
    pthread_join(thr2,NULL);
    
	return 0;
}