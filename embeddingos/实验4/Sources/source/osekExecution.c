/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekExecution.c,v $
 * Revision 1.0  2008/03/12 13:37:00
 * 创建文件。
 */

/*
 * @file 	osekExecution.c
 * @brief
 *	<li>功能：实现系统的启动和关闭等功能。</li>
 * @author
 * @date 	2008-03-12
 * <p>部门：
 */



/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"
#include <string.h>


/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

// 全局变量
OSBYTE osekExecution_CurrentAppMode;


/**************************** 实现部分 *********************************/

/*
 * @brief
 *    StartOS：启动操作系统。
 * @param
 * @param[in]	mode：应用模式。
 * @param[out]  无。
 * @returns:    无。
 */

void StartOS( AppModeType mode )
{

	// 关闭中断
	OSEK_TARGET_DisableOSIntWithoutSave() ;

	//初始化应用模式；
	osekExecution_CurrentAppMode = mode;

    //初始化任务管理
	osekTask_Initialize();

	// 初始化中断管理
#if (!defined(CONFIG_OSEK_INTERRUPT_ISR_NULL))
    osekInterrupt_Initialize();
#endif

    // 初始化Counter管理
#if defined(CONFIG_OSEK_COUNTER)
    osekCounter_Initialize();
#endif

    // 初始化Alarm管理
#if defined(CONFIG_OSEK_ALARM)
    osekAlarm_Initialize();
#endif

	//初始化资源管理
#if defined(CONFIG_OSEK_RESOURCE)
    osekResource_Initialize();
#endif


	// 准备进入多任务运行

	// 将系统的第一个任务安排为IDLE任务
#if defined(CONFIG_OSEK_SYSTEM_EP)
	osekTask_RunningTask = OSEK_TASK_GetReadyTask(0);
#else
	osekTask_RunningTask = OSEK_TASK_GetReadyTask(OSEK_TASK_ID2PRI(OSEK_TASK_IDLE_ID));
#endif

    // 初始化通信管理
#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)
#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0
	InitCOM();
#endif
#endif

	// 调用系统启动HOOK函数
	OSEK_HOOK_StartupHook();

	// 进行系统重调度
	osekTask_Dispatch();

	// 打开中断
	OSEK_TARGET_EnableOSIntWithoutSave();

	// IDLE任务主循环
	while(1)
	{

		OSEK_HOOK_IdleHook();

	}

}


/*
 * @brief
 *    ShutdownOS：关闭操作系统。
 * @param
 * @param[in]	error：错误信息。
 * @param[out]  无。
 * @returns:    无。
 */

void ShutdownOS( StatusType error )
{

	// 关闭中断
	OSEK_TARGET_DisableOSIntWithoutSave() ;

	// 调用系统关闭HOOK函数
	OSEK_HOOK_ShutdownHook(error);

	// 关闭通信模块
#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)
#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0
	CloseCOM();
#endif
#endif

	// 进入无限循环
	while(1)
	{
	}

}


