/****************************************************************************
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC All Rights Reserved.
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

//show LOGO
void showLogo()
{
	printf("************************************************************\n");
	printf("****************ESEC OSEK EVENT AND MESSAGE*****************\n");
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
	StatusType status;
	EventMaskType mask;
	
	GetTaskID(&taskId);

	showLogo();

	
	printf("**********************task 1 running**********************\n");
	
	showTaskInfo(taskId); 
	//start COM module
	StartCOM();
	
	//task1 waits for event0x01
	printf("task1 wait event: 0x01\n");
	WaitEvent(0x01);
	//get event maskcode
	GetEvent(taskId, &mask);
	printf("task1 get event: %d\n", (int)mask);
	
   //set event 0x02 for task2
	printf("task1 set event 0x02 for task2\n");
	SetEvent(Task2, 0x02);
		


	
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
//	OSBYTE buf1[] = "hello";
//	OSBYTE buf2[] = "hello again";
	EventMaskType mask;
	
	GetTaskID(&taskId);
	
    printf("**********************task 2 running**********************\n");
	showTaskInfo(taskId); 
	
	//task2 waits for event 0x02
	printf("task2 wait event: 0x02\n");
	WaitEvent(0x02);
	//get event maskcode
	GetEvent(taskId, &mask);
	printf("task2 get event: %d\n", (int)mask);
	
	
	//set event 0x07 for task7
	printf("task2 set event 0x07 for task7\n");
	SetEvent(Task7, 0x07);

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
	EventMaskType mask;

	GetTaskID(&taskId);
    printf("**********************task 3 running**********************\n");
    showTaskInfo(taskId); 
    
    //task3 waits for event 0x03
    printf("task3 wait event: 0x03\n");
	WaitEvent(0x03);
	GetEvent(taskId, &mask);
	 printf("task3 get event: %d\n", (int)mask);
	
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
	 	EventMaskType mask;

	GetTaskID(&taskId);
    printf("**********************task 4 running**********************\n");
    showTaskInfo(taskId); 
    
    //task4 waits for event 0x04
    printf("task4 wait event: 0x04\n");
	WaitEvent(0x04);
	//get event maskcode
	GetEvent(taskId, &mask);
	printf("task4 get event: %d\n", (int)mask);
	

	
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
//	StatusType status;
	 	EventMaskType mask;

	GetTaskID(&taskId);	  
    printf("**********************task 5 running**********************\n");
   	showTaskInfo(taskId); 

	//task5 waits for event 0x05
	printf("task5 wait event: 0x05\n");
	WaitEvent(0x05);
	//get event maskcode
	GetEvent(taskId, &mask);
	printf("task5 get event: %d\n", (int)mask);

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
		EventMaskType mask;

	GetTaskID(&taskId);
    printf("**********************task 6 running**********************\n");
    showTaskInfo(taskId); 

	//task6 waits for event 0x06
	printf("task6 wait event: 0x06\n");
	WaitEvent(0x06);
	//get event maskcode
	GetEvent(taskId, &mask);
	printf("task6 get event: %d\n", (int)mask);

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
	EventMaskType mask;
  OSBYTE buf1[] = "hello msg1";

	GetTaskID(&taskId);
    
    printf("**********************task 7 running**********************\n");
    showTaskInfo(taskId); 
    
    	//task7 waits for event 0x07
	printf("task7 wait event: 0x07\n");
	WaitEvent(0x07);
	//get event maskcode
	GetEvent(taskId, &mask);
	printf("task7 get event: %d\n", (int)mask);
			
	//set event 0x03 for task3
	printf("task7 set event 0x03 for task3\n");
	SetEvent(Task3, 0x03);
	//set event 0x04 for task4
	printf("task7 set event 0x04 for task4\n");
	SetEvent(Task4, 0x04);
	   	//set event 0x05 for task5
	printf("task7 set event 0x05 for task4\n");
	SetEvent(Task5, 0x05);
		//set event 0x06 for task6
	printf("task7 set event 0x06 for task4\n");
	SetEvent(Task6, 0x06);

//task7 send message Msg1
	printf("task7 send Msg1: %s\n", buf1);
	SendMessage(Msg1, buf1);
	
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
	OSBYTE buf[20];
	 OSBYTE buf1[] = "hello msg2";

	EventMaskType mask;
	
	GetTaskID(&taskId);

	
	printf("**********************task 8 running**********************\n");
	
	//show task information
	showTaskInfo(taskId); 
	
	//set event 0x01 for task1
	printf("task7 set event 0x01 for task1\n");
	SetEvent(Task1, 0x01);

	//task8 tries to receive message Msg1
	status = ReceiveMessage(Msg1, buf);
	if(status == E_OK)
	{
	 	printf("task8 receive message Msg1: %s\n", buf);
	} else
	{
		printf("task8 receive message error: %d\n", status);
	}
	
	//task8 send message Msg2
	printf("task8 send Msg2: %s\n", buf1);
	SendMessage(Msg2, buf1);
	
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
	OSBYTE buf[20];
	  OSBYTE buf1[] = "hello msg3";

	EventMaskType mask;
	
	GetTaskID(&taskId);

	
	printf("**********************task 9 running**********************\n");
	
	//show task information
	showTaskInfo(taskId); 

		        	//task9 tries to receive message Msg2
	status = ReceiveMessage(Msg2, buf);
	if(status == E_OK)
	{
	 	printf("task9 receive message Msg2: %s\n", buf);
	} else
	{
		printf("task9 receive message error: %d\n", status);
	}
		//task9 send message Msg3
	printf("task9 send Msg3: %s\n", buf1);
	SendMessage(Msg3, buf1);
	
	
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
	OSBYTE buf[20];
		  OSBYTE buf1[] = "hello msg4";

	GetTaskID(&taskId);
    printf("**********************task 10 running**********************\n");
    showTaskInfo(taskId); 

     	//task10 tries to receive message Msg3
	status = ReceiveMessage(Msg3, buf);
	if(status == E_OK)
	{
	 	printf("task10 receive message Msg3: %s\n", buf);
	} else
	{
		printf("task10 receive message error: %d\n", status);
	}
              	//task10 send message Msg4
	printf("task10 send Msg4: %s\n", buf1);
	SendMessage(Msg4, buf1);
	

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
	OSBYTE buf[20];
		  OSBYTE buf1[] = "hello msg5";

	GetTaskID(&taskId);
    printf("**********************task 11 running**********************\n");
    showTaskInfo(taskId);
    
     	//task11 tries to receive message Msg4
	status = ReceiveMessage(Msg4, buf);
	if(status == E_OK)
	{
	 	printf("task11 receive message Msg4: %s\n", buf);
	} else
	{
		printf("task11 receive message error: %d\n", status);
	}
                        	//task11 send message Msg5
	printf("task11 send Msg5: %s\n", buf1);
	SendMessage(Msg5, buf1);

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
	OSBYTE buf[20];
			  OSBYTE buf1[] = "hello msg6";

	GetTaskID(&taskId);
    printf("**********************task 12 running**********************\n");
    showTaskInfo(taskId); 

 	//task12 tries to receive message Msg5
	status = ReceiveMessage(Msg5, buf);
	if(status == E_OK)
	{
	 	printf("task12 receive message Msg5: %s\n", buf);
	} else
	{
		printf("task12 receive message error: %d\n", status);
	}
	
	              	//task12 send message Msg6
	printf("task10 send Msg6: %s\n", buf1);
	SendMessage(Msg6, buf1);
	
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
		OSBYTE buf[20];
              		  OSBYTE buf1[] = "hello msg7";

	GetTaskID(&taskId);
    printf("**********************task 13 running**********************\n");
    showTaskInfo(taskId); 

 	//task13 tries to receive message Msg6
	status = ReceiveMessage(Msg6, buf);
	if(status == E_OK)
	{
	 	printf("task13 receive message Msg6: %s\n", buf);
	} else
	{
		printf("task13 receive message error: %d\n", status);
	}
		              	//task13 send message Msg7
	printf("task13 send Msg7: %s\n", buf1);
	SendMessage(Msg7, buf1);
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
		OSBYTE buf[20];
	GetTaskID(&taskId);
    printf("**********************task 14 running**********************\n");
    showTaskInfo(taskId); 
 	//task14 tries to receive message Msg7
	status = ReceiveMessage(Msg7, buf);
	if(status == E_OK)
	{
	 	printf("task14 receive message Msg7: %s\n", buf);
	} else
	{
		printf("task14 receive message error: %d\n", status);
	}
    printf("**********************task 14 end**************************\n");
    TerminateTask();
}

