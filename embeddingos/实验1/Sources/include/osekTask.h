/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekTask.h,v $
 * Revision 1.0  2008/03/05 9:12:00
 * 创建文件
*/

/**
 * @file 	osekTask.h
 * @brief
 *	<li> 功能：定义任务相关的数据结构、宏，以及任务相关的申明。 </li>
 * @author
 * @date 	2008-03-05
 * <p>部门：
 */

/**
 * \defgroup  Task 任务
 * 任务管理模块,主要实现任务的管理
 */
/**@{*/
/** @name 任务管理模块*/
//@{


#ifndef _OSEK_TASK_H
#define _OSEK_TASK_H

/**************************** 引用部分 *********************************/


#ifdef __cplusplus
extern "C" {
#endif


/**************************** 声明部分 *********************************/

//  任务管理提供给外部的公共调用函数

/**
 * @brief
 *    ActivateTask：激活指定的任务。
 * @param[in]	taskId：任务的ID号。
 * @returns		E_OK：成功激活
 * @returns    	E_OS_LIMIT：任务状态不对或者超过了最大激活次数
 * @returns     [E_OS_ID：无效的任务ID]
 */
StatusType ActivateTask( TaskType taskId );

/**
 * @brief
 *    TerminateTask：结束当前任务, 正确情况下是不会返回的。
 * @param[in]	无
 * @returns		[E_OS_RESOURCE：还有资源没有释放；]
 * @returns		[E_OS_CALLEVEL：在中断中调用了该功能。]
 *
 */
StatusType TerminateTask( void );

/**
 * @brief
 *    ChainTask：结束当前任务并且激活指定的任务。
 * @param[in]	taskId：需要激活的任务ID号
 * @returns		E_OS_LIMIT：需要激活的任务状态不对，或者超过了最大激活次数。
 * @returns     [E_OS_RESOURCE：还有资源没有释放；]
 * @returns		[E_OS_CALLEVEL：在中断中调用此函数；]
 * @returns		[E_OS_ID：需要激活的任务ID号不合法；]
 *
 */
StatusType ChainTask( TaskType taskId );

/**
 * @brief
 *    Schedule：重调度，主要用于任务主动放弃CPU允许其它高优先级任务运行。
 * @param[in]	无。
 * @returns		E_OK：成功。
 * @returns     [E_OS_RESOURCE: 还有资源（包括调度锁资源和中断资源）没有释放；]
 * @returns     [E_OS_CALLEVEL: 在中断中调用的此函数；]
 *
 */
StatusType Schedule( void );

/**
 * @brief
 *    GetTaskID：获取当前任务的ID号。
 * @param[in]	taskId：保存任务ID的变量的引用
 * @returns		E_OK: 成功获取了任务状态。
 *
 */
StatusType GetTaskID( TaskRefType taskId );

/**
 * @brief
 *    GetTaskState：获取指定任务的状态，状态包括RUNNING、WAITING、READY、SUSPENDED等。
 * @param[in]	taskId：任务ID号；
 * @param[in]	state：任务状态变量的引用，用于保存输出的任务状态。
 * @returns 	E_OK: 成功获取了任务状态；
 * @returns     [E_OS_ID：任务ID号无效。]
 *
 */
StatusType GetTaskState ( TaskType taskId, TaskStateRefType state );

// 内部函数
void osekTask_Dispatch( void );
void osekTask_TerminateDispatch( void );
void osekTask_WaitTask( void );
void osekTask_ChangeRunningTaskPriority( OSPRIOTYPE newPri );
void osekTask_Initialize( void );


/**************************** 定义部分 *********************************/

#define OSEK_TASK_CallTaskEntry()  (*(osekTask_RunningTask->taskControlBlock->configTable.entry))()

#define OSEK_TASK_INVALID_ID CONFIG_OSEK_TASK_NUMBER	// 无效的任务ID
#define OSEK_TASK_ID2PRI(id) (CONFIG_OSEK_TASK_PRIORITY_NUMBER - (id) - 1)	// 从任务ID转换为优先级的宏，当系统为BCC1或ECC1时需要这样计算任务的优先级


/// 任务属性：在系统启动时激活任务
#define OSEK_TASK_ACTIVE     ( (OSBYTE)0x08 )
/// 任务属性：非抢占任务
#define OSEK_TASK_NONPREEMPT ( (OSBYTE)0x40 )
/// 任务属性：扩展任务
#define OSEK_TASK_EXTENDED   ( (OSBYTE)0x80 )


/// 任务状态：运行状态
#define RUNNING              ((TaskStateType)0)
/// 任务状态：等待状态
#define WAITING              ((TaskStateType)1)
/// 任务状态：就绪状态
#define READY                ((TaskStateType)2)
/// 任务状态：挂起状态
#define SUSPENDED            ((TaskStateType)3)

#define OSEK_TASK_FIRST_RUN ((TaskStateType)4)      // 任务第一次运行

#define OSEK_TASK_STATE_MASK (((TaskStateType)3))   // 任务状态掩码

#define OSEK_TASK_GETTOPOFSTACK(addr, size)  ((addr) + ((size)-0x20))	// 获取栈顶位置

#define OSEK_TASK_NEED_DISPATCH      ((StatusType)0xFF)	// 需要执行调度


/// 任务的配置结构
typedef struct T_OSEK_TASK_ConfigTable_Struct
{

	/// 任务入口
	T_OSEK_TASK_Entry entry;
	/// 任务的ID(0-255)，在BCC1和ECC1时，系统最多支持64个任务，并且任务的优先级与ID号相关
	TaskType taskId;

// 应用模式大于1，表示系统中有多个模式
#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
	/// 是否要系统自动启动任务，每个位表示一个模式，如果需要在该模式启动任务，则将该变量的对应位置位（该属性在应用模式大于1时才有效）
	OSDWORD autoStart;
#endif

	/// 配置任务的属性：是否启动时激活任务、是否是扩展任务、是否是非抢占任务
	OSBYTE property;

// 扩展任务的栈配置
#if defined(CONFIG_OSEK_SYSTEM_ET)
	/// 扩展任务的栈顶（在配置了扩展任务时该属性才存在）
	OSBYTEPTR stackTop;
	/// 扩展任务的栈底（在配置了扩展任务时该属性才存在）
	OSBYTEPTR stackBottom;
#endif

// 当使用内部资源时
#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
	/// 使用内部资源时任务的优先级（在配置了内部资源时该属性才存在）
	OSPRIOTYPE inresPriority;
#endif

// 允许一个优先级上有多个任务时
#if defined(CONFIG_OSEK_SYSTEM_EP)
	/// 任务的优先级（允许一个优先级上有多个任务时该属性才存在）
	OSPRIOTYPE basePriority;
	/// 基本任务的最大激活次数（允许一个优先级上有多个任务时该属性才存在）
	OSBYTE maxActive;
#endif

}T_OSEK_TASK_ConfigTable_Struct;


/// 任务的控制结构
typedef struct T_OSEK_TASK_ControlBlock_Struct
{

	/// 任务状态信息，其中0-1位表示基本状态（0－运行，1－等待，2－就绪，3－挂起），位2表示任务是第一次运行
	TaskStateType status;

// 允许一个优先级上有多个任务时
#if defined(CONFIG_OSEK_SYSTEM_EP)
	/// 任务的当前激活次数（允许一个优先级上有多个任务时该属性才存在）
	OSBYTE curActiveNum;
#endif

	/// 任务的当前优先级；
	OSPRIOTYPE curPriority;
    /// 任务的配置表
    T_OSEK_TASK_ConfigTable configTable;

// 当使用扩展任务时
#if defined(CONFIG_OSEK_SYSTEM_ET)
	/// 任务要等待的事件，最多32个事件（当配置了扩展任务时该属性才存在）
	EventMaskType waitEvent;
	/// 任务被设置的事件，最多32个事件（当配置了扩展任务时该属性才存在）
	EventMaskType setEvent;
#endif

	/// 任务上下文，如果系统中只有基本任务是没有任务上下文的
	T_OSEK_TARGET_TaskContext      *context;

// 当使用了资源
#if defined(CONFIG_OSEK_RESOURCE)
	/// 任务使用的资源的链表（当配置了资源时该属性才存在）
	ResourceType resourceList;
#endif

}T_OSEK_TASK_ControlBlock_Struct;

// 用于在就绪队列中排队的就绪块结构
typedef struct T_OSEK_TASK_ReadyBlock_Struct
{
	struct T_OSEK_TASK_ReadyBlock_Struct   *nextTask;			// 下一个就绪的任务节点
	T_OSEK_TASK_ControlBlock_Struct *taskControlBlock; // 指向对应任务控制块的指针
}T_OSEK_TASK_ReadyBlock_Struct;

// 就绪任务队列表结构
typedef struct
{

	T_OSEK_TASK_ReadyBlock_Struct *readyTaskHead; // 就绪任务队列的头指针
	T_OSEK_TASK_ReadyBlock_Struct *readyTaskTail; // 就绪任务队列的尾指针

}T_OSEK_TASK_ReadyTaskTableItem;


// 就绪任务队列声明
extern T_OSEK_TASK_ReadyTaskTableItem osekTask_ReadyTaskTable[];

// 任务控制块表声明
extern T_OSEK_TASK_ControlBlock_Struct osekTask_TaskTable[];

/// 任务配置信息表声明
extern const T_OSEK_TASK_ConfigTable_Struct osekConfig_TaskTable[];

// 当前运行任务指针声明
extern T_OSEK_TASK_ReadyBlock_Struct *osekTask_RunningTask;

// 任务就绪块表声明
extern T_OSEK_TASK_ReadyBlock_Struct osekTask_ReadyBlock[];
// 系统空闲就绪块链声明
extern T_OSEK_TASK_ReadyBlock_Struct *osekTask_FreeReadyBlock;

// 中断服务程序中调试标志
extern OSDWORD osekTask_IsrTriggeredTaskSwitchNecessary;


// 优先级位图
// 硬件实现的优先级位图
#if defined(CONFIG_OSEK_TARGET_HARDBITMAP)
extern OSDWORD osekTask_PriorityBitMap1;

// 硬件支持扩展优先级
#if defined(CONFIG_OSEK_TASK_EXTPRIORS)
extern OSDWORD osekTask_PriorityBitMap2;

// 设置优先级位图中指定优先级的就绪标志
#define OSEK_TASK_SetPriorityBitMap(pri)  \
( (pri < 32) ? \
(osekTask_PriorityBitMap1 |= ( ((OSDWORD)0x00000001)<< (pri) )) :  \
(osekTask_PriorityBitMap2 |= ( ((OSDWORD)0x00000001)<< ((pri)-32) )) )

// 清除优先级位图中指定优先级的就绪标志
#define OSEK_TASK_ClearPriorityMap(pri) \
{ \
	if( osekTask_ReadyTaskTable[(pri)].readyTaskHead == NULLPTR ) \
	{ \
		 (pri < 32) ? \
		(osekTask_PriorityBitMap1 &= ~( ((OSDWORD)0x00000001)<< (pri) )) :  \
		(osekTask_PriorityBitMap2 &= ~( ((OSDWORD)0x00000001)<< ((pri)-32) )); \
	} \
}

// 获取最高优先级
#if defined(CONFIG_OSEK_TARGET_PPC)
static inline  OSDWORD GetHighestPriority( OSDWORD scheduler_vector1, OSDWORD scheduler_vector2 )
{
    OSDWORD result;
    asm volatile("cntlzw %0,%1":"=r"((result)):"r"((scheduler_vector2)));

    if( result < 32 )
    {
    	result =  63 - result;
    }
    else
    {
    	asm volatile("cntlzw %0,%1":"=r"((result)):"r"((scheduler_vector1)));
    	result = 31 - result;

    }

    return result;
}
#endif


#define OSEK_TASK_GetHighestPriority() GetHighestPriority( osekTask_PriorityBitMap1, osekTask_PriorityBitMap2 )


// 硬件不支持扩展优先级
#else /* #if defined(CONFIG_OSEK_TASK_EXTPRIORS) */

// 设置优先级位图中指定优先级的就绪标志
#define OSEK_TASK_SetPriorityBitMap(pri) osekTask_PriorityBitMap1 |= ( ((OSDWORD)0x00000001)<< (pri) )

// 清除优先级位图中指定优先级的就绪标志
#define OSEK_TASK_ClearPriorityMap(pri)  \
{ \
	if( osekTask_ReadyTaskTable[(pri)].readyTaskHead == NULLPTR ) \
	{ \
		osekTask_PriorityBitMap1 &= ~( ((OSDWORD)0x00000001)<< (pri) ); \
	} \
}

// 获取最高优先级
#if defined(CONFIG_OSEK_TARGET_PPC)
static inline OSDWORD GetHighestPriority( OSDWORD scheduler_vector )
{
    OSDWORD result;
    asm volatile("cntlzw %0,%1":"=r"((result)):"r"((scheduler_vector)));
    return ( 31 - result);
}
#endif


#define OSEK_TASK_GetHighestPriority() GetHighestPriority(osekTask_PriorityBitMap1)

#endif /* else defined(CONFIG_OSEK_TASK_EXTPRIORS) */

#endif /* #if defined(CONFIG_OSEK_TARGET_HARDBITMAP)  */


// 调度锁计数变量声明
extern OSDWORD osekTask_SchedulerLockLevel;


// 软件实现的优先级位图
#if defined(CONFIG_OSEK_TARGET_SOFTBITMAP)
// 优先级位图数组
extern OSBYTE osekTask_PriorityBitMap[( (OSBYTE) (CONFIG_OSEK_TASK_PRIORITY_NUMBER) ) >> 3];
// 主优先级位图变量
extern OSBYTE osekTask_PriorityBitMapMajor;
// 优先级索引表
extern OSBYTE osekTask_PriorityIndexTable[256];

// 设置优先级位图中指定优先级的就绪标志
#define OSEK_TASK_SetPriorityBitMap(pri)  do {  \
	osekTask_PriorityBitMap[(pri)>>3] |= (1<<((pri)&7)) ; \
	osekTask_PriorityBitMapMajor |= (1 << ((pri)>>3)); \
}while(0)


