/****************************************************************************
 *
 * 		Copyright (C) 2005-2018 ESEC UESTC All Rights Reserved.
 ***************************************************************************/

/**
 * @file 	init.c
 * @brief
 * @date
 */



#include "include.h"
#include <stdio.h>
#include <string.h>


void showLogo();

int main(int argc ,char *argv[])
{

  //volatile int x=sizeof(OSDWORD);


 	BSP_UartInit(); 	
	BSP_TimerInit();
	EnableInterrupts;
	//showLogo();
	StartOS(OSDEFAULTAPPMODE);
	
	
	return 0;

}

//show LOGO information
void showLogo()
{
	printf("************************************************************\n");
	printf("****************ESEC OSEK INT COUNTER ALARM*****************\n");
	printf("************************************************************\n\n");
}

//output task ID and state
void showTaskInfo(TaskType taskId)
{
	TaskStateType taskState;
	GetTaskState(taskId, &taskState);
	printf("task id: %u, ", (int)taskId);
	switch(taskState)
	{
		case RUNNING:
			printf("taskState: running\n");
		break;
		case WAITING:
			printf("taskState: waiting\n");
		break;
		case READY:
			printf("taskState: ready\n");
		break;
		case SUSPENDED:
			printf("taskState: suspended\n");
	    break;
	    default:
	    break;
	}
}

/*************************************************
**************** Task1 ***************************
**************************************************
*/
TASK(Task1)
{

	TaskType taskId;
	GetTaskID(&taskId);
	//StatusType status;
	showLogo();
	
	printf("**********************task 1 running**********************\n");
	showTaskInfo(taskId); 
	


	printf("set TaskAlarm1...\n");	
	SetRelAlarm(TaskAlarm1, 3000, 10000);
	
	printf("set TaskAlarm2...\n");	
	SetRelAlarm(TaskAlarm2, 3000, 10000);
	printf("set TaskAlarm3...\n");	
	SetRelAlarm(TaskAlarm3, 3000, 10000);
	printf("set TaskAlarm4...\n");	
	SetRelAlarm(TaskAlarm4, 3000, 10000);
	printf("set TaskAlarm5...\n");	
	SetRelAlarm(TaskAlarm5, 3000, 10000);
	printf("set TaskAlarm6...\n");	
	SetRelAlarm(TaskAlarm6, 3000, 10000);
	printf("set TaskAlarm7...\n");	
	SetRelAlarm(TaskAlarm7, 3000, 10000);
	printf("set TaskAlarm8...\n");	
	SetRelAlarm(TaskAlarm8, 3000, 10000);
	
	//ActivateTask(Task6);
  //ActivateTask(Task7);
	//ActivateTask(Task8);
	//ActivateTask(Task9);
	  
	  //get Resource1
  //  status = GetResource(Resource1);
  //  printf("task1 get resource1...\n");
  //  if(status != E_OK)
   // {
  //  	printf("task1 get resource1 error: %d\n", status);	
 //   }
    
    //release Resource1
  //  printf("task1 release resource1...\n");
  //  status = ReleaseResource(Resource1);
  //  if(status != E_OK)
  //  {
   // 	printf("task1 release resource1 error: %d\n", status);	
    //}
    
//  printf("task1 wait event: 0x01\n");
//	WaitEvent(0x01);
//	printf("task1 get event 0x01...\n");	



	
 	printf("**********************task 1 end**************************\n"); 
 	
	TerminateTask();
}



/*************************************************
**************** Task2 ***************************
**************************************************
*/

TASK(Task2)
{
	TaskType taskId;
		StatusType status;

	GetTaskID(&taskId);
    printf("**********************task 2 running**********************\n");
	showTaskInfo(taskId); 
                     	  //get Resource1
   status = GetResource(Resource1);
  printf("task2 get resource1...\n");
   if(status != E_OK)
   {
   	printf("task2 get resource1 error: %d\n", status);	
    }
    
       //release Resource1
    printf("task2 release resource1...\n");
    status = ReleaseResource(Resource1);   
    if(status != E_OK)
    {
    	printf("task2 release resource1 error: %d\n", status);	
    }
    
	printf("**********************task 2 end**************************\n");
	TerminateTask();
}



