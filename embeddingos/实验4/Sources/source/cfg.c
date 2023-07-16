
#include "osprop.h"
#include "osapi.h"
#include "cfg.h"

#define TASK_STACK_SIZE		120 		//task stack size
#define	MESSAGE_BUF_SIZE	30			//message buffer size

OSBYTE	taskStack[CONFIG_OSEK_TASK_NUMBER][TASK_STACK_SIZE]; //task stacks
OSBYTE	messageBuf[OCC_NMSGS][MESSAGE_BUF_SIZE];			 //message buffers
OSBYTE	messageQueue[OCC_NMSGS][MESSAGE_BUF_SIZE];			 //message queues

#pragma LINK_INFO DERIVATIVE "mc9s12xep100"

#pragma CODE_SEG NON_BANKED
/*==================================================================================
|
| 					Interrupt Service Routines
|
===================================================================================*/



#ifndef Ir_RealTimInt

ISR(Ir_RealTimInt)
{
	 CounterTrigger(SysCounter);
}
#endif
#pragma CODE_SEG DEFAULT

StatusType MessageInit()
{

}


//interrupt type configurations
OSBYTE	osekConfig_InterruptTypeTable[OCC_ISRLEVELNUM] = 
{
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
};

//ISR functions
T_OSEK_TASK_Entry	osekConfig_InterruptEntryTable[OCC_ISRLEVELNUM] = 
{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		(T_OSEK_TASK_Entry)Ir_RealTimInt,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
};



/* task configurations */

