/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osapi.h,v $
 * Revision 1.0  2008/03/04 10:34:00
 * 创建文件
*/

/**
 * @file 	osapi.h
 * @brief
 *	<li>功能：本文件作为头文件使用的统一外部头文件，其包含了操作系统各个组成部分的子头文件。</li>
 * @author
 * @date 	2008-03-04
 * <p>部门：
 */


#ifndef _OS_API_H
#define _OS_API_H


#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_OSEK_OCC 
#include "osekOccConfig.h"
#else
#include "osekTurboConfig.h"
#endif


// 定义操作系统API调用的返回状态
/// 操作系统API调用返回状态 0：操作成功，用于OSEK的各个组件中
#define E_OK  			( (StatusType)0 )

// 1...16：OS部分的错误码区域
/// 操作系统API调用返回状态 :访问权限错误
#define E_OS_ACCESS 	( (StatusType)1 )
/// 操作系统API调用返回状态 :调用层次错误
#define E_OS_CALLEVEL 	( (StatusType)2 )
/// 操作系统API调用返回状态 :对象ID错误
#define E_OS_ID 		( (StatusType)3 )
/// 操作系统API调用返回状态 :超出范围错误
#define E_OS_LIMIT 		( (StatusType)4 )
/// 操作系统API调用返回状态 :没有此功能
#define E_OS_NOFUNC 	( (StatusType)5 )
/// 操作系统API调用返回状态 :资源未释放
#define E_OS_RESOURCE 	( (StatusType)6 )
/// 操作系统API调用返回状态 :状态错误
#define E_OS_STATE 		( (StatusType)7 )
/// 操作系统API调用返回状态 :数值错误
#define E_OS_VALUE 		( (StatusType)8 )

// 32...63：COM部分的错误码区域
/// 操作系统API调用返回状态 :消息对象“忙”
#define E_COM_BUSY 		( (StatusType)(1 + 32) )
/// 操作系统API调用返回状态 :无效的消息对象
#define E_COM_ID 		( (StatusType)(2 + 32) )
/// 操作系统API调用返回状态 :消息队列有溢出
#define E_COM_LIMIT 	( (StatusType)(3 + 32) )
/// 操作系统API调用返回状态 :消息对象被锁定
#define E_COM_LOCKED 	( (StatusType)(4 + 32) )
/// 操作系统API调用返回状态 :消息队列中无消息
#define E_COM_NOMSG 	( (StatusType)(5 + 32) )

// 17...31: 实现自定义区域
/// 操作系统API调用返回状态 :内部堆栈溢出
#define E_OS_SYS_STACK      ( (StatusType)17 )
/// 操作系统API调用返回状态 :不正确的功能调用顺序
#define E_OS_SYS_ORDER      ( (StatusType)18 )
/// 操作系统API调用返回状态 :主堆栈溢出
#define E_OS_SYS_MAINSTACK  ( (StatusType)19 )
/// 操作系统API调用返回状态 :中断栈溢出
#define E_OS_SYS_ISRSTACK   ( (StatusType)20 )


/// 无效的任务ID
#define INVALID_TASK ((OSDWORD)0xFFFFFFFF)

#define NULLPTR  ((void *)0)


// 定义基本数据类型
/// OSBYTE类型定义
typedef unsigned char       OSBYTE;
/// OSBYTEPTR类型定义
typedef unsigned char      *OSBYTEPTR;
/// OSWORD类型定义
typedef unsigned short      OSWORD;
/// OSWORDPTR类型定义
typedef unsigned short      *OSWORDPTR;
/// OSDWORD类型定义
typedef unsigned long        OSDWORD;
typedef unsigned char       OSCARDINAL;
/// OSSIGNEDDWORD类型定义
typedef signed long          OSSIGNEDDWORD;
/// OSPRIOTYPE类型定义
typedef signed short         OSPRIOTYPE;
/// OSTASKTYPE类型定义
typedef OSDWORD             OSTASKTYPE;

/// AppModeType类型定义
typedef OSBYTE			AppModeType;
/// StatusType类型定义
typedef unsigned char   StatusType;

/// TaskType类型定义
typedef OSDWORD			TaskType;
/// TaskRefType类型定义
typedef TaskType		*TaskRefType;
/// TaskStateType类型定义
typedef OSBYTE			TaskStateType;
/// TaskStateRefType类型定义
typedef TaskStateType	*TaskStateRefType;

