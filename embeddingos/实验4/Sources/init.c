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
**************** Taskini_LLD ***************************
**************************************************
*/
TASK(Taskini_LLD)
{
	TaskType taskId;
	GetTaskID(&taskId);
	
	showLogo();
	//start COM module
	StartCOM();
	printf("**********************Taskini_LLD running**********************\n");
	showTaskInfo(taskId); 
	
	printf("activate TaskHLS_ini...\n");
	ActivateTask(HLS_ini);
	
	
 	printf("**********************Taskini_LLD end**************************\n"); 
 	
	TerminateTask();
}



/*************************************************
**************** HLS_ini ***************************
**************************************************
*/

TASK(TaskHLS_ini)
{
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************TaskHLS_ini running**********************\n");

	showTaskInfo(taskId); 
	
	printf("activate TaskHLS_inisyn...\n");
	ActivateTask(HLS_inisyn);

	
	printf("**********************TaskHLS_ini end**************************\n");
	TerminateTask();
}



/*************************************************
**************** HLS_inisyn ***************************
**************************************************
*/
TASK(TaskHLS_inisyn)
{ 
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************TaskHLS_inisyn running**********************\n");
    showTaskInfo(taskId); 
  printf("activate TaskHLS_ini2...\n");
	ActivateTask(HLS_ini2);

	printf("**********************TaskHLS_inisyn end**************************\n");
	TerminateTask();
}