/*************************************************
**************** Task3 ***************************
**************************************************
*/
TASK(Task3)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 3 running**********************\n");
    showTaskInfo(taskId); 
         	                  	  //get Resource1
   status = GetResource(Resource2);
  printf("task3 get resource2...\n");
   if(status != E_OK)
   {
   	printf("task3 get resource2 error: %d\n", status);	
    }
    
       //release Resource1
    printf("task3 release resource2...\n");
    status = ReleaseResource(Resource2);   
    if(status != E_OK)
    {
    	printf("task3 release resource2 error: %d\n", status);	
    }
	
	
	printf("**********************task 3 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** Task4 ***************************
**************************************************
*/
TASK(Task4)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 4 running**********************\n");
    showTaskInfo(taskId); 
    
	                     		                  	  //get Resource3
   status = GetResource(Resource3);
  printf("task4 get resource3...\n");
   if(status != E_OK)
   {
   	printf("task4 get resource3 error: %d\n", status);	
    }
    
       //release Resource3
    printf("task4 release resource3...\n");
    status = ReleaseResource(Resource3); 
	printf("**********************task 4 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** Task5 ***************************
**************************************************
*/
TASK(Task5)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 5 running**********************\n");
    showTaskInfo(taskId); 
    
    	  //get Resource1
   status = GetResource(Resource1);
  printf("task5 get resource1...\n");
   if(status != E_OK)
   {
   	printf("task5 get resource1 error: %d\n", status);	
    }
    
       //release Resource1
    printf("task5 release resource1...\n");
    status = ReleaseResource(Resource1);   
    if(status != E_OK)
    {
    	printf("task5 release resource1 error: %d\n", status);	
    }
	
	printf("**********************task 5 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** Task6 ***************************
**************************************************
*/
TASK(Task6)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 6 running**********************\n");
    showTaskInfo(taskId); 
    
	printf("task6 wait event: 0x60\n");
	WaitEvent(0x60);	
	printf("task6 get event 0x60...\n");	
		                  	  //get Resource12
   status = GetResource(Resource2);
  printf("task6 get resource2...\n");
   if(status != E_OK)
   {
   	printf("task6 get resource2 error: %d\n", status);	
    }
    
       //release Resource2
    printf("task6 release resource2...\n");
    status = ReleaseResource(Resource2);   
    if(status != E_OK)
    {
    	printf("task8 release resource1 error: %d\n", status);	
    }
	
	printf("**********************task 6 end**************************\n");
	TerminateTask();
}
/*************************************************
**************** Task7 ***************************
**************************************************
*/
TASK(Task7)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 7 running**********************\n");
    showTaskInfo(taskId); 
    
	printf("task7 wait event: 0x70\n");
	WaitEvent(0x70);
	
	printf("task7 get event 0x70...\n");	
	              		                  	  //get Resource3
   status = GetResource(Resource3);
  printf("task7 get resource3...\n");
   if(status != E_OK)
   {
   	printf("task7 get resource3 error: %d\n", status);	
    }
    
       //release Resource3
    printf("task7 release resource3...\n");
    status = ReleaseResource(Resource3); 
	printf("**********************task 7 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** Task8 ***************************
**************************************************
*/
TASK(Task8)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 8 running**********************\n");
    showTaskInfo(taskId); 
    
	printf("task8 wait event: 0x80\n");
	WaitEvent(0x80);
		printf("task8 get event 0x80...\n");
		
	                  	  //get Resource1
   status = GetResource(Resource1);
  printf("task8 get resource1...\n");
   if(status != E_OK)
   {
   	printf("task8 get resource1 error: %d\n", status);	
    }
    
       //release Resource1
    printf("task8 release resource1...\n");
    status = ReleaseResource(Resource1);   
    if(status != E_OK)
    {
    	printf("task8 release resource1 error: %d\n", status);	
    }
	
	printf("**********************task 8 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** Task9 ***************************
**************************************************
*/
TASK(Task9)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 9 running**********************\n");
    showTaskInfo(taskId); 
    
	printf("task9 wait event: 0x90\n");
	WaitEvent(0x90);
	
	printf("task9 get event 0x90...\n");
			                  	  //get Resource2
   status = GetResource(Resource2);
  printf("task9 get resource2...\n");
   if(status != E_OK)
   {
   	printf("task9 get resource2 error: %d\n", status);	
    }
    
       //release Resource2
    printf("task9 release resource2...\n");
    status = ReleaseResource(Resource2); 
		printf("task1 cancel TaskAlarm1...\n");
  CancelAlarm(TaskAlarm1);
	printf("task1 cancel TaskAlarm2...\n");
	CancelAlarm(TaskAlarm2);
 printf("task1 cancel TaskAlarm3...\n");
	CancelAlarm(TaskAlarm3);
	printf("task1 cancel TaskAlarm4...\n");
	CancelAlarm(TaskAlarm4);
	printf("task1 cancel TaskAlarm5...\n");
	CancelAlarm(TaskAlarm5);
	printf("task1 cancel TaskAlarm6...\n");
	CancelAlarm(TaskAlarm6);
	printf("task1 cancel TaskAlarm7...\n");
	CancelAlarm(TaskAlarm7);
	printf("task1 cancel TaskAlarm8...\n");
	CancelAlarm(TaskAlarm8);

	printf("**********************task 9 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** Task10 ***************************
**************************************************
*/
TASK(Task10)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    printf("**********************task 10 running**********************\n");
    showTaskInfo(taskId); 
	              		                  	  //get Resource3
   status = GetResource(Resource3);
  printf("task10 get resource3...\n");
   if(status != E_OK)
   {
   	printf("task10 get resource3 error: %d\n", status);	
    }
    
       //release Resource3
    printf("task10 release resource3...\n");
    status = ReleaseResource(Resource3); 
	printf("**********************task 10 end**************************\n");
	TerminateTask();
		ShutdownOS(0xff);

}

/*************************************************
**************** Task Idle ***********************
**************************************************
*/
TASK(Idle)
{
	while(1)
	{
	
	}
}