/// EventMaskType类型定义
typedef OSDWORD			EventMaskType;
/// EventMaskRefType类型定义
typedef EventMaskType	*EventMaskRefType;

typedef OSDWORD			IntMaskType;

/// OSCALLBACK类型定义
typedef void          (*OSCALLBACK)( void );


// COM特定的数据类型定义
/// FlagType类型定义
typedef unsigned char      *FlagType;
/// FlagValue类型定义
typedef unsigned char       FlagValue;
/// 消息对象ID的类型定义
typedef OSBYTE              OSMSGIDTYPE;
/// AccessNameRef类型定义：消息数据域的地址
typedef void*               AccessNameRef;
/// 标志未被设置
#if defined(CONFIG_OSEK_TARGET_PPC)
#define FALSE         ((FlagValue)0)
/// 标志被设置
#define TRUE          ((FlagValue)1)
#endif


#define TASK( TaskName ) void Func##TaskName( void )
/// T_OSEK_TASK_Entry类型定义
typedef void (*T_OSEK_TASK_Entry)( void );  // 任务函数入口


/// 任务函数声明的定义
#define DeclareTask(TaskID)     TASK(TaskID)
/// 中断处理函数声明的定义
#define DeclareISR(isrName)     void isrName(void)

// 资源管理控制结构的相关类型定义
/// ResourceType类型定义
typedef struct T_OSEK_RESOURCE_ControlBlock_Struct *ResourceType;
/// T_OSEK_RESOURCE_ControlBlock类型定义
typedef struct T_OSEK_RESOURCE_ControlBlock_Struct T_OSEK_RESOURCE_ControlBlock;

/// T_OSEK_TARGET_TaskContext（任务上下文结构）类型重定义
typedef struct T_OSEK_TARGET_TaskContext_struct T_OSEK_TARGET_TaskContext;
/// T_OSEK_TARGET_ExceptionFrame（X86的异常框架结构）类型重定义
typedef struct T_OSEK_TARGET_ExceptionFrame_struct T_OSEK_TARGET_ExceptionFrame;

/// T_OSEK_TASK_ControlBlock（任务管理控制结构类型）重定义
typedef struct T_OSEK_TASK_ControlBlock_Struct T_OSEK_TASK_ControlBlock;
typedef struct T_OSEK_TASK_ReadyBlock_Struct T_OSEK_TASK_ReadyBlock;

/// T_OSEK_TASK_ConfigTable（任务配置表结构）类型重定义
typedef struct T_OSEK_TASK_ConfigTable_Struct T_OSEK_TASK_ConfigTable;

/// T_OSEK_COUNTER_ControlBlock（COUNTER管理控制结构）类型重定义
typedef struct T_OSEK_COUNTER_ControlBlock_Struct  T_OSEK_COUNTER_ControlBlock;

/// T_OSEK_COUNTER_ConfigTable（COUNTER配置表结构）类型重定义
typedef struct T_OSEK_COUNTER_ConfigTable_Struct T_OSEK_COUNTER_ConfigTable;

/// T_OSEK_ALARM_ControlBlock（ALARM管理控制结构）类型重定义
typedef struct T_OSEK_ALARM_ControlBlock_Struct T_OSEK_ALARM_ControlBlock;

/// T_OSEK_ALARM_ConfigTable（ALARM配置表结构）类型重定义
typedef struct T_OSEK_ALARM_ConfigTable_Struct T_OSEK_ALARM_ConfigTable;

/// T_OSEK_ALARM_AutoTypeConfigTable（自动类型ALARM配置表结构）类型重定义
typedef struct T_OSEK_ALARM_AutoTypeConfigTable_Struct T_OSEK_ALARM_AutoTypeConfigTable;

/// T_OSEK_MESSAGE_ControlBlock（消息控制结构）类型重定义
typedef struct T_OSEK_MESSAGE_ControlBlock_Struct  T_OSEK_MESSAGE_ControlBlock;
/// MsgType类型定义
typedef T_OSEK_MESSAGE_ControlBlock*        MsgType;

/// T_OSEK_MESSAGE_ConfigTable（消息配置表结构）类型重定义
typedef struct T_OSEK_MESSAGE_ConfigTable_Struct  T_OSEK_MESSAGE_ConfigTable;


