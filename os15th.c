#include <stdio.h> 
#include <stdlib.h>  
#include <pthread.h>
#include <string.h> 
#include <time.h> 
#include <semaphore.h> 

#define NUM_SEATING 3 
#define SLEEP_MAXX 5 
  
 pthread_mutex_t mutex; /* mutex */
 
  
 sem_t sem_stud;       //sematphores 
 sem_t sem_t_a;  
  
 int chairs[3]; 
 int count = 0;        //count the waiting students 
 int nexxt_bench = 0; 
 int nexxt_teach = 0; 
  
 void random_sleeps(void); 
 void* stud_programme(void* stud_ids); 
 void* ta_teaches(); 
  
 int main(int argcs, char **argvs){ 
 
  	
 	pthread_t *students; 
 	pthread_t ta; 
 	 
 	int* students_id; 
 	int students_numb; 
 	 
 	//index 
 	int i; 
 
  	 
 	printf("Number of Students are : "); 
 	scanf("%d", &students_numb); 

if (students_numb==0)
{
printf ("Teacher Assistant is sleeping,no students are their\n");
exit(-1);
}

 	
 	students = (pthread_t*)malloc(sizeof(pthread_t) * students_numb); 
 	students_id = (int*)malloc(sizeof(int) * students_numb); 
  	memset(students_id, 0, students_numb); 

  	sem_init(&sem_stud,0,0); 
 	sem_init(&sem_t_a,0,1); 
 	 
 	//set random 
 	srand(time(NULL));  
  
 	//create thread 
 	pthread_create(&ta,NULL,ta_teaches,NULL); 
 	
 	//sem_init(&mutex,0,1); 
 	pthread_mutex_init(&mutex,NULL);
 
  	    //create the threadss 
 	for(i=0; i<students_numb; i++) 
 	{ 
 		students_id[i] = i+1; 
 		pthread_create(&students[i], NULL, stud_programme, (void*) &students_id[i]); 
 	}  
 	 
 	pthread_join(ta, NULL); 
 	 
 	for(i=0; i<students_numb;i++) 
 	{ 
 		pthread_join(students[i],NULL); 
 	}	 
 	 
 	return 0; 
 } 
  
 void* stud_programme(void* stud_ids) 
 { 
 	int id = *(int*)stud_ids; 
  
 	printf("[stu] This Student %d is about to leave for TA's' office\n",id);		 
 	 
 	while(1) 
 	{ 
 		random_sleeps(); 
  
 		
 		pthread_mutex_lock(&mutex); 
 
 
if(count < NUM_SEATING)	 
 		{ 
 			chairs[nexxt_bench] = id; 
 			count++; 
 			 
 			printf("	[stu]  This Student %d is about to be waiting seated in hallway\n",id); 
 			printf("Wating student are as follows : [1] %d [2] %d [3] %d\n",chairs[0],chairs[1],chairs[2]); 
			nexxt_bench = (nexxt_bench+1) % NUM_SEATING; 
			 
			
 			pthread_mutex_unlock(&mutex); 
 	 
 		
 			sem_post(&sem_stud); 
 			sem_wait(&sem_t_a); 
 		} 
 		else //no more chairss 
 		{ 
 			
 			pthread_mutex_unlock(&mutex); 
 			 
 			printf("[stu] Their is no extra chair. Stud %d is now out of the hallway and about to be back later\n",id);		 
 		} 
 	}				 
 } 
 
 
 void* ta_teaches() 
 { 
 	while(1) 
 	{ 
 		sem_wait(&sem_stud);	 
 		 
 		
 		pthread_mutex_lock(&mutex); 
 		 
 		printf("		[ta] TA continously teaches the student %d\n",chairs[nexxt_teach]);	 
 		chairs[nexxt_teach]=0; 
 		count--; 
 		 
 		printf("Waiting Students are mentioned here: [1] %d [2] %d [3] %d\n",chairs[0],chairs[1],chairs[2]); 
 		nexxt_teach = (nexxt_teach + 1) % NUM_SEATING; 
 		 
 		random_sleeps(); 
 
 
 		printf("		[ta] Finally Teaching got finished.\n");	 
 
 
 		
 		pthread_mutex_unlock(&mutex); 
 
 
 		sem_post(&sem_t_a); 
 	}	 
 } 
 
 
 void random_sleeps(void){ 
 	int time = rand() % SLEEP_MAXX + 1; 
 	sleep(time); 
 }
