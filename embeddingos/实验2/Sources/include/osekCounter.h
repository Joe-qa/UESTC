/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekCounter.h,v $
 * Revision 1.0  2008/03/29 11:48:00
 * 创建文件
*/

/**
 * @file 	osekCounter.h
 * @brief
 *	<li>功能：定义计数器相关的数据结构、宏，以及任务相关的申明。</li>
 * @author
 * @date 	2008-03-29
 * <p>部门：
 */

/**
 * \defgroup  Counter 计数器
 * 计数器管理模块,主要实现计数器的管理
 */
/**@{*/
/** @name 计数器管理模块*/
//@{


#ifndef _OSEK_COUNTER_H
#define _OSEK_COUNTER_H

/**************************** 引用部分 *********************************/

#ifdef __cplusplus
extern "C" {
#endif


/**************************** 声明部分 *********************************/

// COUNTER管理提供给外部的公共调用函数

/**
 * @brief
 *    InitCounter：初始化COUNTER的计数值。
 * @param[in]	ctrId：COUNTER的ID号。
 * @param[in]	value：COUNTER的计数值。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的COUNTER ID号。]
 * @returns     [E_OS_VALUE:输入的value值超出范围。]
 * @returns     [E_OS_CALLEVEL:在中断中调用了此函数。]
 */
StatusType InitCounter( CtrType ctrId, TickType value );

/**
 * @brief
 *    GetCounterValue：获取指定 COUNTER 的计数值。
 * @param[in]	ctrId：COUNTER的ID号。
 * @param[out]	ticks：COUNTER 计数值变量的引用。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的COUNTER ID号。]
 */
StatusType GetCounterValue( CtrType ctrId, TickRefType ticks );

/**
 * @brief
 *    CounterTrigger：触发指定的COUNTER。
 * @param[in]	ctrId：COUNTER 的 ID号。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的COUNTER ID号。]
 */
StatusType CounterTrigger( CtrType ctrId );

/**
 * @brief
 *    GetCounterInfo：获取指定 COUNTER 的基本信息。
 * @param[in]	ctrId：COUNTER 的ID号。
 * @param[out]	info： COUNTER 基本信息结构的引用。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的 COUNTER ID号。]
 */
StatusType GetCounterInfo( CtrType ctrId, CtrInfoRefType info );

// 内部函数
void osekCounter_Initialize( void );


/**************************** 定义部分 *********************************/

/// COUNTER 配置表结构
typedef struct T_OSEK_COUNTER_ConfigTable_Struct
{
    /// 允许的最大计数值
    TickType        maxallowedvalue;
    /// 转换常数：每时基的tick数
    TickType        ticksperbase;

// 如果使用了扩展状态
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    /// ALARM 的最小周期值（该属性在配置了扩展返回状态时才存在）
    TickType        mincycle;
#endif
}T_OSEK_COUNTER_ConfigTable_Struct;

/// COUNTER 控制块结构
typedef struct T_OSEK_COUNTER_ControlBlock_Struct
{
#if defined(CONFIG_OSEK_ALARM)
    /// 指向对应的 ALARMs 的指针（该属性在配置了ALARM时才存在）
    AlarmType           alarm;
#endif
    /// COUNTER 的当前计数值
    TickType            currentTick;
    /// COUNTER 的配置表
    T_OSEK_COUNTER_ConfigTable    configTable;
}T_OSEK_COUNTER_ControlBlock_Struct;


#if defined(CONFIG_OSEK_COUNTER_NUMBER)
// COUNTER 控制块表声明
extern T_OSEK_COUNTER_ControlBlock_Struct osekCounter_CounterTable[];
/// COUNTER 配置信息表声明（该变量在配置了计数器时才存在）
extern T_OSEK_COUNTER_ConfigTable osekConfig_CounterTable[];
#endif


#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
//@}
/** @}*/
