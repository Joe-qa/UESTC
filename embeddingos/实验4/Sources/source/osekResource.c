/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekResource.c,v $
 * Revision 1.0  2008/04/04 14:55:00
 * 创建文件。
 */

/*
 * @file 	osekResource.c
 * @brief
 *	<li>功能：实现对资源的管理，包括获取和释放。</li>
 * @author
 * @date 	2008-04-04
 * <p>部门：
 */


/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"
#include <string.h>

#if defined(CONFIG_OSEK_RESOURCE)

/*************************** 前向声明部分 ******************************/


/**************************** 定义部分 *********************************/

// 全局变量

// 资源控制块表
#if CONFIG_OSEK_RESOURCE_NUMBER > 0
T_OSEK_RESOURCE_ControlBlock osekResource_ResourceTable[CONFIG_OSEK_RESOURCE_NUMBER];
#endif

#if defined(CONFIG_OSEK_RESOURCE_SCHEDULER)
// 资源调度器变量
T_OSEK_RESOURCE_ControlBlock osekResource_Scheduler;
#endif

#if defined(CONFIG_OSEK_RESOURCE_ISR)
// 中断资源链表头
ResourceType    osekResource_ISRLast;
#endif


/**************************** 实现部分 *********************************/

/*
 * @brief
 *    GetResource：获取资源。
 * @param
 * @param[in]	resId：指向资源控制块的指针。
 * @param[out]  无。
 * @returns:    E_OK：成功
 * <p>          [E_OS_ID]: 资源ID不合法
 * <p>          [E_OS_ACCESS]:资源已被使用，或任务优先级大于资源优先级，或系统处于关调度，或没有正确地使用中断资源
 */

StatusType GetResource( ResourceType resId )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;
#if defined(CONFIG_OSEK_RESOURCE_ISR)
	OSEK_TARGET_OSIntSave( save ) ;
	OSEK_TARGET_OSIntSave( vector ) ;
#endif

	// 如果支持资源调度器，则系统关调度
#if defined(CONFIG_OSEK_RESOURCE_SCHEDULER)
	if( resId == RES_SCHEDULER )
	{
		if( osekTask_SchedulerLockLevel != 0 )
		{
			OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
		}
		else
		{
			osekTask_SchedulerLockLevel++;
			return E_OK;
		}
	}
#endif

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果资源ID不合法，则返回E_OS_ID
	if( resId != resId->resourceId )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetResource,resId) ;
	}

	// 如果该资源已被使用，返回E_OS_ACCESS
	if( (resId->isUsed) != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
	}

	//如果在中断程序中调用非中断资源，返回E_OS_ACCESS
	if( osekInterrupt_NestedLevl != 0 )
	{
#if defined(CONFIG_OSEK_RESOURCE_ISR)
		// 中断资源的操作
		if(resId->ceilingPriority < 0)
		{
			// 如果资源优先级小于原始静态配置中断优先级，返回E_OS_ACCESS
			OSEK_ISR_VectorSave(&vector);
			if( ((OSPRIOTYPE)vector)-1 > -(resId->ceilingPriority + 1) )
			{
				OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
			}
		}
		else
#endif /* defined(CONFIG_OSEK_RESOURCE_ISR) */
		{
			// 该资源不属于中断资源
			OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
		}

	}

#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// 任务资源的操作
	if(resId->ceilingPriority >= 0)
#endif /* defined(CONFIG_OSEK_RESOURCE_ISR) */
	{
		// 该资源不属于中断资源
		if( osekInterrupt_NestedLevl != 0 )
		{
			OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
		}

		// 如果资源优先级小于运行任务原始静态配置任务优先级，返回E_OS_ACCESS
#if defined(CONFIG_OSEK_SYSTEM_EP)
		if( osekTask_RunningTask->taskControlBlock->configTable.basePriority > resId->ceilingPriority )
#else
		if( OSEK_TASK_ID2PRI(osekTask_RunningTask->taskControlBlock->configTable.taskId) > resId->ceilingPriority )
#endif /* #else defined(CONFIG_OSEK_SYSTEM_EP) */
		{
			OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId);
		}
	}

	// 如果调度锁不为0，返回E_OS_ACCESS
	if( osekTask_SchedulerLockLevel != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
	}
