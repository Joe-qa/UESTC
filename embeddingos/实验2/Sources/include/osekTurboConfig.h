/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekTurboConfig.h,v $
 * Revision 1.0  2008/03/24 13:11:00
 * 创建文件
*/

/*
 * @file 	osekTurboConfig.h
 * @brief
 *	<li>功能：将OSEKTurbo的配置宏定义转换为DeltaOSEK所需要的配置宏定义。</li>
 * @author
 * @date 	2008-03-24
 * <p>部门：
 */


#ifndef _OSEK_TURBO_CONFIG_H
#define _OSEK_TURBO_CONFIG_H


/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/

// 系统配置部分

#if defined(OSBCC1) || defined(OSBCC2)  // 系统中只允许有基本任务
#define CONFIG_OSEK_SYSTEM_BT
#endif

#if defined(OSECC1) || defined(OSECC2) // 系统中允许有基本任务和扩展任务
#define CONFIG_OSEK_SYSTEM_ET
#endif

#if defined(OSBCC1) || defined(OSECC1) // 系统中只允许一个优先级上最多有一个任务
#define CONFIG_OSEK_SYSTEM_BP
#endif

#if defined(OSBCC2) || defined(OSECC2)	// 系统中允许一个优先级上有多个任务，且允许基本任务被多次激活
#define CONFIG_OSEK_SYSTEM_EP
#define CONFIG_OSEK_SYSTEM_MACTIVE
#endif


#if defined(OSEXTSTATUS) // 允许系统有扩展信息返回
#define CONFIG_OSEK_SYSTEM_EXTSTATUS
#endif


#define CONFIG_OSEK_SYSTEM_APPMODES  OSNAPPMODES	// 应用模式数


#define CONFIG_OSEK_SYSTEM_RTI_LEVEL  OSORTIDEBUGLEVEL	// 调试级别



// 任务配置部分

#if defined(OSNOPREEMPT) // 系统中只有非抢占的任务
#define CONFIG_OSEK_TASK_NOPREEMPT
#endif

#if defined(OSMIXPREEMPT) // 系统中有非抢占和抢占的任务
#define CONFIG_OSEK_TASK_MIXPREEMPT
#endif

#if defined(OSFULLPREEMPT) // 系统中都是允许抢占的任务
#define CONFIG_OSEK_TASK_FULLPREEMPT
#endif

#if defined(OSEXTPRIORS) // 表明支持64级优先级，如果未定义则表明支持32级优先级
#define CONFIG_OSEK_TASK_EXTPRIORS
#define CONFIG_OSEK_TASK_PRIORITY_NUMBER 64
#else
#define CONFIG_OSEK_TASK_PRIORITY_NUMBER 32
#endif


#define CONFIG_OSEK_TASK_NUMBER OSNTSKS	// 系统中的任务总数
#define CONFIG_OSEK_TASK_MAX_ID (CONFIG_OSEK_TASK_NUMBER - 2)  // 不包括IDLE任务在内的最大任务ID
#define OSEK_TASK_IDLE_ID ((OSDWORD)(CONFIG_OSEK_TASK_NUMBER - 1))	// IDLE任务的ID
#define CONFIG_OSEK_TASK_TOTAL_ACTIVATE_NUMBER OSTOTALACTIVENUMBER	// 系统中最多的任务激活总次数


#if defined(OSBCC1)&&(!defined(OSEXTSTATUS))&&(!defined(OSNOFASTTERMINATE))
#error "Not Support Fast Terminate Configuration:"
#endif

#if defined(OSSTKCHECK)	// 堆栈检查
#define CONFIG_OSEK_TASK_STACK_CHECK
#endif

#define CONFIG_OSEK_TASK_STACK_BOTTOM



// 资源配置部分

#if defined(OSRESOURCE)	// 使用资源管理功能
#define CONFIG_OSEK_RESOURCE
#endif