/// TickType类型定义
typedef OSDWORD             TickType;
/// TickRefType类型定义
typedef TickType*           TickRefType;

/// CtrType（Counter ID）类型定义
typedef unsigned char       CtrType;

/// CtrInfoType（Counter特性即配置信息）类型重定义
typedef T_OSEK_COUNTER_ConfigTable     CtrInfoType;
// CtrInfoRefType（指向Counter特性信息的指针）类型定义
typedef CtrInfoType*        CtrInfoRefType;

// 根据测试集的ext_bcc1_full_alarm_t2用例增加的
/// 报警回调函数原型
#define ALARMCALLBACK(alarmCallback)  void alarmCallback( void )
/// ALARM回调函数入口类型定义
typedef void          (*T_OSEK_ALARM_CallBackEntry)( void );

/// AlarmType（指向ALARM控制块的指针）类型定义
typedef T_OSEK_ALARM_ControlBlock*            AlarmType;
/// AlarmBaseType（Counter配置表信息）类型定义
typedef T_OSEK_COUNTER_ConfigTable            AlarmBaseType;
/// AlarmBaseRefType（Counter配置表的引用）类型定义
typedef AlarmBaseType*      AlarmBaseRefType;


/// OSSERVICEIDTYPE类型定义
typedef OSBYTE  OSSERVICEIDTYPE;

#if defined(CONFIG_OSEK_SYSTEM_GETSERVICEID)

#define OSErrorGetServiceId()       (osekHook_ServiceId)

// OSEK OS 服务组标识
#define OSSYSTEMSERVICEIDBASE       0x00    // 操作系统执行控制服务组
#define OSTASKSERVICEIDBASE         0x10    // 任务管理服务组
#define OSISRSERVICEIDBASE          0x20    // 中断处理服务组
#define OSRESOURCESERVICEIDBASE     0x30    // 资源管理服务组
#define OSEVENTSERVICEIDBASE        0x40    // 事件控制服务组
#define OSMESSAGESERVICEIDBASE      0x50    // 消息通信服务组
#define OSCOUNTERSERVICEIDBASE      0x70    // counter服务组
#define OSALARMSERVICEIDBASE        0x80    // alarm服务组
#define OSHOOKSERVICEIDBASE         0x90    // hook例程组


#define OSServiceId_NoService                   (OSSERVICEIDTYPE)0xFF

// 操作系统执行控制服务
#define OSServiceId_StartOS                     (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x02 )
#define OSServiceId_ShutdownOS                  (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x04 )
#define OSServiceId_GetActiveApplicationMode    (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x06 )
// 任务管理服务
#define OSServiceId_ActivateTask                (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x00 )
#define OSServiceId_TerminateTask               (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x02 )
#define OSServiceId_ChainTask                   (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x04 )
#define OSServiceId_Schedule                    (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x06 )
#define OSServiceId_GetTaskID                   (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x08 )
#define OSServiceId_GetTaskState                (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x0A )
// 中断处理服务
#define OSServiceId_ResumeAllInterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x00 )
#define OSServiceId_SuspendAllinterrupts        (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x02 )
#define OSServiceId_ResumeOSInterrupts          (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x04 )
#define OSServiceId_SuspendOSinterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x06 )
#define OSServiceId_EnableAllInterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x08 )
#define OSServiceId_DisableAllInterrupts        (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x0A )
// 资源管理服务
#define OSServiceId_GetResource                 (OSSERVICEIDTYPE)( OSRESOURCESERVICEIDBASE + 0x00 )
#define OSServiceId_ReleaseResource             (OSSERVICEIDTYPE)( OSRESOURCESERVICEIDBASE + 0x02 )
// 事件控制服务
#define OSServiceId_SetEvent                    (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x00 )
#define OSServiceId_ClearEvent                  (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x02 )
#define OSServiceId_GetEvent                    (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x04 )
#define OSServiceId_WaitEvent                   (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x06 )
// 消息通信服务
#define OSServiceId_SendMessage                 (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x00 )
#define OSServiceId_ReceiveMessage              (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x02 )
#define OSServiceId_GetMessageResource          (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x04 )
#define OSServiceId_ReleaseMessageResource      (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x06 )
#define OSServiceId_GetMessageStatus            (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x08 )
#define OSServiceId_StartCOM                    (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x0A )
#define OSServiceId_StopCOM                     (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x0C )
#define OSServiceId_InitCOM                     (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x0E )
#define OSServiceId_CloseCOM                    (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x10 )
#define OSServiceId_ReadFlag                    (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x12 )
#define OSServiceId_ResetFlag                   (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x14 )
// counter服务
#define OSServiceId_InitCounter                 (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x00 )
#define OSServiceId_CounterTrigger              (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x02 )
#define OSServiceId_GetCounterValue             (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x04 )
#define OSServiceId_GetCounterInfo              (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x06 )
// alarm服务
#define OSServiceId_GetAlarmBase                (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x00 )
#define OSServiceId_GetAlarm                    (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x02 )
#define OSServiceId_SetRelAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x04 )
#define OSServiceId_SetAbsAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x06 )
#define OSServiceId_CancelAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x08 )
// hook例程
#define OSServiceId_ErrorHook                   (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x00 )
#define OSServiceId_PreTaskHook                 (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x02 )
#define OSServiceId_PostTaskHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x04 )
#define OSServiceId_StartupHook                 (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x06 )
#define OSServiceId_ShutdownHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x08 )
#define OSServiceId_IdleLoopHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x0A )

