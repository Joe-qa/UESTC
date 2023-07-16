/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekMessage.c,v $
 * Revision 1.0  2008/03/25 08:48:00  chenlr
 * 创建文件；
 */

/*
 * @file 	osekMessage.c
 * @brief
 *	<li>功能：实现符合OSEK COM标准的通信管理功能。</li>
 * @author Chen-LiRong
 * @date 2008-03-25
 * <p>部门：
 */



/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"

#include    <string.h>                          // 本实现文件中使用了其中的memcpy()



#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)
#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0

/*************************** 前向声明部分 ******************************/

// 局部函数
// 锁定消息
StatusType msg_LockMessage( MsgType msg );
// 当消息到达时进行通知
#if defined(OSEK_MSG_USE_NOTIFICATION)
StatusType msg_SignalMessage( MsgType msg );
#endif  /* defined(OSEK_MSG_USE_NOTIFICATION) */


/**************************** 定义部分 *********************************/

// 全局变量
// 消息控制块表
T_OSEK_MESSAGE_ControlBlock osekMsg_MessageTable[CONFIG_OSEK_COM_NUMBER_OF_MESSAGES];

// 局部函数
// 解锁消息
#define msg_UnlockMessage( msg )  ((msg)->messageStatus &= ((OSBYTE)~OSEK_COM_MESSAGE_LOCKED))
// 设置标志
#define msg_SetFlag(msg) ((msg)->messageFlag = TRUE)


/**************************** 实现部分 *********************************/

/*
 * @brief
 *    Msg_Init：根据消息配置信息初始化所有的消息控制块（内部函数）。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 无。
 */

void Msg_Init( void )
{
    OSMSGIDTYPE i;

    for( i = 0; i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES; i++ )
    {
        osekMsg_MessageTable[i].messageStatus = 0;
		osekMsg_MessageTable[i].messageConfigTable = osekConfig_MsgTable[i];

#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0
		osekMsg_MessageTable[i].messageFlag = FALSE;
#endif

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
		// 对于队列型消息，初始化其读、写指针和已有消息数量。
        if (osekMsg_MessageTable[i].messageConfigTable.fifoLength != 0)
        {
            osekMsg_MessageTable[i].fifoWrite = osekMsg_MessageTable[i].messageConfigTable.messagePtr;
            osekMsg_MessageTable[i].fifoRead = osekMsg_MessageTable[i].fifoWrite;
            osekMsg_MessageTable[i].fifoCount = 0;
        }
#endif

    }
}


#if defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS)

/*
 * @brief
 *    GetMessageStatus：获取消息的状态。
 * @param		msg：指向特定消息对象控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    [E_COM_ID: 无效消息]
 * <p>          E_COM_LOCKED: 消息被锁定
 * <p>          E_COM_BUSY: 消息处于忙状态
 * <p>          E_COM_NOMSG: 消息队列中无消息
 * <p>          E_COM_LIMIT: 消息队列满
 * <p>          E_OK: 不是以上任何状态
 *
 * <p>Notes:    除ID无效外，本函数在任何情况下都不调用error hook。
 */