// 清除优先级位图中指定优先级的就绪标志
#pragma INLINE
static  void ClearPriorityMap( OSPRIOTYPE pri)
{
	if( osekTask_ReadyTaskTable[(pri)].readyTaskHead == NULLPTR )
	{
		OSPRIOTYPE index = ((pri)>>3) ;
		osekTask_PriorityBitMap[index] &= (~(1<<((pri)&7))) ;
		if( osekTask_PriorityBitMap[index] == 0 )
		{
			osekTask_PriorityBitMapMajor &= (~(1 << index)) ;
		}
	}
}

#define OSEK_TASK_ClearPriorityMap(pri)  (ClearPriorityMap(pri))


// 获取最高优先级
#define OSEK_TASK_GetHighestPriority() \
	((osekTask_PriorityIndexTable[osekTask_PriorityBitMapMajor] << 3 )| \
	(osekTask_PriorityIndexTable[osekTask_PriorityBitMap[osekTask_PriorityIndexTable[osekTask_PriorityBitMapMajor]]]))


#endif /* #if defined(CONFIG_OSEK_TARGET_SOFTBITMAP) */


// 将指定任务放入就绪表的尾部
#pragma INLINE
static  void AddTask2ReadyTableAtTail( T_OSEK_TASK_ReadyBlock *readyblockptr)
{
	T_OSEK_TASK_ReadyTaskTableItem *chain;

	// 当任务挂到队列尾部时，是以其原始优先级进入就绪队列的
#if defined(CONFIG_OSEK_SYSTEM_EP)
	chain = &osekTask_ReadyTaskTable[readyblockptr->taskControlBlock->configTable.basePriority];
#else
	chain = &osekTask_ReadyTaskTable[OSEK_TASK_ID2PRI(readyblockptr->taskControlBlock->configTable.taskId)];
#endif

	(readyblockptr)->nextTask = NULLPTR ;
	if( chain->readyTaskHead == NULLPTR )
	{
		chain->readyTaskHead = (readyblockptr);
	}
	else
	{
		chain->readyTaskTail->nextTask = (readyblockptr);
	}
	chain->readyTaskTail = (readyblockptr);

}

#define OSEK_TASK_AddTask2ReadyTableAtTail(readyblockptr)  (AddTask2ReadyTableAtTail(readyblockptr))



// 将指定任务放入就绪表的头部
static  void AddTask2ReadyTableAtHead( T_OSEK_TASK_ReadyBlock *readyblockptr)
{
	T_OSEK_TASK_ReadyTaskTableItem *chain;

	// 当任务挂到队列头部时，以当前优先级进入
	chain = &osekTask_ReadyTaskTable[readyblockptr->taskControlBlock->curPriority];
	(readyblockptr)->nextTask = chain->readyTaskHead ;
	if( chain->readyTaskTail == NULLPTR )
	{
		chain->readyTaskTail = (readyblockptr);
	}
	chain->readyTaskHead = (readyblockptr);

}

#define OSEK_TASK_AddTask2ReadyTableAtHead(readyblockptr)  (AddTask2ReadyTableAtHead(readyblockptr))

// 在就绪队列中加入一个新的就绪块
static  StatusType AddaReadyBlock( T_OSEK_TASK_ControlBlock *tcbPtr )
{
	T_OSEK_TASK_ReadyBlock *readyBlock;

	if(osekTask_FreeReadyBlock!=NULLPTR)
	{
		readyBlock = osekTask_FreeReadyBlock;
		osekTask_FreeReadyBlock = osekTask_FreeReadyBlock->nextTask;

		readyBlock->taskControlBlock = tcbPtr;

		// 当任务新就绪时，是以其原始优先级进入就绪队列的
#if defined(CONFIG_OSEK_SYSTEM_EP)
		OSEK_TASK_SetPriorityBitMap(tcbPtr->configTable.basePriority);
#else
		OSEK_TASK_SetPriorityBitMap(OSEK_TASK_ID2PRI(tcbPtr->configTable.taskId));
#endif
		OSEK_TASK_AddTask2ReadyTableAtTail(readyBlock);

		return E_OK;
	}
	else
	{
		return E_OS_LIMIT;//不是单个任务激活数超标，而是系统没有空闲readyBlock了。通过配置工具保证，理论上这种情况不应出现。
	}

}

#define OSEK_TASK_AddaReadyBlock(tcbPtr)  (AddaReadyBlock(tcbPtr))

// 释放一个就绪块回空闲链
static  void ReleaseReadyBlock( T_OSEK_TASK_ReadyBlock *readyBlock )
{

	readyBlock->nextTask = osekTask_FreeReadyBlock;
	osekTask_FreeReadyBlock = readyBlock;

}

#define OSEK_TASK_ReleaseReadyBlock(readyBlock)  (ReleaseReadyBlock(readyBlock))

// 从就绪表中移出指定优先级的一个任务
static  void MoveTaskFromReadyTable(OSPRIOTYPE pri)
{
	T_OSEK_TASK_ReadyTaskTableItem *chain;

	chain = &osekTask_ReadyTaskTable[(pri)];
	chain->readyTaskHead = chain->readyTaskHead->nextTask;  // 假定调用者已保证了chain->readyTaskHead的有效性
	if(chain->readyTaskHead == NULLPTR )
	{
		chain->readyTaskTail = NULLPTR;
	}
}
#pragma NO_INLINE
#define OSEK_TASK_MoveTaskFromReadyTable(pri)  (MoveTaskFromReadyTable(pri))

// 获取指定优先级上的第一个就绪任务
#define OSEK_TASK_GetReadyTask(pri) (osekTask_ReadyTaskTable[(pri)].readyTaskHead)


// 内部函数（以任务控制块指针作为输入参数）
StatusType osekTask_ActiveTask(T_OSEK_TASK_ControlBlock *tcbPtr);
StatusType osekTask_WakeTask(T_OSEK_TASK_ControlBlock *tcbPtr);

#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
//@}
/** @}*/