#endif	// #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)

	OSEK_TARGET_DisableOSInt(osIntSave);

#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// 中断资源的操作
	if(resId->ceilingPriority < 0)
	{
		//增加有异常中断资源的情况
	#if defined (CONFIG_OSEK_TARGET_MPC5554)
		if (-(resId->ceilingPriority) == 17)    /* ISR resource for FIT */
		{
			resId->savedIsrMask = 17;
		}
		else
	#endif
		{
			// 获取当前的中断掩码值
			OSEK_ISR_MaskSave( &save );

        	resId->savedIsrMask = save;
		}


        // 根据中断资源优先级屏蔽当前及更低的中断
        OSEK_ISR_MaskConversion(save, (-(resId->ceilingPriority + 1)));

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
        // 设置资源的已使用标志
        resId->isUsed = 1;
#endif

 		// 将该资源挂接到中断的资源链中
        resId->nextResource = osekResource_ISRLast;
        osekResource_ISRLast = resId;

        // 设置新的中断掩码值
        OSEK_ISR_MaskRestore(save);
	}
	else	// 任务资源的操作
#endif /* defined(CONFIG_OSEK_RESOURCE_ISR) */
	{
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
		// 设置资源状态为已使用
		resId->isUsed = 1;
#endif

		// 将该资源添加到任务的资源链中
		resId->nextResource = osekTask_RunningTask->taskControlBlock->resourceList;
		osekTask_RunningTask->taskControlBlock->resourceList = resId;

		// 将运行任务先优级调整至资源优先级
		if( osekTask_RunningTask->taskControlBlock->curPriority < resId->ceilingPriority )
		{
			osekTask_ChangeRunningTaskPriority(resId->ceilingPriority);
		}
		// chenlr add 2008-12-17
		// 设置资源的当前优先级
		resId->currentPriority = osekTask_RunningTask->taskControlBlock->curPriority;
	}

	OSEK_TARGET_EnableOSInt(osIntSave);

	return E_OK;

}



/*
 * @brief
 *    ReleaseResource：释放资源。
 * @param
 * @param[in]	resId：指向资源控制块的指针。
 * @param[out]  无。
 * @returns:    E_OK:成功
 * <p>          E_OS_NOFUNC:释放的是调度器资源但系统未关调度
 * <p>          [E_OS_ID]: 资源ID不合法
 * <p>          [E_OS_NOFUNC]: 资源未被使用，或要释放的不是任务资源列表的头节点（未按正确顺序释放资源），或未正确使用中断资源
 * <p>          [E_OS_ACCESS]: 任务优先级大于资源优先级，或系统处于关调度
 */
StatusType ReleaseResource( ResourceType resId )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;
#if defined(CONFIG_OSEK_RESOURCE_ISR)
	OSEK_TARGET_OSIntSave( save ) ;
	OSEK_TARGET_OSIntSave( vector ) ;
#endif

#if defined(CONFIG_OSEK_RESOURCE_SCHEDULER)
	// 如果支持资源调度器，并且运行任务不是非抢占类型，则进行任务调度
	if( resId == RES_SCHEDULER )
	{
		if( osekTask_SchedulerLockLevel == 0 )
		{
			OSEK_HOOK_ErrorHook(E_OS_NOFUNC,OSServiceId_ReleaseResource,resId) ;
		}
		else
		{
			osekTask_SchedulerLockLevel--;

			if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_NONPREEMPT ) == 0 )
			{
				OSEK_TARGET_DisableOSInt(osIntSave);
				osekTask_Dispatch();
				OSEK_TARGET_EnableOSInt(osIntSave);
			}

			return E_OK;
		}
	}
#endif

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果资源ID不合法，则返回E_OS_ID
	if( resId != resId->resourceId )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_ReleaseResource,resId) ;
	}

	// 如果资源未被使用，返回E_OS_NOFUNC
	if( resId->isUsed != 1 )
	{
		OSEK_HOOK_ErrorHook(E_OS_NOFUNC,OSServiceId_ReleaseResource, resId) ;
	}