#if defined(OSINRES)	// 使用了内部资源
#define CONFIG_OSEK_RESOURCE_INTERNAL
#endif

#define CONFIG_OSEK_RESOURCE_NUMBER OSNALLRES	// 系统中的资源总数（不包括调度器资源）



// HOOK配置部分

#if defined(OSHOOKERROR) // 用户要使用Error hook
#define CONFIG_OSEK_HOOK_ERROR
#endif

#if defined(OSHOOKPRETASK) // 用户要使用任务进入时hook
#define CONFIG_OSEK_HOOK_PRETASK
#endif

#if defined(OSHOOKPOSTTASK) // 用户要使用任务退出前hook
#define CONFIG_OSEK_HOOK_POSTTASK
#endif

#if defined(OSHOOKSTARTUP) // 用户要使用系统启动的hook
#define CONFIG_OSEK_HOOK_STARTUP
#endif

#if defined(OSHOOKSHUTDOWN) // 用户要使用系统关闭的hook
#define CONFIG_OSEK_HOOK_SHUTDOWN
#endif

#if defined(OSHOOKSCHEDULER) // 用户要使用系统空闲的hook
#define CONFIG_OSEK_HOOK_IDLE
#endif



// 中断配置部分

#if defined(OSNOISR2)&& (!defined(OSNOISR1))	// 系统中只有类型1的中断
#define CONFIG_OSEK_INTERRUPT_ONLY_ISR1
#endif

#if defined(OSNOISR1)&& (!defined(OSNOISR2))	// 系统中只有类型2的中断
#define CONFIG_OSEK_INTERRUPT_ONLY_ISR2
#endif

#if (!defined(OSNOISR1))&& (!defined(OSNOISR2))	// 系统中既有类型1又有类型2的中断
#define CONFIG_OSEK_INTERRUPT_ISR_ALL
#endif

#if defined(OSNOISR2)&& defined(OSNOISR1)	// 系统中没有中断
#define CONFIG_OSEK_INTERRUPT_ISR_NULL
#endif



// Counter与Alarm配置部分

#if defined(OSALARM)	// 使用ALARM功能
#define CONFIG_OSEK_ALARM
#endif

#if defined(OSALMSETEVENT)	// 使用ALARM的设置事件功能
#define CONFIG_OSEK_ALARM_EVENT
#endif

#if defined(OSALMCALLBACK)	// 使用ALARM的回调函数
#define CONFIG_OSEK_ALARM_CALLBACK
#endif

#define CONFIG_OSEK_COUNTER_NUMBER  OSNCTRS	// 系统中计数器的总数
#define CONFIG_OSEK_ALARM_NUMBER    OSNALMS	// 系统ALARM总数
#if defined(OSNAUTOALMS)
#define CONFIG_OSEK_ALARM_AUTO_NUMBER	OSNAUTOALMS	// 自动启动的ALARM总数
#endif



// 事件配置部分



// 通信配置部分

#if defined(OSUSEMESGRESOURCE)	// 使用了通信资源
#define		CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE
#endif

#if defined(OSUSEMESGSTATUS)	// 使用了消息状态
#define		CONFIG_OSEK_COM_USE_MESSAGE_STATUS
#endif

// 通信范围
#if defined(LOCALMESSAGESONLY)	// 只有内部通信
#define		CONFIG_OSEK_COM_LOCALMESSAGESONLY
#endif

// 消息通知机制
#if defined(OSSIGACTIVTASK)	// 配置了消息到达时激活任务
#define		CONFIG_OSEK_MSG_ACTIVATETASK
#endif

#if defined(OSSIGEVENT)	// 配置了消息到达时设置事件
#define		CONFIG_OSEK_MSG_SETEVENT
#endif

#if defined(OSSIGCALLBACK)	// 配置了消息到达时回调函数
#define		CONFIG_OSEK_MSG_CALLBACK
#endif