StatusType GetMessageStatus( MsgType msg )
{
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

	// 检查消息是否有效，无效返回E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_GetMessageStatus,msg);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

	// 如果消息被锁定，返回E_COM_LOCKED
    if( (msg->messageStatus & OSEK_COM_MESSAGE_LOCKED) != 0 )
    {
		return E_COM_LOCKED;
    }

#if defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE)
	// 如果消息处于忙状态，返回E_COM_BUSY
    if( (msg->messageStatus & OSEK_COM_MESSAGE_BUSY) != 0 )
    {
		return E_COM_BUSY;
    }
#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE) */

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES
	// 对于队列型消息，如果已有消息数为0，返回E_COM_NOMSG；如果队列溢出，返回E_COM_LIMIT
    if( (msg->messageConfigTable.fifoLength != 0) )
#endif
    {
        if( msg->fifoCount == 0 )
        {
			return E_COM_NOMSG;
        }
        if( (msg->messageStatus & OSEK_COM_MESSAGE_OVERFLOW) != 0 )
        {
			return E_COM_LIMIT;
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0 */

    return E_OK;

}

#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS) */


/*
 * @brief
 *    SendMessage：发送消息。
 * @param		msg：指向特定消息对象控制块的指针。
 *              data：被发送数据域的引用（首地址）。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:E_OK            成功返回。
 * <p>      E_COM_LOCKED    消息对象已经被锁定。
 * <p>      [E_COM_ID       无效消息]
 * <p>      [E_OS_CALLEVEL  在ISR中调用并使用了无效的配置]
 *
 * <p>Notes:无
 */

StatusType SendMessage(MsgType msg, AccessNameRef data)
{
    OSEK_TARGET_OSIntSave( osIntSave );

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

	// 检查消息是否有效，无效返回E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_SendMessage,msg);
    }

    // 如果是从ISR中调用的该服务，则检查从ISR中调用的正确性。如果非队列消息使用了非拷贝方式或者是队列消息，返回E_OS_CALLEVEL
    if ( osekInterrupt_NestedLevl != 0 )
    {
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES //如果不满足则全部都是队列消息
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES == 0                                  //全部都是非队列消息
		// 非拷贝方式
        if( msg->messageConfigTable.messagePtr == data )
#else														                        //既有队列消息又有非队列消息
        //非队列消息使用了非拷贝方式或者是队列消息
        if( (msg->messageConfigTable.messagePtr == data) || (msg->messageConfigTable.fifoLength != 0) )
#endif
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */
        {
			OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_SendMessage,msg);
        }
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

    OSEK_TARGET_DisableOSInt(osIntSave);


	// 试图锁定消息，如果失败返回E_COM_LOCKED
    if (msg_LockMessage( msg ) != E_OK )
    {
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_LOCKED,OSServiceId_SendMessage,msg);
    }

    OSEK_TARGET_EnableOSInt(osIntSave);

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES //如果不满足，则都是队列消息
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                  //如果不满足，则都是非队列消息
    // 非队列消息
    if( msg->messageConfigTable.fifoLength == 0 )
#endif
    {
        // 如果是拷贝方式发送，则将指定的数据拷贝到消息对象中
        if (msg->messageConfigTable.messagePtr != data )
        {
            memcpy( (void*)msg->messageConfigTable.messagePtr, data, msg->messageConfigTable.messageSize );
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                  //如果不满足，则全是非队列消息
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES //既有队列消息又有非队列消息
    // 队列消息，只能使用拷贝方式
    else
#endif
    {
        // 已有消息数等于队列长度，设置消息状态为溢出，当下次接收消息时可获得这个错误信息;指定数据未被发送到消息队列中，它被简单地丢弃掉
        if( msg->fifoCount == msg->messageConfigTable.fifoLength )
        {
            msg->messageStatus |= OSEK_COM_MESSAGE_OVERFLOW;
        }
        else
        {
            // 将指定数据拷贝到消息队列中
            memcpy( (void*)msg->fifoWrite, data, msg->messageConfigTable.messageSize );

            // 如果写指针到达队列尾部，将其调整至头部
            if( msg->fifoWrite == msg->messageConfigTable.fifoLast )
            {
                msg->fifoWrite = (OSBYTEPTR)((msg->messageConfigTable).messagePtr);
            }
            // 否则将消息队列的写指针前进一步
            else
            {
                msg->fifoWrite += msg->messageConfigTable.messageSize;
            }

            msg->fifoCount++;
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0 */

    // 解锁消息对象
    msg_UnlockMessage( msg );

#if defined(OSEK_MSG_USE_NOTIFICATION)
	// 当消息到达时产生通知信号

    OSEK_TARGET_DisableOSInt(osIntSave);

    if ((msg_SignalMessage( msg ) == OSEK_TASK_NEED_DISPATCH)        // 在通知过程中如果有任务被激活
        && (osekInterrupt_NestedLevl == 0)                           // 并且是在任务级调用的此服务，则调用操作系统的派遣功能
    )
    {
    	osekTask_Dispatch();
    }

    OSEK_TARGET_EnableOSInt(osIntSave);

#endif  /* defined(OSEK_MSG_USE_NOTIFICATION) */

    return E_OK;

}


/*
 * @brief
 *    ReceiveMessage：接收消息。
 * @param		msg：指向特定消息对象控制块的指针。
 *              data：接收数据的缓冲区的引用（首地址）。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:E_OK            成功返回。
 * <p>      E_COM_LOCKED    消息已经被锁定。
 * <p>      E_COM_NOMSG     消息队列为空。
 * <p>      E_COM_LIMIT：   自上次调用以来队列中有消息溢出。
 * <p>      [E_COM_ID       无效消息。]
 * <p>      [E_OS_CALLEVEL  在ISR中调用并使用了无效的配置]
 *
 * <p>Notes:无
 */

StatusType ReceiveMessage(MsgType msg, AccessNameRef data)
{
    OSEK_TARGET_OSIntSave( osIntSave );
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
	// 先将返回状态缺省设置为E_OK
    StatusType status = E_OK;
#endif

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

    // 检查消息是否有效，无效返回E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_ReceiveMessage,msg);
    }

    // 是从ISR中调用的该服务，则检查从ISR中调用的正确性。如果非队列消息使用了非拷贝方式或者是队列消息，返回E_OS_CALLEVEL
    if ( osekInterrupt_NestedLevl != 0 )
    {
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES  //如果不满足则全部都是队列消息
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES == 0                                   //全部都是非队列消息
        // 非拷贝方式
        if( msg->messageConfigTable.messagePtr == data )
#else														                         //既有队列消息又有非队列消息
        //非队列消息使用了非拷贝方式或者是队列消息
        if( (msg->messageConfigTable.messagePtr == data) || (msg->messageConfigTable.fifoLength != 0) )
#endif
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */
        {
			OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_ReceiveMessage,msg);
        }
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

    OSEK_TARGET_DisableOSInt(osIntSave);


    // 试图锁定消息，如果失败返回E_COM_LOCKED
    if( msg_LockMessage( msg ) != E_OK )
    {
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_LOCKED,OSServiceId_ReceiveMessage,msg);
    }

    OSEK_TARGET_EnableOSInt(osIntSave);

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES  //如果不满足，则都是队列消息
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                   //如果不满足，则都是非队列消息
    // 对于非队列消息，如果是以拷贝方式接收消息，则将消息对象中的数据拷贝到指定位置
    if( msg->messageConfigTable.fifoLength == 0 )
#endif
    {
        if( msg->messageConfigTable.messagePtr != data )
        {
            memcpy( data, (void*)msg->messageConfigTable.messagePtr, msg->messageConfigTable.messageSize );
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                   //如果不满足，则全是非队列消息
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES  //既有队列消息又有非队列消息
    // 对于队列消息，将以拷贝方式接收消息
    else
#endif
    {
        // 队列中没有消息，解锁消息对象，返回E_COM_NOMSG
        if( msg->fifoCount == 0 )
        {
            msg_UnlockMessage( msg );
			OSEK_HOOK_ErrorHook(E_COM_NOMSG,OSServiceId_ReceiveMessage,msg);
        }

        // 如果消息队列溢出，清除溢出状态，并设置status为E_COM_LIMIT
        if( (msg->messageStatus & OSEK_COM_MESSAGE_OVERFLOW) != 0 )
        {
            status = E_COM_LIMIT;
            msg->messageStatus &= (OSBYTE)~OSEK_COM_MESSAGE_OVERFLOW;
        }
        // 从消息队列中拷贝消息
        memcpy( data, (void*)msg->fifoRead, msg->messageConfigTable.messageSize );

        // 如果读指针到达消息队列尾部，将其调整至头部
        if( msg->fifoRead == msg->messageConfigTable.fifoLast )
        {
            msg->fifoRead = (OSBYTEPTR)((msg->messageConfigTable).messagePtr);
        }
        // 否则将消息队列的读指针前进一步
        else
        {
            msg->fifoRead += msg->messageConfigTable.messageSize;
        }

        msg->fifoCount--;
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0 */

    msg_UnlockMessage( msg );

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
    // 返回错误信息
    if( status != E_OK )
    {
		OSEK_HOOK_ErrorHook(status,OSServiceId_ReceiveMessage,msg);
    }
#endif

    return E_OK;

}


#if defined(OSEK_MSG_USE_NOTIFICATION)

/*
 * @brief
 *    msg_SignalMessage：当消息到达时被调用，完成有关的通知功能：激活任务或设置事件或回调函数或设置标志（内部函数）。
 * @param		msg：指向特定消息对象控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:OSEK_TASK_NEED_DISPATCH（0xFF）            需要重新调度。
 *          0或其他值（被本函数调用的其他函数的返回值）   不需要重新调度。
 *
 * Notes:
 */

StatusType msg_SignalMessage( MsgType msg )
{

#if defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT)
    TaskType taskId;
	T_OSEK_TASK_ControlBlock *tcbPtr;
#endif

#if defined(CONFIG_OSEK_MSG_FLAG)
	// 如果配置了标志，则设置标志并返回0
    if( msg->messageConfigTable.messageSigFlag != OSEK_COM_NO_FLAG )
    {
        msg_SetFlag(msg);
        return 0;
    }
#endif  /* defined(CONFIG_OSEK_MSG_FLAG) */

#if defined(CONFIG_OSEK_MSG_CALLBACK)
    // 如果配置了回调函数，则调用它并且返回0
    if (msg->messageConfigTable.messageSigCallBack != 0 )
    {
        (msg->messageConfigTable.messageSigCallBack)();
        return 0;
    }
#endif  /* defined(CONFIG_OSEK_MSG_CALLBACK) */

#if defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_ACTIVATETASK)
	// 如果配置了设置事件或激活任务，获取任务ID号和控制块
    taskId = msg->messageConfigTable.messageSigTask;
    // 覆盖测试发现下列4行代码冗余，如果保证配置没有问题，它们是永远不会执行的。
    //if( taskId == INVALID_TASK )
    //{
    //	return 0;
    //}
	tcbPtr = &osekTask_TaskTable[taskId];
#endif  /* defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_ACTIVATETASK) */

#if defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_MSG_SETEVENT)
	// 如果配置了需设置的事件，则设置事件并返回其结果
    if( msg->messageConfigTable.messageSigEvent != 0 )
    {
	 	return osekEvent_SetEvent(tcbPtr,msg->messageConfigTable.messageSigEvent);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_MSG_SETEVENT) */

#if defined(CONFIG_OSEK_MSG_ACTIVATETASK)
  	// 上述条件均不满足，并且配置了需激活的任务，则激活任务并返回其结果
  	return osekTask_ActiveTask(tcbPtr);
#endif  /* defined(CONFIG_OSEK_MSG_ACTIVATETASK) */

    return 0;  /* 程序不会执行到这里，此句只是防止编译器警告 */

}

#endif  /* defined(OSEK_MSG_USE_NOTIFICATION) */


/*
 * @brief
 *    msg_LockMessage：当发送或接收消息的时候调用本函数锁定消息（内部函数）。
 * @param		msg：指向特定消息对象控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:E_OK, 成功返回。
 *          E_COM_LOCKED, 消息已经被锁定。
 * Notes:  本函数应该在禁止中断的情况下被调用。
 */

StatusType msg_LockMessage( MsgType msg )
{
    // 如果消息对象已经被锁定，返回E_COM_LOCKED
    if ((msg->messageStatus & (OSEK_COM_MESSAGE_LOCKED )) != 0 )
    {
        return E_COM_LOCKED;
    }

    // 锁定消息
    msg->messageStatus |= OSEK_COM_MESSAGE_LOCKED;

    return E_OK;
}


#if defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE)

/*
 * @brief
 *    GetMessageResource：将指定消息对象的状态设置为“忙”。
 * @param		msg：指向特定消息对象控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:E_OK, 成功返回。
 * <p>      E_COM_LOCKED, 消息已经被锁定。
 * <p>      E_COM_BUSY, 消息已经被设置为“忙”状态。
 * <p>      [E_COM_ID,   (扩展状态)   无效消息。]
 */

StatusType GetMessageResource( MsgType msg )
{
    OSEK_TARGET_OSIntSave( osIntSave );

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;
	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

    // 检查消息是否有效，无效返回E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_GetMessageResource,msg);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */


    OSEK_TARGET_DisableOSInt(osIntSave);

    // 试图锁定消息，如果失败返回E_COM_LOCKED
    if( msg_LockMessage(msg) != E_OK )
    {
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_LOCKED,OSServiceId_GetMessageResource,msg);
    }

    // 如果消息处于忙状态，返回E_COM_BUSY
    if( (msg->messageStatus & OSEK_COM_MESSAGE_BUSY) != 0 )
    {
        msg_UnlockMessage( msg );
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_BUSY,OSServiceId_GetMessageResource,msg);
    }

    // 设置消息对象为“忙”状态
    msg->messageStatus |= OSEK_COM_MESSAGE_BUSY;
    // 解锁消息
    msg_UnlockMessage( msg );
    OSEK_TARGET_EnableOSInt(osIntSave);

    return E_OK;

}


/*
 * @brief
 *    ReleaseMessageResource：清除消息对象的“忙”状态。
 * @param		msg：指向特定消息对象控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:E_OK, 成功返回。
 * <p>      [E_COM_ID,   (扩展状态)   无效消息。]
 */

StatusType ReleaseMessageResource( MsgType msg )
{
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

    // 检查消息是否有效，无效返回E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_ReleaseMessageResource,msg);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

    // 解除消息的“忙”状态
    msg->messageStatus &= (OSBYTE)~OSEK_COM_MESSAGE_BUSY;
    return E_OK;

}

#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE) */


#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0

/*
 * @brief
 *    ReadFlag：读取标志。
 * @param		flagName：指定标志的名称。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 标志值。
 */

FlagValue ReadFlag(FlagType flagName)
{
    return (*flagName);
}

/*
 * @brief
 *    ResetFlag：复位标志。
 * @param		flagName：指定标志的名称。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: E_OK, 成功返回。
 */

StatusType ResetFlag( FlagType flagName )
{
    OSEK_TARGET_OSIntSave( osIntSave );

    OSEK_TARGET_DisableOSInt(osIntSave);

    *flagName = FALSE;

    OSEK_TARGET_EnableOSInt(osIntSave);

    return E_OK;
}

#endif /* CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0 */


/*
 * @brief
 *    StartCOM：启动 COM。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns：应用提供的消息初始化函数的返回值。
 */

StatusType StartCOM( void )
{
    StatusType osRetVal;

    // 调用COM模块的初始化函数
    Msg_Init();
    // 调用应用提供的初始化函数
    osRetVal = MessageInit();

    return osRetVal;
}


/*
 * @brief
 *    InitCOM：初始化COM模块。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: E_OK, 成功返回。
 */

StatusType InitCOM( void )
{
    return E_OK;
}


/*
 * @brief
 *    CloseCOM：关闭COM模块。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: E_OK, 成功返回。
 */

StatusType CloseCOM( void )
{
    return E_OK;
}

/*
 * @brief
 *    StopCOM：检查所有消息状态。
 * @param		ShutdownMode：关闭模式（在本实现中未利用此参数）。
 * @param[in]	无。
 * @param[out]	无。
 * @return: E_OK,       成功返回。
 * <p>      E_COM_BUSY, 有消息处于“忙”状态或被锁定。
 */

StatusType StopCOM(OSBYTE ShutdownMode)
{
    OSMSGIDTYPE i;

    for( i = 0; i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES; i++ )
    {
        if( (osekMsg_MessageTable[i].messageStatus & ( OSEK_COM_MESSAGE_BUSY | OSEK_COM_MESSAGE_LOCKED )) != 0 )
        {
			OSEK_HOOK_ErrorHook(E_COM_BUSY,OSServiceId_StopCOM,&osekMsg_MessageTable[i]);
        }
    }

    return E_OK;
}

#endif  /* CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0 */
#endif  /* defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY) */