#if defined(CONFIG_OSEK_RESOURCE_ISR)
    if( resId->ceilingPriority < 0 )	// 中断资源的操作
    {
    	if( osekResource_ISRLast != resId )
        {
            OSEK_HOOK_ErrorHook(E_OS_NOFUNC, OSServiceId_ReleaseResource, resId );
        }

        //如果在中断程序中调用非中断资源，返回E_OS_ACCESS
		if( osekInterrupt_NestedLevl != 0 )
		{
			// 如果资源优先级小于原始静态配置中断优先级，返回E_OS_ACCESS
			OSEK_ISR_VectorSave(&vector);
			if( ((OSPRIOTYPE)vector -1) > -(resId->ceilingPriority + 1) )
			{
				OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
			}
		}

    }
    else	// 任务资源部分的操作
#endif /* defined(CONFIG_OSEK_RESOURCE_ISR) */
    {
		// 如果要释放的不是任务资源列表的头节点（未按正确顺序释放资源），返回E_OS_NOFUNC
		if( osekTask_RunningTask->taskControlBlock->resourceList != resId )
		{
			OSEK_HOOK_ErrorHook(E_OS_NOFUNC,OSServiceId_ReleaseResource,resId) ;
		}

		// 如果调度锁不为0，返回E_OS_ACCESS
		if( osekTask_SchedulerLockLevel != 0 )
		{
			OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
		}

		if (osekInterrupt_NestedLevl != 0 )
		{
			OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
		}

		// 如果资源优先级小于运行任务原始静态配置任务优先级，返回E_OS_ACCESS
#if defined(CONFIG_OSEK_SYSTEM_EP)
		if( osekTask_RunningTask->taskControlBlock->configTable.basePriority > resId->ceilingPriority )
#else
		if( OSEK_TASK_ID2PRI(osekTask_RunningTask->taskControlBlock->configTable.taskId) > resId->ceilingPriority )
#endif /* #else defined(CONFIG_OSEK_SYSTEM_EP) */
		{
			OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetResource,resId) ;
		}
	}

#else	// #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
#if defined(CONFIG_OSEK_RESOURCE_ISR)
    if( osekInterrupt_NestedLevl != 0 )
   	{
   		return E_OK;
    }
#endif
#endif	// #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)

	OSEK_TARGET_DisableOSInt(osIntSave);

#if defined(CONFIG_OSEK_RESOURCE_ISR)
    if( resId->ceilingPriority < 0 )	// 中断资源的操作
    {

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
        // 清除资源的已使用标志
        resId->isUsed = 0;
        // 处理中断资源链
        osekResource_ISRLast = resId->nextResource;
#endif
		save = resId->savedIsrMask;

		//增加有异常中断资源的情况
	#if defined (CONFIG_OSEK_TARGET_MPC5554)
		if (-(resId->ceilingPriority) == 17)    /* ISR resource for FIT */
		{
			// 设置新的中断掩码值
	    	OSEK_FIT_MaskRestore(save);
		}
		else
	#endif
		{
			// 设置新的中断掩码值
		    OSEK_ISR_MaskRestore(save);
		}

		OSEK_TARGET_EnableOSInt(osIntSave);

	    // 如果已释放完所有的中断资源
	    if( osekResource_ISRLast == 0 )
		{
		#if defined(CONFIG_OSEK_INTERRUPT_ONLY_ISR2) || defined(CONFIG_OSEK_INTERRUPT_ISR_ALL)
			// 若在中断服务程序中调用该函数时，使用中断处理任务调度
			if( osekInterrupt_NestedLevl != 0 )
			{
				osekTask_IsrTriggeredTaskSwitchNecessary++;
			}
			// 否则实施调度
			else
		#endif /* #if defined(CONFIG_OSEK_INTERRUPT_ONLY_ISR2)|| defined(CONFIG_OSEK_INTERRUPT_ISR_ALL) */
			{
				OSEK_TARGET_DisableOSInt(osIntSave);
				osekTask_Dispatch();
				OSEK_TARGET_EnableOSInt(osIntSave);
			}
		}

    }
    else	// 任务级资源的操作
