/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekAlarm.c,v $
 * Revision 1.0  2008/03/29 14:44:00
 * 创建文件。
 */

/*
 * @file 	osekAlarm.c
 * @brief
 *	<li>功能：实现报警器的管理。</li>
 * @author
 * @date 	2009-03-29
 * <p>部门：
 */


/**************************** 引用部分 *********************************/
#include "osprop.h"
//#include "osekTarget.h"
#include "osapi.h"
#include <string.h>

#if defined(CONFIG_OSEK_ALARM)

/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

// 全局变量
// ALARM控制块表
T_OSEK_ALARM_ControlBlock osekAlarm_AlarmTable[CONFIG_OSEK_ALARM_NUMBER];

/**************************** 实现部分 *********************************/

/*
 * @brief
 *    GetAlarmBase：获取指定ALARM的基本信息。
 * @param
 * @param[in]	almId：指向ALARM控制块的指针。
 * @param[out]	info： ALARM基本信息结构的引用。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的ALARM。]
 */

StatusType GetAlarmBase( AlarmType almId, AlarmBaseRefType info )
{
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId != almId->alarmId )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetAlarmBase,almId) ;
	}
#endif

	// 通过almId找到相应counter的配置信息，并将其赋值给输出参数info
	*info = osekCounter_CounterTable[almId->configTable.ctrId].configTable ;

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    GetAlarm：获取指定ALARM到期前的相对时间。
 * @param
 * @param[in]	almId: 指向ALARM控制块的指针。
 * @param[out]	tick:  相对时间值的引用。
 * @returns:    E_OK：成功。
 * <p>          E_OS_NOFUNC:指定的ALARM未被使用。
 * <p>          [E_OS_ID:无效的ALARM。]
 */

StatusType GetAlarm( AlarmType almId, TickRefType tick )
{
	CtrType     ctrId;
	OSEK_TARGET_OSIntSave(osIntSave);

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId != almId->alarmId )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetAlarm,almId) ;
	}
#endif

	// 如果指定ALARM未被使用，返回E_OS_NOFUNC
	if( almId->prevAlarm == (AlarmType)0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_NOFUNC,OSServiceId_GetAlarm,almId) ;
	}

	// 获取指定ALARM所对应的counter
	ctrId =  almId->configTable.ctrId;

	// 屏蔽中断
	OSEK_TARGET_DisableOSInt(osIntSave);

	// 获取计数器的当前计数值
	*tick = osekCounter_CounterTable[ctrId].currentTick;

    // counter当前值小于等于ALARM到期值的相对时间计算
	if( almId->expirationTick >= *tick )
    {
        *tick = almId->expirationTick - *tick;
    }
    // counter当前值大于ALARM到期值的相对时间计算
	else
    {
        *tick = almId->expirationTick + osekCounter_CounterTable[ctrId].configTable.maxallowedvalue + 1 - *tick;
    }

	// 打开中断
    OSEK_TARGET_EnableOSInt(osIntSave);

	// 成功，返回E_OK
	return E_OK;

}

/*
 * @brief
 *    SetAlarm：设置ALARM（内部函数）。
 * @param		almId: 指向ALARM控制块的指针。
 *              start: 以tick为单位的起始时间值。
 *              cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 无。
 */
static void SetAlarm( AlarmType almId, TickType start, TickType cycle )
{
    CtrType     ctrId;
	StatusType	status;

	// 获取指定ALARM所对应的counter
    ctrId = almId->configTable.ctrId;

    almId->expirationTick = start;
    almId->periodTick = cycle;

	// 将指定ALARM的控制块插入到相应counter的ALARM链表头节点
    almId->nextAlarm =  osekCounter_CounterTable[ctrId].alarm;
    osekCounter_CounterTable[ctrId].alarm = almId;
    almId->prevAlarm = (AlarmType)(&( osekCounter_CounterTable[ctrId].alarm));
    if ( almId->nextAlarm != (AlarmType)0  )
    {
        ( almId->nextAlarm )->prevAlarm = almId;
    }

	// 如果起始时间等于counter的当前tick值，则调用osekAlarm_Check2函数处理这个已到期的ALARM
    if (start == osekCounter_CounterTable[ctrId].currentTick )
    {
        status = osekAlarm_Check2( almId, &osekCounter_CounterTable[ctrId] );
		// 如果调度标志不为0，则实施调度
		if( status == OSEK_TASK_NEED_DISPATCH )
		{
    		osekTask_Dispatch();
		}
    }
}



