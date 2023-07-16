/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekExecution.h,v $
 * Revision 1.0  2008/03/12 13:33:00
 * 创建文件
*/

/**
 * @file 	osekExecution.h
 * @brief
 *	<li>功能：定义执行相关的数据结构、宏、申明等。</li>
 * @author
 * @date 	2008-03-12
 * <p>部门：
 */

/**
 * \defgroup  Execution 系统执行
 * 系统执行管理模块,主要实现系统执行的管理
 */
/**@{*/
/** @name 系统执行管理模块*/
//@{


#ifndef _OSEK_EXECUTION_H
#define _OSEK_EXECUTION_H

/**************************** 引用部分 *********************************/


#ifdef __cplusplus
extern "C" {
#endif


/**************************** 声明部分 *********************************/

// 执行管理提供给外部的公共调用函数

/**
 * @brief
 *    StartOS：启动操作系统。
 * @param[in]	mode：应用模式。
 * @returns     无。
 */
void StartOS( AppModeType mode );

/**
 * @brief
 *    ShutdownOS：关闭操作系统。
 * @param[in]	error：错误信息。
 * @returns     无。
 */
void ShutdownOS( StatusType error );

// 变量声明
extern OSBYTE osekExecution_CurrentAppMode;


/**************************** 定义部分 *********************************/

/**
 * @brief
 *    GetActiveApplicationMode：获取当前的应用模式。
 * @param   	无。
 * @returns     当前应用模式。
 */
#define GetActiveApplicationMode() (osekExecution_CurrentAppMode)




#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
//@}
/** @}*/
