/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekHook.c,v $
 * Revision 1.0  2008/04/10 14:54:00
 * 创建文件
*/

/*
 * @file 	osekHook.c
 * @brief
 *	<li>功能：实现HOOK相关内容。</li>
 * @author
 * @date 	2008-04-10
 * <p>部门：
 */


/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"

/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

// 全局变量
#if defined(CONFIG_OSEK_SYSTEM_GETSERVICEID) || defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS)
OSBYTE  osekHook_ServiceId;
OSDWORD osekHook_ObjId;
#endif


#if defined(CONFIG_OSEK_HOOK_ERROR)
OSBYTE  osekHook_ErrorNestedLevel;
#endif

/**************************** 实现部分 *********************************/