const		T_OSEK_TASK_ConfigTable_Struct		osekConfig_TaskTable[OCC_NTSKS]=	
{
		/*ini_LLD	ID:0 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-0-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskini_LLD,  				//task function
		ini_LLD,										//task ID 
		0 | OSEK_TASK_ACTIVE | OSEK_TASK_NONPREEMPT | OSEK_TASK_EXTENDED,  //task initial attributes
		&taskStack[ini_LLD][TASK_STACK_SIZE-1],		//top of the task stack
		&taskStack[ini_LLD][0]						//bottom of the task stack
		
	},		
	/*HLS_ini	ID:1 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-1-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskHLS_ini,  
		HLS_ini,
		OSEK_TASK_NONPREEMPT | OSEK_TASK_EXTENDED,
		&taskStack[HLS_ini][TASK_STACK_SIZE-1],
		&taskStack[HLS_ini][0]
	},			
	/*HLS_inisyn	ID:2 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-2-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskHLS_inisyn,  
		HLS_inisyn,
		OSEK_TASK_NONPREEMPT | OSEK_TASK_EXTENDED,
		&taskStack[HLS_inisyn][TASK_STACK_SIZE-1],
		&taskStack[HLS_inisyn][0]
	},	
		/*HLS_ini2	ID:3 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-3-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskHLS_ini2,  
		HLS_ini2,
		OSEK_TASK_NONPREEMPT | OSEK_TASK_EXTENDED,
		&taskStack[HLS_ini2][TASK_STACK_SIZE-1],
		&taskStack[HLS_ini2][0]
	},
		/*ini_LLD2	ID:4 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-4-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskini_LLD2,  
		ini_LLD2,
		OSEK_TASK_NONPREEMPT | OSEK_TASK_EXTENDED,
		&taskStack[ini_LLD2][TASK_STACK_SIZE-1],
		&taskStack[ini_LLD2][0]
	},
			/*period50	ID:5 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-5-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskperiod50,  
		period50,
	 OSEK_TASK_EXTENDED,
		&taskStack[period50][TASK_STACK_SIZE-1],
		&taskStack[period50][0]
	},
			/*period100	ID:6 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-6-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskperiod100,  
		period100,
		OSEK_TASK_EXTENDED,
		&taskStack[period100][TASK_STACK_SIZE-1],
		&taskStack[period100][0]
	},
			/*period200	ID:7 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-7-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskperiod200,  
		period200,
		OSEK_TASK_EXTENDED,
		&taskStack[period200][TASK_STACK_SIZE-1],
		&taskStack[period200][0]
	},
			/*period1000	ID:8 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-8-1 */
	{
		(T_OSEK_TASK_Entry)FuncTaskperiod1000,  
		period1000,
		OSEK_TASK_EXTENDED,
		&taskStack[period1000][TASK_STACK_SIZE-1],
		&taskStack[period1000][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskHLS_syn2,  
		HLS_syn2,
		OSEK_TASK_EXTENDED | OSEK_TASK_ACTIVE,
		&taskStack[HLS_syn2][TASK_STACK_SIZE-1],
		&taskStack[HLS_syn2][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskHLS_rstsyn,  
		HLS_rstsyn,
		OSEK_TASK_EXTENDED | OSEK_TASK_ACTIVE,
		&taskStack[HLS_rstsyn][TASK_STACK_SIZE-1],
		&taskStack[HLS_rstsyn][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskHLS_firstsyn,  
		HLS_firstsyn,
			OSEK_TASK_EXTENDED | OSEK_TASK_ACTIVE,
		&taskStack[HLS_firstsyn][TASK_STACK_SIZE-1],
		&taskStack[HLS_firstsyn][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskHLS_syn,  
		HLS_syn,
		 	OSEK_TASK_EXTENDED | OSEK_TASK_ACTIVE,
		&taskStack[HLS_syn][TASK_STACK_SIZE-1],
		&taskStack[HLS_syn][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskCOM_task1,  
		COM_task1,
		 	OSEK_TASK_EXTENDED ,
		&taskStack[HLS_syn][TASK_STACK_SIZE-1],
		&taskStack[HLS_syn][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskCOM_task2,  
		COM_task2,
		 	OSEK_TASK_EXTENDED ,
		&taskStack[HLS_syn][TASK_STACK_SIZE-1],
		&taskStack[HLS_syn][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskCOM_task3,  
		COM_task3,
		 	OSEK_TASK_EXTENDED ,
		&taskStack[HLS_syn][TASK_STACK_SIZE-1],
		&taskStack[HLS_syn][0]
	},
		{
		(T_OSEK_TASK_Entry)FuncTaskCOM_task4,  
		COM_task4,
		 	OSEK_TASK_EXTENDED ,
		&taskStack[HLS_syn][TASK_STACK_SIZE-1],
		&taskStack[HLS_syn][0]
	},
	
	/*Task Idle */
	{
		0, 
		OSEK_TASK_IDLE_ID,
		OSEK_TASK_ACTIVE,
	},			
};



//COUNTER configuration table
T_OSEK_COUNTER_ConfigTable osekConfig_CounterTable[OCC_NCTRS] = 
{
	{
		32767,	// maximum value of the counter
		1,
		1
	},
};




T_OSEK_MESSAGE_ConfigTable_Struct osekConfig_MsgTable[OCC_NMSGS] = 
{
   {
   		0,						 //ID
   		&messageBuf[0],			 //message butter
   		sizeof(messageBuf[0]),	 //size of the message buffer
   		&messageQueue[0],		 //message queue
   		MESSAGE_BUF_SIZE		 //size of the queue
   },
   {
   		1,						 //ID
   		&messageBuf[1],
   		sizeof(messageBuf[1]),
   		&messageQueue[1],
   		MESSAGE_BUF_SIZE
   },
    {
   		2,						 //ID
   		&messageBuf[2],
   		sizeof(messageBuf[2]),
   		&messageQueue[2],
   		MESSAGE_BUF_SIZE
   },
   {
   		3,						 //ID
   		&messageBuf[3],
   		sizeof(messageBuf[3]),
   		&messageQueue[3],
   		MESSAGE_BUF_SIZE
   }
};

//ALARM configuration
T_OSEK_ALARM_ConfigTable osekConfig_AlarmTable[ ] = 
{
	{
		period50,	   	//the task notified by the Alarm		
		0x00,		//the event to be set for the task 如果是0 代表就是激活事件 
		SysCounter, //name of the counter that the alarm uses
	},
		{
		period100,	   	//the task notified by the Alarm
					
		0x00,		//the event to be set for the task 如果是0 代表就是激活事件 

		SysCounter, //name of the counter that the alarm uses
	},
		{
		period200,	   	//the task notified by the Alarm
					
		0x00,		//the event to be set for the task 如果是0 代表就是激活事件 

		SysCounter, //name of the counter that the alarm uses
	},
		{
		period1000,	   	//the task notified by the Alarm
					
		0x00,		//the event to be set for the task 如果是0 代表就是激活事件 

		SysCounter, //name of the counter that the alarm uses
	},
};



