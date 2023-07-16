
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
	/*Task1	ID:0 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-0-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask1,		//task function		  
		Task1,								//task IDs
											//task initial attributes
		0 | OSEK_TASK_ACTIVE | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT,
		&taskStack[Task1][TASK_STACK_SIZE-1], //top of task stack
		&taskStack[Task1][0]				  //bottom of task stack
		
	},	
	/*Task2	ID:1 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-1-1 */		
	{
		(T_OSEK_TASK_Entry)FuncTask2,  
		Task2,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task2][TASK_STACK_SIZE-1],
		&taskStack[Task2][0]
	},			
	/*Task3	ID:2 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-2-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask3,  
		Task3,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task3][TASK_STACK_SIZE-1],
		&taskStack[Task3][0]
	},
	/*Task4	ID:3 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-3-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask4,  
		Task4,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE, 
		&taskStack[Task4][TASK_STACK_SIZE-1],
		&taskStack[Task4][0]
	},											
	/*Task5	ID:4 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-4-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask5,  
		Task5,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task5][TASK_STACK_SIZE-1],
		&taskStack[Task5][0]
	},
	/*Task6	ID:5 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-5-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask6,  
		Task6,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task6][TASK_STACK_SIZE-1],
		&taskStack[Task6][0]
	},
	/*Task7	ID:6 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-6-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask7,  
		Task7,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task7][TASK_STACK_SIZE-1],
		&taskStack[Task7][0]
	},			
		/*Task8	ID:7 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-7-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask8,  
		Task8,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task8][TASK_STACK_SIZE-1],
		&taskStack[Task8][0]
	},
		/*Task9	ID:8 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-8-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask9,  
		Task9,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task9][TASK_STACK_SIZE-1],
		&taskStack[Task9][0]
	},
		/*Task10	ID:9 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-9-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask10,  
		Task10,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task10][TASK_STACK_SIZE-1],
		&taskStack[Task10][0]
	},
		/*Task11	ID:10 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-10-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask11,  
		Task11,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task11][TASK_STACK_SIZE-1],
		&taskStack[Task11][0]
	},
		/*Task12	ID:11 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-11-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask12,  
		Task12,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task12][TASK_STACK_SIZE-1],
		&taskStack[Task12][0]
	},
		/*Task13	ID:12 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-12-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask13,  
		Task13,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task13][TASK_STACK_SIZE-1],
		&taskStack[Task13][0]
	},
		/*Task14	ID:13 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-6-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask14,  
		Task14,
		0 | OSEK_TASK_EXTENDED | OSEK_TASK_NONPREEMPT | OSEK_TASK_ACTIVE,
		&taskStack[Task14][TASK_STACK_SIZE-1],
		&taskStack[Task14][0]
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



//ALARM configuration table
T_OSEK_ALARM_ConfigTable osekConfig_AlarmTable[OCC_NALMS] = 
{
	{
		Task1,
#if (defined(OCC_ECC1) || defined(OCC_ECC2)) && defined(OCC_ALMSETEVENT)
		0,
#endif
		SysCounter,
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
   },
     {
   		4,						 //ID
   		&messageBuf[4],
   		sizeof(messageBuf[4]),
   		&messageQueue[4],
   		MESSAGE_BUF_SIZE
   },
  {
   		5,						 //ID
   		&messageBuf[5],
   		sizeof(messageBuf[5]),
   		&messageQueue[5],
   		MESSAGE_BUF_SIZE
   },
  {
   		6,						 //ID
   		&messageBuf[6],
   		sizeof(messageBuf[6]),
   		&messageQueue[6],
   		MESSAGE_BUF_SIZE
   }

};




