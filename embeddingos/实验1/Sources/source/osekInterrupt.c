/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekInterrupt.c,v $
 * Revision 1.0  2008/03/21 09:41:00
 * 创建文件。
 */

/*
 * @file 	osekInterrupt.c
 * @brief
 *	<li>功能：实现抽象中断管理的相关功能。</li>
 * @author
 * @date 	2008-03-21
 * <p>部门：
 */


/**************************** 引用部分 *********************************/

#include "osprop.h"
#include "osekTarget.h"
#include "osapi.h"

/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

// 全局变量

// 中断服务程序中调试标志
//OSDWORD osekTask_IsrTriggeredTaskSwitchNecessary;

// 中断嵌套深度
OSDWORD osekInterrupt_NestedLevl;

// 全局中断开关嵌套深度
static OSDWORD osekInterrupt_NestedAllInt;

// ISR2类中断嵌套深度
static OSDWORD osekInterrupt_NestedOsInt;

// 中断栈栈顶
OSWORD osekInterrupt_IsrStackTop;

// ISR2类中断堆栈空间
static OSBYTE isr2Stack[CONFIG_OSEK_INTERRUPT_STACK_SIZE];


/**************************** 实现部分 *********************************/

/*
 * @brief
 *    osekInterrupt_Initialize：中断初始化（内部函数）。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void osekInterrupt_Initialize(void)
{
	osekInterrupt_IsrStackTop = (OSDWORD)isr2Stack + (OSDWORD)(CONFIG_OSEK_INTERRUPT_STACK_SIZE - 0x20);
}


/*
 * @brief
 *    DisableAllInterrupts：禁止所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void DisableAllInterrupts( void )
{
	OSEK_TARGET_DisableAllInt();
}


/*
 * @brief
 *    EnableAllInterrupts：使能所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void EnableAllInterrupts( void )
{
	OSEK_TARGET_EnableAllInt();
}


/*
 * @brief
 *    SuspendAllInterrupts：挂起所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void SuspendAllInterrupts( void )
{

	if( osekInterrupt_NestedAllInt == 0 )
	{
		OSEK_TARGET_DisableNestedAllInt();
	}

	osekInterrupt_NestedAllInt++;
}


/*
 * @brief
 *    ResumeAllInterrupts：恢复所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void ResumeAllInterrupts( void )
{
	// 如果禁止中断层数为0，直接返回
	if( osekInterrupt_NestedAllInt == 0)
	{
		return;
	}

	// 禁止中断层数减1，减到0时使能所有中断
	osekInterrupt_NestedAllInt--;
	if( osekInterrupt_NestedAllInt == 0 )
	{
		OSEK_TARGET_EnableNestedAllInt();
	}
}


/*
 * @brief
 *    SuspendOSInterrupts：挂起OS相关的中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void SuspendOSInterrupts( void )
{
	if( osekInterrupt_NestedOsInt == 0 )
	{
		OSEK_TARGET_DisableNestedOsInt();
	}

	osekInterrupt_NestedOsInt++;
}


/*
 * @brief
 *    ResumeOSInterrupts：恢复OS相关的中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void ResumeOSInterrupts( void )
{
	// 如果禁止中断层数为0，直接返回
	if( osekInterrupt_NestedOsInt == 0 )
	{
		return;
	}

	// 禁止中断层数减1，减到0时使能OS相关的中断
	osekInterrupt_NestedOsInt--;

	if( osekInterrupt_NestedOsInt == 0 )
	{
		OSEK_TARGET_EnableNestedOsInt();
	}
}


