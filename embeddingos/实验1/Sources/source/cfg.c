
/************************* includings ***********************************/

#include "osprop.h"
#include "osapi.h"
#include "cfg.h"
/************************* declarations ***********************************/

#define TASK_STACK_SIZE		100	 //task stack size

OSBYTE	taskStack[CONFIG_OSEK_TASK_NUMBER][TASK_STACK_SIZE]; //task stack

/************************* definitions ***********************************/
#pragma LINK_INFO DERIVATIVE "mc9s12xep100"

#pragma CODE_SEG NON_BANKED
/*==================================================================================
|
| 					Interrupt Service Routines
|
===================================================================================*/
/***********************************************************************************
|name             :    TimingInt
|parameters       :    none
|return value     :    none
|describe         :    TimingInt interrupt service routine level 0
************************************************************************************/



#ifndef Ir_RealTimInt

ISR(Ir_RealTimInt)
{
	 CounterTrigger(SysCounter);
}
#endif
#pragma CODE_SEG DEFAULT



/* interrupt objects */
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

//ISR entry configuration
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



/* task objects */
//stack space of extended tasks

const		T_OSEK_TASK_ConfigTable_Struct		osekConfig_TaskTable[OCC_NTSKS]=	
{
	/*Task1	ID:0 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-0-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask1,			//task entry  
		Task1,									//task ID
		OSEK_TASK_ACTIVE   ,					//task initial attributes
		&taskStack[Task1][TASK_STACK_SIZE-1],   //top of task stack
		&taskStack[Task1][0]					//bottom of task stack
				
	},
	/*Task2	ID:1 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-1-1 */			
	{
		(T_OSEK_TASK_Entry)FuncTask2,  
		Task2,
	  OSEK_TASK_ACTIVE,
		&taskStack[Task2][TASK_STACK_SIZE-1],
		&taskStack[Task2][0]
	},
	/*Task3	ID:2 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-2-1 */			
	{
		(T_OSEK_TASK_Entry)FuncTask3,  
		Task3,
		OSEK_TASK_ACTIVE,
		&taskStack[Task3][TASK_STACK_SIZE-1],
		&taskStack[Task3][0]
	},
	/*Task4	ID:3 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-3-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask4,  
		Task4,
		SUSPENDED,
		&taskStack[Task4][TASK_STACK_SIZE-1],
		&taskStack[Task4][0]
	},	
	/*Task5	ID:4 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-4-1 */										
	{
		(T_OSEK_TASK_Entry)FuncTask5,  
		Task5,
		SUSPENDED,
		&taskStack[Task5][TASK_STACK_SIZE-1],
		&taskStack[Task5][0]
	},
	/*Task6	ID:5 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-5-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask6,  
		Task6,
		SUSPENDED,
		&taskStack[Task6][TASK_STACK_SIZE-1],
		&taskStack[Task6][0]
	},
	/*Task7	ID:6 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-6-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask7,  
		Task7,
		SUSPENDED,
		&taskStack[Task7][TASK_STACK_SIZE-1],
		&taskStack[Task7][0]
	},
		/*Task8	ID:7 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-7-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask8,  
		Task8,
		SUSPENDED,
		&taskStack[Task8][TASK_STACK_SIZE-1],
		&taskStack[Task8][0]
	},
		/*Task9	ID:8 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-8-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask9,  
		Task9,
		SUSPENDED,
		&taskStack[Task9][TASK_STACK_SIZE-1],
		&taskStack[Task9][0]
	},
		/*Task10	ID:9 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-9-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask10,  
		Task10,
		OSEK_TASK_ACTIVE,
		&taskStack[Task10][TASK_STACK_SIZE-1],
		&taskStack[Task10][0]
	},
		/*Task11	ID:10 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-10-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask11,  
		Task11,
		OSEK_TASK_ACTIVE ,
		&taskStack[Task11][TASK_STACK_SIZE-1],
		&taskStack[Task11][0]
	},
		/*Task12	ID:11 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-11-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask12,  
		Task12,
		OSEK_TASK_ACTIVE,
		&taskStack[Task12][TASK_STACK_SIZE-1],
		&taskStack[Task12][0]
	},
	/*Task13	ID:12 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-12-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask13,  
		Task13,
		SUSPENDED,
		&taskStack[Task13][TASK_STACK_SIZE-1],
		&taskStack[Task13][0]
	},
		/*Task14	ID:13 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-13-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask14,  
		Task14,
		SUSPENDED,
		&taskStack[Task14][TASK_STACK_SIZE-1],
		&taskStack[Task14][0]
	},
		/*Task15	ID:14 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-14-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask15,  
		Task15,
		SUSPENDED,
		&taskStack[Task15][TASK_STACK_SIZE-1],
		&taskStack[Task15][0]
	},
		/*Task16	ID:15 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-15-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask16,  
		Task16,
		OSEK_TASK_ACTIVE,
		&taskStack[Task16][TASK_STACK_SIZE-1],
		&taskStack[Task16][0]
	},
		/*Task17	ID:16 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-16-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask17,  
		Task17,
		OSEK_TASK_ACTIVE,
		&taskStack[Task17][TASK_STACK_SIZE-1],
		&taskStack[Task17][0]
	},
		/*Task18	ID:17 priority:CONFIG_OSEK_TASK_PRIORITY_NUMBER-17-1 */
	{
		(T_OSEK_TASK_Entry)FuncTask18,  
		Task18,
		OSEK_TASK_ACTIVE,
		&taskStack[Task18][TASK_STACK_SIZE-1],
		&taskStack[Task18][0]
	},	
	/*Task Idle	*/		
	{
		0, 
		OSEK_TASK_IDLE_ID,
		OSEK_TASK_ACTIVE,
	},		
};



/* counters */
//COUNTER configuration table
T_OSEK_COUNTER_ConfigTable osekConfig_CounterTable[OCC_NCTRS] = 
{
	{
		32767,	// maximum value of the counter
		1,
		1
	},
};



/* alarms */
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




