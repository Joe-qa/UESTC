/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekDebug.h,v $
 * Revision 1.0  2008/04/10 15:32:00
 * 创建文件
*/

/**
 * @file 	osekDebug.h
 * @brief
 *	<li>功能：声明调试管理的所有外部API、宏等。</li>
 * @author
 * @date 	2008-04-10
 * <p>部门：
 */

/**
 * \defgroup  Debug 调试
 * 调试管理模块,主要实现调试的辅助功能
 */
/**@{*/
/** @name 调试管理模块*/
//@{


#ifndef _OSEK_DEBUG_H
#define _OSEK_DEBUG_H

/**************************** 引用部分 *********************************/


#ifdef __cplusplus
extern "C" {
#endif

/**************************** 声明部分 *********************************/

// 调试管理提供给外部的公共调用函数

/**
 * @brief
 *    GetRunningStackUsage：获取运行任务的堆栈使用情况。
 * @param       无。
 * @returns     OSEK_DEBUG_STACKNOASSIGNED: 指定任务没有堆栈。
 * @returns     栈已使用的字节数。
 */
OSDWORD GetRunningStackUsage( void );

/**
 * @brief
 *    GetStackUsage：获取栈的使用情况。
 * @param[in]	taskId：任务的ID。
 * @returns     OSEK_DEBUG_STACKNOASSIGNED: 指定任务没有堆栈。
 * @returns     栈已使用的字节数。
 */
OSDWORD GetStackUsage( TaskType taskId );

/**
 * @brief
 *    GetTimeStamp：获取时间戳。
 * @param       无。
 * @returns     时间戳的值。
 */
OSDWORD GetTimeStamp( void );

extern void read_timer_value(unsigned int *value);

/**************************** 定义部分 *********************************/

#define OSEK_DEBUG_STACKPATTERN    0x55555555
#define OSEK_DEBUG_STACKNOASSIGNED 0xFFFFFFFF


#define OSEK_Time_ReadCount(value) \
    read_timer_value(value)


#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
//@}
/** @}*/
