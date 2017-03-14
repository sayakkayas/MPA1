
/*Assignment 3, Multiprocessor architecture :- Matrix multiplication using MPI
created by Sayak Chakraborti , Feb 2017
Not single address space so have to pass on elements to each other
Pass messages from the master thread to the slave ones and this created a copy for each of the threads,memory extensive but no process
gets in anyones way.NOTE:In order to run you do need to have the input file in the same directory as the executable
Compile- mpicc MPI_mat.c -o mpi_200 -w -lrt
Run-mpirun -np 4 mpi_200
If want ot compile for the 4000 matrix change the name of the input file on line 249 to mat4000x4000.txt and 
Compile-mpicc MPI_mat.c -o mpi_4000 -w -lrt
Run-mpirun -np 4 mpi_4000
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<time.h>
#include<stdbool.h>
#include<mpi.h>
#define NANO 1000000000
#define send_data_tag 2001
#define return_data_tag 2002


	int M,N;
	int num_processes=1;
	int ** arr;   //input matrix provided
   	int ** out_arr;            //output matrix

        int ierr;

      clockid_t clk_id;
      struct timespec ts1,ts2,diff;
         struct rows_assigned * r;
     MPI_Status status;

struct rows_assigned            //data structure to assign start and end rows to the processes
{
   int start;
   int end;

};

void  threadFunction(struct rows_assigned* data);

//function to calculate the time difference
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

   ierr=MPI_Init(&argc,&argv);

    int process_id,num_processes,i,j;
   ierr=MPI_Comm_rank(MPI_COMM_WORLD,&process_id);
   ierr=MPI_Comm_size(MPI_COMM_WORLD,&num_processes);  
     

   if(process_id==0)
     { readInput(&M,&N);

  
       int num_ops=M/num_processes;
         int num_ops_spec,i,j;

         if(num_ops*num_processes==M)
         {
         	//equal division of load among processes
         	num_ops_spec=0;
         }
         else
         {
            num_ops_spec=M-(num_ops*(num_processes-1));

         }

            
        //printf("All okay\n");

     
         //printf("Num ops %d\n",num_ops);
         //printf("Num ops spec %d\n",num_ops_spec);
        //thread creation
          
        
          
         
         r=(struct rows_assigned*)malloc(num_processes*sizeof(struct rows_assigned));
         
          
          //assign rows to processes
         int s=0;
         for(i=0;i<num_processes-1;i++)
         { r[i].start=s;
           r[i].end=s+num_ops;
           s=s+num_ops;
         }

         if(num_ops_spec!=0)
         {
         	r[i].start=s;
         	r[i].end=s+num_ops_spec;
         }
         else
         {
         	 r[i].start=s;
         	r[i].end=s+num_ops;

         }
  

      
     for(i=1;i<num_processes;i++)
      {
        ierr=MPI_Send(&(r[i].start),1,MPI_INT,i,send_data_tag,MPI_COMM_WORLD);
       ierr=MPI_Send(&(r[i].end),1,MPI_INT,i,send_data_tag,MPI_COMM_WORLD);
      
      }

     clock_gettime(clk_id,&ts1);
 
     ProcessFunction(&r[0]);

     //printf("Process 0 Start %d End %d\n",r[0].start,r[1].end);
           
       //   display_matrix(out_arr,M,N);
    

         int ack=0;
          for(i=1;i<num_processes;i++)
          {
            ierr=MPI_Recv(&(out_arr[r[i].start][0]),N*(r[i].end-r[i].start),MPI_INT,i, return_data_tag, MPI_COMM_WORLD, &status );
            ierr = MPI_Recv( &ack, 1, MPI_INT,i, return_data_tag, MPI_COMM_WORLD, &status);
       
          }

          //     ierr=MPI_Finalize();
          // printf("\n\n");
     
          //printf("Process 0 Start %d End %d\n",r[0].start,r[1].end);
           
         // display_matrix(out_arr,M,N);
    
       clock_gettime(clk_id,&ts2);
  
                time_difference(&ts1,&ts2,&diff);


                   //output

                    printf("Num Process %d Time spent %f secs\n",num_processes,(double)((diff.tv_sec*NANO)+diff.tv_nsec)/1000000000.0 );

             
      //ierr=MPI_Finalize();
     
     
   }//process ID 0 finish
   else
   {
        struct rows_assigned r;     
        int start,end; 
         readInput(&M,&N);

         ierr = MPI_Recv( &start, 1, MPI_INT,0, send_data_tag, MPI_COMM_WORLD, &status);
         ierr = MPI_Recv( &end, 1, MPI_INT,0, send_data_tag, MPI_COMM_WORLD, &status);
          r.start=start;
          r.end=end;        
          
          ProcessFunction(&r);
                
          // printf("Process %d Start %d  End %d M %d N %d\n",process_id,start,end,M,N);
         // display_matrix(arr,M,N);     
                //  printf("\n\n");
    
                
         //  display_matrix(out_arr,M,N);
                  //printf("%d\n",N*(r.end-r.start));
            ierr=MPI_Send(&(out_arr[r.start][0]),N*(r.end-r.start),MPI_INT,0,return_data_tag,MPI_COMM_WORLD);  //send data back
                                      
          
                 int flag=1;
            ierr=MPI_Send(&flag,1,MPI_INT,0,return_data_tag,MPI_COMM_WORLD);  //send back acknowledgement of completed computation

 
       //  ierr=MPI_Finalize();
     
  }


ierr=MPI_Finalize();
 
   
return 0;	
}

void display_matrix(int** arr,int M,int N)
{
int i,j;
    for(i=0;i<M;i++)
                  {
                     for(j=0;j<N;j++)
                         printf("%d ",arr[i][j]);
                         printf("\n");

                }


}



//Process function that each thread is going to run with different parameters
void ProcessFunction(struct rows_assigned * data)
{
	//printf("ProcessFunction %d %d\n",data->start,data->end);
 
    int i,j,k;
    for(i=data->start;i<data->end;i++)
    {
        int sum=0;
       for(j=0;j<N;j++)
        {
            for(k=0;k<M;k++)
             sum=sum+arr[i][k]*arr[k][j];
           
             out_arr[i][j]=sum;
             sum=0;
        }
    }

}

//read input from file
void readInput(int* M,int * N)
{
   
   FILE * fp=fopen("mat200x200.txt","r");
    int i,j;
    fscanf(fp,"%d %d",M,N);       //scanning the array dimensions

    //printf("%dx%d\n",(*M),(*N));

    int* data1;int *data2;
    data1=(int*)malloc((*M)*(*N)*sizeof(int));
    data2=(int*)malloc((*M)*(*N)*sizeof(int));
    
    arr=(int**)malloc((*M)*sizeof(int*));            //dynamic declaration of a 2d array based on the dimensions
    out_arr=(int**)malloc((*M)*sizeof(int*));

    for(i=0;i<(*M);i++)
    {arr[i]=&(data1[(*N)*i]);
     out_arr[i]=&(data2[(*N)*i]);
     }  
     //matrix populated
    for(i=0;i<(*M);i++)
    {

    	 for(j=0;j<(*N);j++)
    	 {fscanf(fp,"%d",&arr[i][j]);
    	  //printf("%d\n",arr[i][j]);
         } 
   	 
    }

   
fclose(fp);


}