#endif


#if defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS)
// 获取服务的第一个参数的宏
// 任务管理服务
#define OSError_StartOS_Mode()              ((AppModeType)osekHook_ObjId)
#define OSError_ActivateTask_TaskID()      ( (TaskType)osekHook_ObjId)
#define OSError_ChainTask_TaskID()          ((TaskType)osekHook_ObjId)
#define OSError_GetTaskState_TaskID()      ( (TaskType)osekHook_ObjId)
// 资源管理服务
#define OSError_GetResource_ResID()         ((ResourceType)osekHook_ObjId)
#define OSError_ReleaseResource_ResID()    ( (ResourceType)osekHook_ObjId)
// 事件控制服务
#define OSError_SetEvent_TaskID()           ((TaskType)osekHook_ObjId)
#define OSError_GetEvent_TaskID()           ((TaskType)osekHook_ObjId)
// 消息通信服务
#define OSError_SendMessage_Message()      ( (MsgType)osekHook_ObjId)
#define OSError_ReceiveMessage_Message()   ( (MsgType)osekHook_ObjId)
#define OSError_GetMessageResource_Message()       ( (MsgType)osekHook_ObjId)
#define OSError_ReleaseMessageResource_Message()   ( (MsgType)osekHook_ObjId)
// alarm 服务
#define OSError_GetAlarmBase_AlarmID()      ((AlarmType)osekHook_ObjId)
#define OSError_GetAlarm_AlarmID()          ((AlarmType)osekHook_ObjId)
#define OSError_SetRelAlarm_AlarmID()       ((AlarmType)osekHook_ObjId)
#define OSError_SetAbsAlarm_AlarmID()       ((AlarmType)osekHook_ObjId)
#define OSError_CancelAlarm_AlarmID()       ((AlarmType)osekHook_ObjId)
// counter服务
#define OSError_InitCounter_CounterID()     ((CtrType)osekHook_ObjId)
#define OSError_CounterTrigger_CounterID()  ((CtrType)osekHook_ObjId)
#define OSError_GetCounterValue_CounterID() ((CtrType)osekHook_ObjId)
#define OSError_GetCounterInfo_CounterID()  ((CtrType)osekHook_ObjId)

#endif  /* defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS) */

// 包含各管理功能模块的头文件
/// 包含目标平台相关头文件
#include "osekTarget.h"
///	包含中断管理模块头文件
#include "osekInterrupt.h"
/// 包含HOOK管理模块头文件
#include "osekHook.h"
/// 包含任务管理模块头文件
#include "osekTask.h"
/// 包含资源管理模块头文件
#include "osekResource.h"
/// 包含系统执行管理模块头文件
#include "osekExecution.h"
/// 包含计数器管理模块头文件
#include "osekCounter.h"
///	包含报警管理模块头文件
#include "osekAlarm.h"
///	包含事件管理模块头文件
#include "osekEvent.h"
/// 包含调试管理模块头文件
#include "osekDebug.h"
///	包含消息管理模块头文件
#include "osekMessage.h"

#ifdef __cplusplus
}
#endif


#endif

