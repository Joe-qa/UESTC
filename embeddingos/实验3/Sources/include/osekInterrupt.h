/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekInterrupt.h,v $
 * Revision 1.0  2008/03/21 09:41:00
 * 创建文件
*/

/**
 * @file 	osekInterrupt.h
 * @brief
 *	<li>功能：定义中断相关的接口、宏、结构等。</li>
 * @author
 * @date 	2008-03-21
 * <p>部门：
 */

/**
 * \defgroup  Interrupt 中断
 * 中断管理模块,主要实现中断的管理
 */
/**@{*/
/** @name 中断管理模块*/
//@{


#ifndef _OSEK_INTERRUPT_H
#define _OSEK_INTERRUPT_H

/**************************** 引用部分 *********************************/


#ifdef __cplusplus
extern "C" {
#endif


/**************************** 声明部分 *********************************/

// 变量声明
extern OSDWORD osekInterrupt_NestedLevl;

// 内部函数
void osekInterrupt_Initialize( void );

// 中断管理提供给外部的公共调用函数

/**
 * @brief
 *    DisableAllInterrupts：禁止所有中断。
 * @param   	无。
 * @returns     无。
 */
void DisableAllInterrupts( void );

/**
 * @brief
 *    EnableAllInterrupts：使能所有中断。
 * @param   	无。
 * @returns     无。
 */
void EnableAllInterrupts( void );

/**
 * @brief
 *    SuspendAllInterrupts：挂起所有中断。
 * @param   	无。
 * @returns     无。
 */
void SuspendAllInterrupts( void );

/**
 * @brief
 *    ResumeAllInterrupts：恢复所有中断。
 * @param   	无。
 * @returns     无。
 */
void ResumeAllInterrupts( void );

/**
 * @brief
 *    SuspendOSInterrupts：挂起OS相关的中断。
 * @param   	无。
 * @returns     无。
 */
void SuspendOSInterrupts( void );

/**
 * @brief
 *    ResumeOSInterrupts：恢复OS相关的中断。
 * @param   	无。
 * @returns     无。
 */
void ResumeOSInterrupts( void );


/**************************** 定义部分 *********************************/

#define ISR(isrName) void isrName(void)




#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
//@}
/** @}*/