/*
 * @brief
 *    SetRelAlarm：设置相对时间ALARM。
 * @param		almId: 指向ALARM控制块的指针。
 *              increment: 以tick为单位的相对起始时间值。
 *              cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    E_OK：成功。
 * <p>          E_OS_STATE:指定的ALARM已经在用。
 * <p>          [E_OS_ID:无效的ALARM。]
 * <p>          [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */

StatusType  SetRelAlarm( AlarmType almId, TickType increment, TickType cycle )
{
	CtrType     ctrId;
	OSEK_TARGET_OSIntSave( osIntSave ) ;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId != almId->alarmId )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_SetRelAlarm,almId) ;
	}
#endif

	// 获取指定ALARM所对应的counter
	ctrId =  almId->configTable.ctrId;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果起始时间值大于counter配置的最大值，返回E_OS_VALUE
	if( increment > osekCounter_CounterTable[ctrId].configTable.maxallowedvalue )
	{
       	OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetRelAlarm,almId) ;
	}

    // 对于周期性ALARM，如果周期值大于counter配置的最大值或低于最小周期值，返回E_OS_VALUE
    if(( cycle != 0 ) && ( (cycle > osekCounter_CounterTable[ctrId].configTable.maxallowedvalue) || (cycle < osekCounter_CounterTable[ctrId].configTable.mincycle) ) )
    {
    	OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetRelAlarm,almId) ;
	}
#endif

    // 如果指定ALARM已经在用，返回E_OS_STATE
    if( almId->prevAlarm != (AlarmType)0 )
    {
		OSEK_HOOK_ErrorHook(E_OS_STATE,OSServiceId_SetRelAlarm,almId) ;
	}

	// 屏蔽中断
    OSEK_TARGET_DisableOSInt(osIntSave);

    // 将相对值转换为绝对值
    if ((osekCounter_CounterTable[ctrId].configTable.maxallowedvalue - osekCounter_CounterTable[ctrId].currentTick) < increment )
    {
        increment -= (osekCounter_CounterTable[ctrId].configTable.maxallowedvalue - osekCounter_CounterTable[ctrId].currentTick + 1);
    }
    else
    {
        increment += osekCounter_CounterTable[ctrId].currentTick;
    }

    // 调用SetAlarm设置ALARM
	SetAlarm(almId,increment,cycle);

	// 打开中断
	OSEK_TARGET_EnableOSInt(osIntSave);

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    SetAbsAlarm：设置绝对时间ALARM。
 * @param		almId: 指向ALARM控制块的指针。
 *              start: 以tick为单位的绝对起始时间值。
 *              cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    E_OK：成功。
 * <p>          E_OS_STATE:指定的ALARM已经在用。
 * <p>          [E_OS_ID:无效的ALARM。]
 * <p>          [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */

StatusType SetAbsAlarm(  AlarmType almId, TickType start, TickType cycle )
{
	CtrType     ctrId;
	OSEK_TARGET_OSIntSave( osIntSave ) ;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId != almId->alarmId )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_SetAbsAlarm,almId) ;
	}
#endif

	// 获取指定ALARM所对应的counter
	ctrId =  almId->configTable.ctrId;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果起始时间值大于counter配置的最大值，返回E_OS_VALUE
	if( start > osekCounter_CounterTable[ctrId].configTable.maxallowedvalue )
	{
		OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetAbsAlarm,almId) ;
	}

    // 对于周期性ALARM，如果周期值大于counter配置的最大值或低于最小周期值，返回E_OS_VALUE
	if(( cycle != 0 ) && ( (cycle > osekCounter_CounterTable[ctrId].configTable.maxallowedvalue) || (cycle < osekCounter_CounterTable[ctrId].configTable.mincycle) ) )
	{
    	OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetAbsAlarm,almId) ;
	}
