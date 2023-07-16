/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekAlarm.h,v $
 * Revision 1.0  2008/03/29 12:23:00
 * 创建文件
*/

/**
 * @file 	osekAlarm.h
 * @brief
 *	<li>功能：定义报警器相关的数据结构、宏，以及相关函数的申明。</li>
 * @author
 * @date 	2008-03-29
 * <p>部门：
 */

/**
 * \defgroup  Alarm 报警
 * Alarm管理模块,主要实现报警的管理
 */
/**@{*/
/** @name Alarm管理模块*/
//@{


#ifndef _OSEK_ALARM_H
#define _OSEK_ALARM_H

/**************************** 引用部分 *********************************/

#ifdef __cplusplus
extern "C" {
#endif

/**************************** 声明部分 *********************************/

// ALARM 管理提供给外部的公共调用函数

/**
 * @brief
 *    SetRelAlarm：设置相对时间ALARM。
 * @param[in]	almId: 指向ALARM控制块的指针。
 * @param[in]   increment: 以tick为单位的相对起始时间值。
 * @param[in]   cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @returns     E_OK：成功。
 * @returns     E_OS_STATE:指定的ALARM已经在用。
 * @returns     [E_OS_ID:无效的ALARM。]
 * @returns     [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */
StatusType SetRelAlarm( AlarmType almId, TickType increment, TickType cycle );

/**
 * @brief
 *    GetAlarm：获取指定ALARM到期前的相对时间。
 * @param[in]	almId: 指向ALARM控制块的指针。
 * @param[out]	tick:  相对时间值的引用。
 * @returns     E_OK：成功。
 * @returns     E_OS_NOFUNC:指定的ALARM未被使用。
 * @returns     [E_OS_ID:无效的ALARM。]
 */
StatusType GetAlarm( AlarmType almId, TickRefType tick );

/**
 * @brief
 *    GetAlarmBase：获取指定ALARM的基本信息。
 * @param[in]	almId：指向ALARM控制块的指针。
 * @param[out]	info： ALARM基本信息结构的引用。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的ALARM。]
 */
StatusType GetAlarmBase( AlarmType almId, AlarmBaseRefType info );

/**
 * @brief
 *    SetAbsAlarm：设置绝对时间ALARM。
 * @param[in]	almId: 指向ALARM控制块的指针。
 * @param[in]   start: 以tick为单位的绝对起始时间值。
 * @param[in]   cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @returns     E_OK：成功。
 * @returns     E_OS_STATE:指定的ALARM已经在用。
 * @returns     [E_OS_ID:无效的ALARM。]
 * @returns     [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */
StatusType SetAbsAlarm(  AlarmType almId, TickType start, TickType cycle );

/**
 * @brief
 *    CancelAlarm：取消设置的ALARM。
 * @param[in]	almId: 指向ALARM控制块的指针。
 * @returns     E_OK：成功。
 * @returns     E_OS_NOFUNC:指定的ALARM未在使用。
 * @returns     [E_OS_ID:无效的输入参数。]
 */
StatusType CancelAlarm( AlarmType almId );

// 内部函数
void osekAlarm_Check1(CtrType ctrId );
StatusType osekAlarm_Check2(AlarmType almId, T_OSEK_COUNTER_ControlBlock *ctrPtr );
void osekAlarm_Initialize( void );


/**************************** 定义部分 *********************************/

/// ALARM 配置信息表结构
typedef struct T_OSEK_ALARM_ConfigTable_Struct
{
	/// 相关任务的ID
    TaskType        taskId;
// ECC 支持事件机制
#if defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_ALARM_EVENT)
    /// 如果setEvent==0 则alarm 激活任务,否则它为该任务设置事件（该属性在ECC情况下才存在）
    EventMaskType   setEvent;
#endif  /*  defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_ALARM_EVENT) */

    /// 相关 Counter 的ID
    CtrType         ctrId;

#if defined(CONFIG_OSEK_ALARM_CALLBACK)
    /// Alarm 回调函数入口（该属性在配置了ALARM回调时才存在）
    T_OSEK_ALARM_CallBackEntry     callBack;
#endif  /* defined(CONFIG_OSEK_ALARM_CALLBACK) */

}T_OSEK_ALARM_ConfigTable_Struct;

/// ALARM 控制块结构
typedef struct T_OSEK_ALARM_ControlBlock_Struct
{
    /// 列表中的下一个 alarm
    AlarmType       nextAlarm;
    /// 列表中的前一个 alarm
    AlarmType       prevAlarm;

// 如果使用了扩展状态
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	/// ALARM ID（该属性在配置了扩展返回状态时才存在）
	AlarmType		alarmId;
#endif

	/// ALARM 的到期时间值
	TickType        expirationTick;
	/// 周期性 ALARM 的周期值
	TickType        periodTick;

	/// ALARM 的配置信息表
	T_OSEK_ALARM_ConfigTable configTable;
}T_OSEK_ALARM_ControlBlock_Struct;

/// ALARM 自动类型配置信息表结构
typedef struct T_OSEK_ALARM_AutoTypeConfigTable_Struct
{
    /// ALARM ID
    AlarmType       alarmId;
    /// ALARM 起始时间值
    TickType        startTick;
    /// 周期性 ALARM 的周期值
    TickType        periodTick;

#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
    /// ALARM 是否自动启动（该属性在应用模式大于1时才存在）
    OSDWORD         autoStart;
#endif  /* (CONFIG_OSEK_SYSTEM_APPMODES > 1) */

}T_OSEK_ALARM_AutoTypeConfigTable_Struct;


#if CONFIG_OSEK_ALARM_NUMBER > 0
// ALARM 控制块表声明
extern T_OSEK_ALARM_ControlBlock osekAlarm_AlarmTable[CONFIG_OSEK_ALARM_NUMBER];
/// ALARM 配置信息表声明（该变量在配置了ALARM时才存在）
extern T_OSEK_ALARM_ConfigTable osekConfig_AlarmTable[CONFIG_OSEK_ALARM_NUMBER];
#if defined(CONFIG_OSEK_ALARM_AUTO_NUMBER)
/// 自动类型ALARM 扩展配置信息表声明（该变量在配置了自动类型ALARM时才存在）
extern T_OSEK_ALARM_AutoTypeConfigTable osekConfig_AutoAlarmTable[CONFIG_OSEK_ALARM_AUTO_NUMBER];
#endif
#endif


#define DeclareAlarm(alarm)


#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
//@}
/** @}*/
