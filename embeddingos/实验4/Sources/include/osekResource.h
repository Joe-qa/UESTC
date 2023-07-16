/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekResource.h,v $
 * Revision 1.0  2008/03/11 14:23:00
 * 创建文件
*/

/**
 * @file 	osekResource.h
 * @brief
 *	<li>功能：定义资源相关的数据结构、宏、申明等。</li>
 * @author
 * @date 	2008-03-11
 * <p>部门：
 */

/**
 * \defgroup  Resource 资源
 * 资源管理模块,主要实现资源的管理
 */
/**@{*/
/** @name 资源管理模块*/
//@{


#ifndef _OSEK_RESOURCE_H
#define _OSEK_RESOURCE_H

/**************************** 引用部分 *********************************/

#ifdef __cplusplus
extern "C" {
#endif

/**************************** 声明部分 *********************************/

// 内部函数
void osekResource_Initialize( void );
// 中断掩码寄存器操作
extern void int_mask_save(unsigned int *value);
extern void int_mask_restore(unsigned int value);
extern void int_fit_restore(unsigned int value);
extern void int_vector_save(unsigned int *value);


// 资源管理提供给外部的公共调用函数

/**
 * @brief
 *    GetResource：获取资源。
 * @param[in]	resId：指向资源控制块的指针。
 * @returns     E_OK：成功
 * @returns     [E_OS_ID]: 资源ID不合法
 * @returns     [E_OS_ACCESS]:资源已被使用，或任务优先级大于资源优先级，或系统处于关调度，或没有正确地使用中断资源
 */
StatusType GetResource( ResourceType resId );

/**
 * @brief
 *    ReleaseResource：释放资源。
 * @param[in]	resId：指向资源控制块的指针。
 * @returns     E_OK:成功
 * @returns     E_OS_NOFUNC:释放的是调度器资源但系统未关调度
 * @returns     [E_OS_ID]: 资源ID不合法
 * @returns     [E_OS_NOFUNC]: 资源未被使用，或要释放的不是任务资源列表的头节点（未按正确顺序释放资源），或未正确使用中断资源
 * @returns     [E_OS_ACCESS]: 任务优先级大于资源优先级，或系统处于关调度
 */
StatusType ReleaseResource( ResourceType resId );




/**************************** 定义部分 *********************************/

/// 资源的控制结构
typedef struct T_OSEK_RESOURCE_ControlBlock_Struct
{

// 如果使用了扩展状态
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	/// 资源ID号（在配置了扩展返回状态时该属性才存在）
	ResourceType    resourceId;
	/// 资源是否被使用（在配置了扩展返回状态时该属性才存在）
	OSBYTE          isUsed;
#endif

// 如果使用了中断资源
#if defined(CONFIG_OSEK_RESOURCE_ISR)
	/// 保存占用资源之前的中断掩码（在配置了中断资源时该属性才存在）
	IntMaskType 		savedIsrMask;
#endif

	/// 资源的优先级
	OSPRIOTYPE      ceilingPriority;
	/// 任务获取该资源后的当前优先级（即截止该资源获取后的最高优先级）
	OSPRIOTYPE		currentPriority;
	/// 指向下一个资源
	ResourceType    nextResource;

}T_OSEK_RESOURCE_ControlBlock_Struct;


#define DeclareResource(resource)

#if defined(CONFIG_OSEK_RESOURCE_ISR)


#define OSEK_ISR_MaskSave(save) \
    int_mask_save(save)

#define OSEK_ISR_MaskRestore(save) \
    int_mask_restore(save)

#define OSEK_FIT_MaskRestore(save) \
    int_fit_restore(save)

#define OSEK_ISR_VectorSave(save) \
    int_vector_save(save)

#define OSEK_ISR_MaskConversion(save, resLevel) \
    save = resLevel
    //save = ~( (1 << (32 - resLevel)) - 1 )


#define OSEK_ISR_MaskFirstMorethenSecond(mask1, mask2) \
    mask1 > mask2

#define OSEK_ISR_MaskCopyFirst2Second(mask1, mask2) \
    mask2 = mask1

#endif


// 变量声明
#if CONFIG_OSEK_RESOURCE_NUMBER > 0
// 资源控制块表声明
extern T_OSEK_RESOURCE_ControlBlock osekResource_ResourceTable[CONFIG_OSEK_RESOURCE_NUMBER];
/// 资源优先级配置表声明（在配置了资源时该变量才存在）
extern OSPRIOTYPE osekConfig_ResourcePriorityTable[CONFIG_OSEK_RESOURCE_NUMBER];
#endif

#if defined(CONFIG_OSEK_RESOURCE_SCHEDULER)
// 调度器资源控制块声明
extern T_OSEK_RESOURCE_ControlBlock osekResource_Scheduler;
/// 调度器资源重命名（在配置了调度器资源时该宏才存在）
#define RES_SCHEDULER              &osekResource_Scheduler
#endif

#if defined(CONFIG_OSEK_RESOURCE_ISR)
// 中断资源控制块链声明
extern ResourceType    osekResource_ISRLast;
#endif



#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
//@}
/** @}*/