#endif /* defined(CONFIG_OSEK_RESOURCE_ISR) */
	{

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
		// 设置资源状态为未使用
		resId->isUsed = 0;
#endif

		// 调整运行任务的资源列表
    	osekTask_RunningTask->taskControlBlock->resourceList = resId->nextResource;

		// 如果任务还拥有其他资源，调整任务优先级以匹配资源的优先级，并进行调度
		resId = resId->nextResource;
		if( resId != 0 )
	    {
	      	if( osekTask_RunningTask->taskControlBlock->curPriority > resId->currentPriority )
	       	{
	       	    osekTask_ChangeRunningTaskPriority( resId->currentPriority );

	            if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_NONPREEMPT ) == 0 )
				{
				  	 osekTask_Dispatch();
				}
	        }
	    }
		// 任务不再拥有任何资源，则进行如下操作
	    else
	    {

#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
			// 如果使用了内部资源，并且任务优先级大于其内部资源优先级，则调整其优先级为内部资源优先级并实施调度
	        if( osekTask_RunningTask->taskControlBlock->curPriority > osekTask_RunningTask->taskControlBlock->configTable.inresPriority && osekTask_RunningTask->taskControlBlock->configTable.inresPriority != 0)
	        {
				osekTask_ChangeRunningTaskPriority( osekTask_RunningTask->taskControlBlock->configTable.inresPriority );
				if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_NONPREEMPT ) == 0 )
				{
					osekTask_Dispatch();
				}
			}
			// 没有使用内部资源
			else if(osekTask_RunningTask->taskControlBlock->configTable.inresPriority == 0)
			{
#endif /* if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */

#if defined(CONFIG_OSEK_SYSTEM_EP)
				// 如果使用了扩展优先级，如果任务优先级不等于其基本优先级，调整任务优先级到基本优先级，并进行调度
				if( osekTask_RunningTask->taskControlBlock->curPriority != osekTask_RunningTask->taskControlBlock->configTable.basePriority )
				{
					osekTask_ChangeRunningTaskPriority( osekTask_RunningTask->taskControlBlock->configTable.basePriority );
					if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_NONPREEMPT ) == 0 )
					{
						osekTask_Dispatch();
					}
				}

#else
				// 如果未使用扩展优先级，如果任务优先级不等于其ID转化的优先级，调整任务优先级到ID转化的优先级，并进行调度
				if( osekTask_RunningTask->taskControlBlock->curPriority != OSEK_TASK_ID2PRI(osekTask_RunningTask->taskControlBlock->configTable.taskId) )
				{
					osekTask_ChangeRunningTaskPriority(OSEK_TASK_ID2PRI(osekTask_RunningTask->taskControlBlock->configTable.taskId));

					if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_NONPREEMPT ) == 0 )
					{
	       				osekTask_Dispatch();
					}
				}
#endif

#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
			}
#endif

	    }

	    OSEK_TARGET_EnableOSInt(osIntSave);
		return E_OK;

	}

	return E_OK;

}


/*
 * @brief
 *    osekResource_Initialize：资源初始化（内部函数）。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */

void osekResource_Initialize( void )
{

#if CONFIG_OSEK_RESOURCE_NUMBER > 0

	int i;

	// 资源控制块清零
	//memset((void*)osekResource_ResourceTable,0,sizeof(osekResource_ResourceTable));

	for( i = 0; i < CONFIG_OSEK_RESOURCE_NUMBER ; i++)
	{
		// 设置每个资源的天花板优先级
		osekResource_ResourceTable[i].ceilingPriority = osekConfig_ResourcePriorityTable[i];
		osekResource_ResourceTable[i].currentPriority = osekConfig_ResourcePriorityTable[i];

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
		// 设置资源的ID
		osekResource_ResourceTable[i].resourceId = &osekResource_ResourceTable[i];
#endif

	}

#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// 初始化中断资源链
	osekResource_ISRLast = 0;
#endif

#endif /* #if CONFIG_OSEK_RESOURCE_NUMBER > 0 */

}


#endif /* #if defined(CONFIG_OSEK_RESOURCE) */