#endif

    // 如果指定ALARM已经在用，返回E_OS_STATE
	if( almId->prevAlarm != (AlarmType)0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_STATE,OSServiceId_SetAbsAlarm,almId) ;
	}

	// 屏蔽中断
    OSEK_TARGET_DisableOSInt(osIntSave);

	// 调用SetAlarm设置ALARM
    SetAlarm(almId,start,cycle);

	// 打开中断
	OSEK_TARGET_EnableOSInt(osIntSave);

	// 成功设置ALARM，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    CancelAlarm：取消设置的ALARM。
 * @param		almId: 指向ALARM控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    E_OK：成功。
 * <p>          E_OS_NOFUNC:指定的ALARM未在使用。
 * <p>          [E_OS_ID:无效的输入参数。]
 */

StatusType CancelAlarm( AlarmType almId )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;

	#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId != almId->alarmId )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_CancelAlarm,almId) ;
	}
	#endif

	// 屏蔽中断
	OSEK_TARGET_DisableOSInt(osIntSave);

	// 如果指定的ALARM未被使用，返回E_OS_NOFUNC
	if( almId->prevAlarm == (AlarmType)0 )
	{
		OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_OS_NOFUNC,OSServiceId_CancelAlarm,almId) ;
	}

    // 将指定的ALARM控制块从正使用的ALARM链表中去掉
	(almId->prevAlarm)->nextAlarm = almId->nextAlarm;
    if ( almId->nextAlarm != (AlarmType)0 )
    {
        (almId->nextAlarm)->prevAlarm = almId->prevAlarm;
    }

    // 将指定的ALARM设置为未使用的
	almId->prevAlarm = (AlarmType)0 ;

	// 打开中断
	OSEK_TARGET_EnableOSInt(osIntSave);

	// 成功将指定的ALARM取消，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    NotifyAlarm：根据ALARM的配置设置事件或激活任务（内部函数）。
 * @param		almId: 指向ALARM控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 设置事件或激活任务的返回值。
 */

static StatusType NotifyAlarm(AlarmType almId)
{
	T_OSEK_TASK_ControlBlock *tcbPtr;

	// 通过almId获取相应的任务控制块
	tcbPtr = &osekTask_TaskTable[almId->configTable.taskId];

#if defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_ALARM_EVENT)
	// 如果需要设置事件，则调用事件管理的设置事件功能，并返回结果
	if( almId->configTable.setEvent)
	{
 		return osekEvent_SetEvent(tcbPtr,almId->configTable.setEvent);
	}
	else
#endif
	// 如果不需要设置事件，则激活任务，并返回结果
	{
		return osekTask_ActiveTask(tcbPtr);
	}

}


/*
 * @brief
 *    osekAlarm_Check1：检查指定counter的ALARM链表，对满足触发时间条件的ALARM进行相应的处理（callback、设置事件或激活任务），
 *                     对于非周期ALARM将其从链表中去掉，对于周期ALARM调整其下一次触发的时间。（内部函数）
 * @param		ctrId:counter ID号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 无。
 */

void osekAlarm_Check1( CtrType ctrId )
{
    AlarmType   almId;
    OSBYTE      dispatchFlag;
    T_OSEK_COUNTER_ControlBlock *ctrPtr;
    StatusType  status;

	// 初始化调度标志
    dispatchFlag = 0;
	// 获取指定计数器的控制块
    ctrPtr = &osekCounter_CounterTable[ctrId];

    almId = ctrPtr->alarm;

	// 遍历指定counter的ALARM链表
    while (almId != (AlarmType)0 )
    {
		// 检查alarm是否到期并做相应处理
		status = osekAlarm_Check2(almId, ctrPtr);
	    // 判断status的值，如果需要调度，则设置调度标志为1
        if( status == OSEK_TASK_NEED_DISPATCH )
		{
			dispatchFlag = 1;
		}
		// 得到链表中的下一个ALARM
        almId = almId->nextAlarm;
    }

	// 如果调度标志不为0，则实施调度
    if( dispatchFlag != 0 )
    {
    	osekTask_Dispatch();
    }

}


