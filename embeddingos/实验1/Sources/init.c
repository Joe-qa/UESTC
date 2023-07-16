/****************************************************************************
 *
 * 		Copyright (C) 2005-2018 ESEC UESTC All Rights Reserved.
 ***************************************************************************/

/*
 *	20060208	file created.
 *
 */

/**
 * @file 	init.c
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

//display LOGO information
void showLogo()
{
	printf("************************************************************\n");
	printf("**********************ESEC OSEK TASK************************\n");
	printf("************************************************************\n\n");
}

//output task ID and states
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
	showLogo();
	printf("**********************task 1 running**********************\n");
	showTaskInfo(taskId); 
	printf("activate task4...\n");
	ActivateTask(Task4);
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
	GetTaskID(&taskId);
    printf("**********************task 2 running**********************\n");
	showTaskInfo(taskId);
	printf("activate task5...\n");
	ActivateTask(Task5);
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
	GetTaskID(&taskId);
    printf("**********************task 3 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task6...\n");
    ActivateTask(Task6);
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
	GetTaskID(&taskId);
    printf("**********************task 4 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task7...\n");
    ActivateTask(Task7);
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
    printf("activate task8...\n");
    ActivateTask(Task8);
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
    printf("activate task9...\n");
    ActivateTask(Task9);
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
    printf("activate task10...\n");
    ActivateTask(Task10);
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
    printf("activate task11...\n");
    ActivateTask(Task11);
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
    printf("activate task12...\n");
    ActivateTask(Task12);
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
    printf("activate task13...\n");
    ActivateTask(Task13);
    printf("**********************task 10 end**************************\n");
    TerminateTask();
}

/*************************************************
**************** Task11 ***************************
**************************************************
*/
TASK(Task11)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 11 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task14...\n");
    ActivateTask(Task14);
    printf("**********************task 11 end**************************\n");
    TerminateTask();
}

/*************************************************
**************** Task12 ***************************
**************************************************
*/
TASK(Task12)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 12 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task15...\n");
    ActivateTask(Task15);
    printf("**********************task 12 end**************************\n");
    TerminateTask();
}


/*************************************************
**************** Task13 ***************************
**************************************************
*/
TASK(Task13)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 13 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task16...\n");
    ActivateTask(Task16);
    printf("**********************task 13 end**************************\n");
    TerminateTask();
}

/*************************************************
**************** Task14 ***************************
**************************************************
*/
TASK(Task14)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 14 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task17...\n");
    ActivateTask(Task17);
    printf("**********************task 14 end**************************\n");
    TerminateTask();
}

/*************************************************
**************** Task15 ***************************
**************************************************
*/
TASK(Task15)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 15 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task18...\n");
    ActivateTask(Task18);
    printf("**********************task 15 end**************************\n");
    TerminateTask();
}
/*************************************************
**************** Task16 ***************************
**************************************************
*/
TASK(Task16)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 16 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task1...\n");
    ActivateTask(Task1);
    printf("**********************task 16 end**************************\n");
    TerminateTask();
}
/*************************************************
**************** Task17 ***************************
**************************************************
*/
TASK(Task17)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 17 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task2...\n");
    ActivateTask(Task2);
    printf("**********************task 17 end**************************\n");
    TerminateTask();
}
/*************************************************
**************** Task18 ***************************
**************************************************
*/
TASK(Task18)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);

    printf("**********************task 18 running**********************\n");
    showTaskInfo(taskId); 
    printf("activate task3...\n");
    ActivateTask(Task3);
    printf("**********************task 18 end**************************\n");
    TerminateTask();
}