/*************************************************
**************** HLS_ini2 ***************************
**************************************************
*/
TASK(TaskHLS_ini2)
{ 
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************TaskHLS_ini2 running**********************\n");
    showTaskInfo(taskId); 
  printf("activate Taskini_LLD2...\n");
	ActivateTask(ini_LLD2);
	
	printf("**********************TaskHLS_ini2 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** ini_LLD2 ***************************
**************************************************
*/
TASK(Taskini_LLD2)
{ 
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************Taskini_LLD2 running**********************\n");
    showTaskInfo(taskId); 
    
	          
	printf("set TaskAlarm1...\n");	
	SetRelAlarm(TaskAlarm1, 3000, 50); 
	
		printf("set TaskAlarm2...\n");	
	SetRelAlarm(TaskAlarm2, 3000, 100);
	 
		printf("set TaskAlarm3...\n");	
	SetRelAlarm(TaskAlarm3, 3000, 200);
	 
		printf("set TaskAlarm4...\n");	
	SetRelAlarm(TaskAlarm4, 3000, 1000); 
	printf("**********************Taskini_LLD2 end**************************\n");
	TerminateTask();

}

/*************************************************
**************** Taskperiod50 ***************************
**************************************************
*/
TASK(Taskperiod50)
{ 
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************Taskperiod50 running**********************\n");
    showTaskInfo(taskId); 
    
	  printf("Taskperiod50 cancel TaskAlarm1...\n");
  	CancelAlarm(TaskAlarm1);
  	
  printf("Taskperiod50 set event 0x10 for TaskHLS_syn2...\n");
	SetEvent( HLS_syn2, 0x10);
	printf("**********************Taskperiod50 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** Taskperiod100 ***************************
**************************************************
*/
TASK(Taskperiod100)
{ 
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************Taskperiod100 running**********************\n");
    showTaskInfo(taskId); 
    
	  printf("Taskperiod100 cancel TaskAlarm2...\n");
  	CancelAlarm(TaskAlarm2);
  	
  	  printf("Taskperiod100 set event 0x20 for TaskHLS_rstsyn...\n");
	SetEvent( HLS_rstsyn, 0x20);
	printf("**********************Taskperiod100 end**************************\n");
	TerminateTask();
}
/*************************************************
**************** Taskperiod200 ***************************
**************************************************
*/
TASK(Taskperiod200)
{ 
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************Taskperiod200 running**********************\n");
    showTaskInfo(taskId); 
            
	  printf("Taskperiod200 cancel TaskAlarm3...\n");
  	CancelAlarm(TaskAlarm3);
	
	  printf("Taskperiod200 set event 0x30 for TaskHLS_firstsyn...\n");
	SetEvent( HLS_firstsyn, 0x30);
	
	printf("**********************Taskperiod200 end**************************\n");
	TerminateTask();
}
/*************************************************
**************** Taskperiod1000 ***************************
**************************************************
*/
TASK(Taskperiod1000)
{ 
	TaskType taskId;
	GetTaskID(&taskId);
    printf("**********************Taskperiod1000 running**********************\n");
    showTaskInfo(taskId); 
                     
	  printf("Taskperiod1000 cancel TaskAlarm4...\n");
  	CancelAlarm(TaskAlarm4);
	
		  printf("Taskperiod1000 set event 0x40 for TaskHLS_syn...\n");
	SetEvent( HLS_syn, 0x40);
	
	printf("**********************Taskperiod1000 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskHLS_inisyn2 ***************************
**************************************************
*/
TASK(TaskHLS_syn2)
{ 
	TaskType taskId;
	OSBYTE buf1[] = "hello COM_task1";
	GetTaskID(&taskId);
    printf("**********************TaskHLS_syn2 running**********************\n");
    
   	printf("TaskHLS_syn2 wait event: 0x10\n");
	WaitEvent(0x10);
	printf("TaskHLS_syn2 get event 0x10...\n");
	showTaskInfo(taskId); 

 printf("activate COM_task1...\n");
	ActivateTask(COM_task1);
	
 //send message Msg1
	printf("TaskHLS_inisyn2 send Msg1: %s\n", buf1);
	SendMessage(Msg1, buf1);
	
	printf("**********************TaskHLS_syn2 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskHLS_rstsyn ***************************
**************************************************
*/
TASK(TaskHLS_rstsyn)
{ 
	TaskType taskId;
		OSBYTE buf1[] = "hello COM_task2";
	GetTaskID(&taskId);
    printf("**********************TaskHLS_rstsyn running**********************\n");
    
 printf("TaskHLS_rstsyn wait event: 0x20\n");
	WaitEvent(0x20);
	printf("TaskHLS_rstsyn get event 0x20...\n");   
	          
	 showTaskInfo(taskId); 

 printf("activate COM_task2...\n");
	ActivateTask(COM_task2);
	
	
	//send message Msg2
	printf("TaskHLS_rstsyn send Msg2: %s\n", buf1);
	SendMessage(Msg2, buf1);
	
	printf("**********************TaskHLS_rstsyn end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskHLS_firstsyn ***************************
**************************************************
*/
TASK(TaskHLS_firstsyn)
{ 
	TaskType taskId;
		OSBYTE buf1[] = "hello COM_task3";
	GetTaskID(&taskId);
    printf("**********************TaskHLS_firstsyn running**********************\n");
                     
  printf("TaskHLS_firstsyn wait event: 0x30\n");
	WaitEvent(0x30);
	printf("TaskHLS_firstsyn get event 0x30...\n");  
	
	showTaskInfo(taskId); 

 printf("activate COM_task3...\n");
	ActivateTask(COM_task3);
	
		//send message Msg3
	printf("TaskHLS_firstsyn send Msg3: %s\n", buf1);
	SendMessage(Msg3, buf1);
	
	printf("**********************TaskHLS_firstsyn end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskHLS_syn ***************************
**************************************************
*/
TASK(TaskHLS_syn)
{ 
	TaskType taskId;
		OSBYTE buf1[] = "hello COM_task4";
	GetTaskID(&taskId);
    printf("**********************TaskHLS_syn running**********************\n");
  printf("TaskHLS_syn wait event: 0x40\n");
	WaitEvent(0x40);
	printf("TaskHLS_syn get event 0x40...\n");         
  showTaskInfo(taskId); 

 printf("activate COM_task4...\n");
	ActivateTask(COM_task4);
	
	//send message Msg4
	printf("TaskHLS_syn send Msg4: %s\n", buf1);
	SendMessage(Msg4, buf1);
	
	printf("**********************TaskHLS_syn end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskCOM_task1 ***************************
**************************************************
*/
TASK(TaskCOM_task1)
{ 
	TaskType taskId;
	StatusType status;
	OSBYTE buf[20];
	GetTaskID(&taskId);
    printf("**********************TaskCOM_task1 running**********************\n");
         
  showTaskInfo(taskId); 
  
  //TaskCOM_task1 tries to receive message Msg1
	status = ReceiveMessage(Msg1, buf);
	if(status == E_OK)
	{
	 	printf("TaskCOM_task1 receive message Msg1: %s\n", buf);
	} else
	{
		printf("TaskCOM_task1 receive message error: %d\n", status);
	}
	
	
	
	printf("**********************TaskCOM_task1 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskCOM_task2 ***************************
**************************************************
*/
TASK(TaskCOM_task2)
{ 
	TaskType taskId;
		StatusType status;
	OSBYTE buf[20];
	GetTaskID(&taskId);
    printf("**********************TaskCOM_task2 running**********************\n");
       
  showTaskInfo(taskId); 

  
  //TaskCOM_task2 tries to receive message Msg1
	status = ReceiveMessage(Msg2, buf);
	if(status == E_OK)
	{
	 	printf("TaskCOM_task2 receive message Msg2: %s\n", buf);
	} else
	{
		printf("TaskCOM_task2 receive message error: %d\n", status);
	}
	
	
	printf("**********************TaskCOM_task2 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskCOM_task3 ***************************
**************************************************
*/
TASK(TaskCOM_task3)
{ 
	TaskType taskId;
		StatusType status;
	OSBYTE buf[20];
	GetTaskID(&taskId);
    printf("**********************TaskCOM_task3 running**********************\n");
        
  showTaskInfo(taskId); 

  
  //TaskCOM_task3 tries to receive message Msg1
	status = ReceiveMessage(Msg3, buf);
	if(status == E_OK)
	{
	 	printf("TaskCOM_task3 receive message Msg3: %s\n", buf);
	} else
	{
		printf("TaskCOM_task3 receive message error: %d\n", status);
	}
	
	
	printf("**********************TaskCOM_task3 end**************************\n");
	TerminateTask();
}

/*************************************************
**************** TaskCOM_task4 ***************************
**************************************************
*/
TASK(TaskCOM_task4)
{ 
	TaskType taskId;
		StatusType status;
	OSBYTE buf[20];
	GetTaskID(&taskId);
    printf("**********************TaskCOM_task4 running**********************\n");
       
  showTaskInfo(taskId); 
           
  //TaskCOM_task4 tries to receive message Msg4
	status = ReceiveMessage(Msg4, buf);
	if(status == E_OK)
	{
	 	printf("TaskCOM_task4 receive message Msg4: %s\n", buf);
	} else
	{
		printf("TaskCOM_task4 receive message error: %d\n", status);
	}
	
	
	printf("**********************TaskCOM_task4 end**************************\n");
	TerminateTask();
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