/*
 * @brief
 *    osekAlarm_Check2：检查指定的单个ALARM，如果满足触发时间则进行相应的处理（callback、设置事件或激活任务），
 *                     对于非周期ALARM将其从链表中去掉，对于周期ALARM调整其下一次触发的时间。（内部函数）
 * @param
 * @param[in]	almId:ALARM ID号,实际为指向ALARM控制结构的指针；
 *              ctrPtr:与该ALARM相关联的COUNTER控制块指针。
 * @param[out]	无。
 * @returns: status: 是否需要调度。
 */
StatusType osekAlarm_Check2(AlarmType almId, T_OSEK_COUNTER_ControlBlock *ctrPtr )
{
    StatusType  status;

	status = (StatusType)0;

	// 如果ALARM到期
	if ( almId->expirationTick == ctrPtr->currentTick )
    {
#if defined(CONFIG_OSEK_ALARM_CALLBACK)
		// 如果设置了callback函数，则调用它
        if( almId->configTable.callBack )
        {
            almId->configTable.callBack();
        }
        // 否则设置事件或激活任务
		else
        {
#endif
           status = NotifyAlarm( almId );

#if defined(CONFIG_OSEK_ALARM_CALLBACK)
        }
#endif

		// 如果是非周期ALARM，则将其从链表中去掉
        if (almId->periodTick == 0 )
        {
				(almId->prevAlarm)->nextAlarm = almId->nextAlarm;
			    if ( almId->nextAlarm != (AlarmType)0 )
			    {
			        (almId->nextAlarm)->prevAlarm = almId->prevAlarm;
			    }
		    // 将指定的ALARM设置为未使用的
			almId->prevAlarm = (AlarmType)0;
        }
        // 否则对于周期性ALARM，重新设置其下一次触发的时间
		else
        {
            almId->expirationTick = ctrPtr->currentTick + almId->periodTick;

            if (almId->expirationTick > ctrPtr->configTable.maxallowedvalue )
            {
                almId->expirationTick -= (ctrPtr->configTable.maxallowedvalue + 1);
            }
        }
    }

	return status;
}


/*
 * @brief
 *    osekAlarm_Initialize：根据ALARM配置信息表等对ALARM控制块进行初始化（内部函数）。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 无。
 */
// 该函数中如果启动自动类型ALARM失败，应有返回值。
void osekAlarm_Initialize( void )
{
	int i;
	//StatusType status;

	// 初始化ALARM控制块表
	//memset((void*)osekAlarm_AlarmTable,0,sizeof(osekAlarm_AlarmTable));

	for( i = 0; i < CONFIG_OSEK_ALARM_NUMBER; i++)
	{
		osekAlarm_AlarmTable[i].configTable = osekConfig_AlarmTable[i];

		#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
		osekAlarm_AlarmTable[i].alarmId = &osekAlarm_AlarmTable[i];
		#endif

	}

#if defined(CONFIG_OSEK_ALARM_AUTO_NUMBER)
	// 设置需自动启动的ALARM
	for( i = 0; i < CONFIG_OSEK_ALARM_AUTO_NUMBER; i++)
	{

#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
        if((osekConfig_AutoAlarmTable[i].autoStart & (1 << osekExecution_CurrentAppMode)) != 0 )
		{
#endif  /* (CONFIG_OSEK_SYSTEM_APPMODES > 1) */

			SetAbsAlarm( osekConfig_AutoAlarmTable[i].alarmId, osekConfig_AutoAlarmTable[i].startTick, osekConfig_AutoAlarmTable[i].periodTick );

#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
		}
#endif  /* (CONFIG_OSEK_SYSTEM_APPMODES > 1) */

	}

#endif  /* defined(CONFIG_OSEK_ALARM_AUTO_NUMBER) */

}

#endif /* #if defined(CONFIG_OSEK_ALARM) */