#if defined(OSSIGFLAG)	// 配置了消息到达时设置标志
#define		CONFIG_OSEK_MSG_FLAG
#endif

#if defined (CONFIG_OSEK_COM_LOCALMESSAGESONLY)

// 总消息数量, 配置COM组件服务的开关宏
#define CONFIG_OSEK_COM_NUMBER_OF_MESSAGES		OSNMSGS

// 标志数量, 配置控制块中是否有标志属性，以及配置通知机制相关的服务
#define CONFIG_OSEK_COM_NUMBER_OF_FLAGS			OSNFLAGS

// 队列型消息数量, 配置剪裁控制块结构
#define CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES	OSNQUEMSGS

#endif  /* CONFIG_OSEK_COM_LOCALMESSAGESONLY */



// 编译配置部分

#define CONFIG_OSEK_COMPILER_GCC



// 目标机配置部分

//#define OSMPC555
#define OSPC386		// 临时使用PC386


#if defined(OSFLOATINGPOINT)	// 使用了浮点处理
#define CONFIG_OSEK_TARGET_FLOAT
#endif

// PowerPC目标机系列
#if defined(OSMPC555)
#define CONFIG_OSEK_TARGET_PPC
#define CONFIG_OSEK_TARGET_MPC555
#endif

#if defined(OSMPC561)
#define CONFIG_OSEK_TARGET_PPC
#define CONFIG_OSEK_TARGET_MPC561
#endif

#if defined(OSMPC562)
#define CONFIG_OSEK_TARGET_PPC
#define CONFIG_OSEK_TARGET_MPC562
#endif

#if defined(OSMPC563)
#define CONFIG_OSEK_TARGET_PPC
#define CONFIG_OSEK_TARGET_MPC563
#endif

#if defined(OSMGT5100)
#define CONFIG_OSEK_TARGET_PPC
#define CONFIG_OSEK_TARGET_MGT5100
#endif

#if defined(OSMGT5200)
#define CONFIG_OSEK_TARGET_PPC
#define CONFIG_OSEK_TARGET_MGT5200
#endif

#if defined(OSMPC5500)
#define CONFIG_OSEK_TARGET_PPC
#define CONFIG_OSEK_TARGET_MPC5500
#endif


// X86目标机系列
#if defined(OSPC386)
#define CONFIG_OSEK_TARGET_X86
#define CONFIG_OSEK_TARGET_PC386
#endif


// ARM目标机系列
#if defined(OSARM44b0X)
#define CONFIG_OSEK_TARGET_ARM
#define CONFIG_OSEK_TARGET_ARM44b0X
#endif


// MIPS目标机系列
#if defined(OSMIPSR4000)
#define CONFIG_OSEK_TARGET_MIPS
#define CONFIG_OSEK_TARGET_MIPSR4000
#endif




// 需要转换的宏

#define OSTASKENTRY  T_OSEK_TASK_Entry
#define OSISRCBTYPE  OSDWORD
#define OSTSK        T_OSEK_TASK_ConfigTable

#define OSTSKACTIVATE   OSEK_TASK_ACTIVE
#define OSTSKNONPREEMPT OSEK_TASK_NONPREEMPT
#define OSTSKEXTENDED   OSEK_TASK_EXTENDED
#define OSGETTOPOFSTACK OSEK_TASK_GETTOPOFSTACK

#define OSHOOKENTRY  T_OSEK_ALARM_CallBackEntry
#define OSMSG        T_OSEK_MESSAGE_ConfigTable

/* 保持与OSEKturbo兼容 */
#define __rom       const               /* ROM类型数据 */
#define __ram                           /* RAM类型数据 */



// 将OSEKturbo的配置表通过宏替换方式转换为新的变量名称
#define OsTaskCfgTable osekConfig_TaskTable
#define OsTaskEntry osekTask_EntryTable
#define OsIsr osekInterrupt_IsrEntryTable

#define OsMsg osekConfig_MsgTable


#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif

