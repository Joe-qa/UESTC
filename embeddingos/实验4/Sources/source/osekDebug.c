/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekDebug.c,v $
 * Revision 1.0  2008/04/10 15:40:00
 * 创建文件。
 */

/*
 * @file 	osekDebug.c
 * @brief
 *	<li>功能：实现调试相关功能。</li>
 * @author
 * @date 	2008-04-10
 * <p>部门：
 */


/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"

/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

/**************************** 实现部分 *********************************/

#if defined(CONFIG_OSEK_TASK_STACK_CHECK)

#if defined(CONFIG_OSEK_SYSTEM_ET)
/*
 * @brief
 *    GetUsage：获取堆栈的使用情况（内部函数）。
 * @param
 * @param[in]	startPtr：起始位置指针。
 *          	endPtr：结束位置指针。
 * @param[out]  无。
 * @returns:    堆栈已使用的字节数。
 */
static OSDWORD GetUsage( OSBYTEPTR startPtr , OSBYTEPTR endPtr)
{
	// 获取特定字符模式的起始位置
	while( (startPtr != endPtr) && (*startPtr == (OSBYTE)OSEK_DEBUG_STACKPATTERN))
	{
		startPtr++;
	}

	// 计算并返回堆栈已使用的字节数量
	return ((OSDWORD)( endPtr - startPtr ));
}
#endif  /* defined(CONFIG_OSEK_SYSTEM_ET) */


/*
 * @brief
 *    GetRunningStackUsage：获取运行任务的堆栈使用情况。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    OSEK_DEBUG_STACKNOASSIGNED: 指定任务没有堆栈。
 * <p>          栈已使用的字节数。
 */
OSDWORD GetRunningStackUsage( void )
{

#if defined(CONFIG_OSEK_SYSTEM_ET)
	// 如果运行任务是扩展任务，则调用内部函数GetUsage获取堆栈的使用情况
	if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_EXTENDED ) != 0 )
	{
		return GetUsage(osekTask_RunningTask->taskControlBlock->configTable.stackBottom,osekTask_RunningTask->taskControlBlock->configTable.stackTop);
	}
#endif

	// 当前运行任务不是扩展任务，没有单独的堆栈，返回OSEK_DEBUG_STACKNOASSIGNED
	return OSEK_DEBUG_STACKNOASSIGNED ;

}


/*
 * @brief
 *    GetStackUsage：获取栈的使用情况。
 * @param
 * @param[in]	taskId：任务的ID。
 * @param[out]  无。
 * @returns:    OSEK_DEBUG_STACKNOASSIGNED: 指定任务没有堆栈。
 * <p>          栈已使用的字节数。
 */
OSDWORD GetStackUsage( TaskType taskId )
{
#if defined(CONFIG_OSEK_SYSTEM_ET)
	T_OSEK_TASK_ControlBlock *tcbPtr;
#endif
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果任务ID超出范围，返回OSEK_DEBUG_STACKNOASSIGNED
	if( taskId > CONFIG_OSEK_TASK_MAX_ID )
	{
		return OSEK_DEBUG_STACKNOASSIGNED;
	}
#endif

#if defined(CONFIG_OSEK_SYSTEM_ET)
	
	tcbPtr = &osekTask_TaskTable[taskId];

	// 如果指定任务是扩展任务，则调用内部函数GetUsage获取堆栈的使用情况
	if( (tcbPtr->configTable.property & OSEK_TASK_EXTENDED ) != 0 )
	{
		return GetUsage(tcbPtr->configTable.stackBottom,tcbPtr->configTable.stackTop);
	}
#endif

	// 指定任务不是扩展任务，没有单独的堆栈，返回OSEK_DEBUG_STACKNOASSIGNED
	return OSEK_DEBUG_STACKNOASSIGNED ;

}

#endif /* #if defined(CONFIG_OSEK_TASK_STACK_CHECK)  */


/*
 * @brief
 *    GetTimeStamp：获取时间戳。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    时间戳的值。
 */
OSDWORD GetTimeStamp( void )
{
	unsigned int value;


	// 读取硬件定时器的计数值
	OSEK_Time_ReadCount(&value);

	return value;

}

