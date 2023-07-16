/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekMessage.h,v $
 * Revision 1.0  2008/03/26 13:16:00  chenlr
 * 创建文件
 */

/**
 * @file 	osekMessage.h
 * @brief
 *	<li>功能：声明消息通信模块的所有外部API，以及内部模块间的全局API。</li>
 * @author 	chenlr
 * @date 	2008-03-26
 * <p>部门：
 */

/**
 * \defgroup  Message 消息通信
 * 通信管理模块,主要实现内部消息通信的管理
 */
/**@{*/
/** @name 通信管理模块*/
//@{


#ifndef _OSEK_MESSAGE_H
#define _OSEK_MESSAGE_H

/**************************** 引用部分 *********************************/


#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)

/**************************** 声明部分 *********************************/

// 内部函数
void Msg_Init( void );

/**
 * @brief
 *    MessageInit：应用提供的消息初始化函数原型，该函数将被StartCOM函数调用。
 * @param   	无。
 * @returns     应用自定义的返回状态，类型为StatusType。
 */
extern StatusType MessageInit (void);

#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0

//  通信管理提供给外部的公共调用函数

/**
 * @brief
 *    SendMessage：发送消息（在配置了消息时该函数才存在）。
 * @param[in]	msg：指向特定消息对象控制块的指针。
 * @param[in]	data：被发送数据域的引用（首地址）。
 * @returns     E_OK            成功返回。
 * @returns     E_COM_LOCKED    消息对象已经被锁定。
 * @returns     [E_COM_ID       无效消息]
 * @returns     [E_OS_CALLEVEL  在ISR中调用并使用了无效的配置]
 */
StatusType SendMessage(MsgType msg, AccessNameRef data);

/**
 * @brief
 *    ReceiveMessage：接收消息（在配置了消息时该函数才存在）。
 * @param[in]	msg：指向特定消息对象控制块的指针。
 * @param[in]	data：接收数据的缓冲区的引用（首地址）。
 * @returns     E_OK            成功返回。
 * @returns     E_COM_LOCKED    消息已经被锁定。
 * @returns     E_COM_NOMSG     消息队列为空。
 * @returns     E_COM_LIMIT：   自上次调用以来队列中有消息溢出。
 * @returns     [E_COM_ID       无效消息。]
 * @returns     [E_OS_CALLEVEL  在ISR中调用并使用了无效的配置]
 */
StatusType ReceiveMessage(MsgType msg, AccessNameRef data);

/**
 * @brief
 *    StopCOM：检查所有消息状态（在配置了消息时该函数才存在）。
 * @param[in]	ShutdownMode：关闭模式（在本实现中未利用此参数）。
 * @returns     E_OK       成功返回。
 * @returns     E_COM_BUSY 有消息处于“忙”状态或被锁定。
 */
StatusType StopCOM(OSBYTE ShutdownMode);

/**
 * @brief
 *    StartCOM：启动 COM（在配置了消息时该函数才存在）。
 * @param		无。
 * @returns     应用提供的消息初始化函数的返回值。
 */
StatusType StartCOM( void );

/**
 * @brief
 *    InitCOM：初始化COM模块（在配置了消息时该函数才存在）。
 * @param		无。
 * @returns     E_OK, 成功返回。
 */
StatusType InitCOM( void );

/**
 * @brief
 *    CloseCOM：关闭COM模块（在配置了消息时该函数才存在）。
 * @param		无。
 * @returns     E_OK, 成功返回。
 */
StatusType CloseCOM( void );

// 如果配置了消息状态相关功能
#if defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS)
/**
 * @brief
 *    GetMessageStatus：获取消息的状态（在配置了消息状态时该函数才存在）。
 * @param[in]	msg：指向特定消息对象控制块的指针。
 * @returns     [E_COM_ID: 无效消息]
 * @returns     E_COM_LOCKED: 消息被锁定
 * @returns     E_COM_BUSY: 消息处于忙状态
 * @returns     E_COM_NOMSG: 消息队列中无消息
 * @returns     E_COM_LIMIT: 消息队列满
 * @returns     E_OK: 不是以上任何状态
 *
 * <p>Notes:    除ID无效外，本函数在任何情况下都不调用error hook。
 */
StatusType GetMessageStatus( MsgType msg );
#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS) */

// 如果要使用消息到达标志
#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0
/**
 * @brief
 *    ReadFlag：读取标志（在配置了消息到达设置标志时该函数才存在）。
 * @param[in]	flagName：指定标志的名称。
 * @returns     标志值。
 */
FlagValue ReadFlag(FlagType flag);

/**
 * @brief
 *    ResetFlag：复位标志（在配置了消息到达设置标志时该函数才存在）。
 * @param[in]	flagName：指定标志的名称。
 * @returns     E_OK, 成功返回。
 */
StatusType ResetFlag(FlagType flag);
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0 */

// 如果要使用消息资源
#if defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE)
/**
 * @brief
 *    GetMessageResource：将指定消息对象的状态设置为“忙”（在配置了消息资源时该函数才存在）。
 * @param[in]	msg：指向特定消息对象控制块的指针。
 * @returns     E_OK, 成功返回。
 * @returns     E_COM_LOCKED, 消息已经被锁定。
 * @returns     E_COM_BUSY, 消息已经被设置为“忙”状态。
 * @returns     [E_COM_ID,   (扩展状态)   无效消息。]
 */
StatusType GetMessageResource( MsgType msg );

/**
 * @brief
 *    ReleaseMessageResource：清除消息对象的“忙”状态（在配置了消息资源时该函数才存在）。
 * @param[in]	msg：指向特定消息对象控制块的指针。
 * @returns     E_OK, 成功返回。
 * @returns     [E_COM_ID,   (扩展状态)   无效消息。]
 */
StatusType ReleaseMessageResource( MsgType msg );
#endif /* defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE) */

#endif  /* CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0  */

/**************************** 定义部分 *********************************/

#if defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_CALLBACK) || defined(CONFIG_OSEK_MSG_FLAG)
#define OSEK_MSG_USE_NOTIFICATION
#endif /* defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_CALLBACK) || defined(CONFIG_OSEK_MSG_FLAG) */

#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0

/// 消息配置信息表结构（在配置了消息时该函数才存在）
typedef struct T_OSEK_MESSAGE_ConfigTable_Struct
{
    /// 消息对象的ID(在消息数组中的序号)
    OSMSGIDTYPE    messageId;
    /// 指向消息实体的指针
    void*          messagePtr;
    /// 以字节为单位的单个消息尺寸
    OSDWORD        messageSize;
// 如果配置了消息到达时激活任务或设置事件
#if defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT)
    /// 消息到达时被通知的任务（在配置了消息到达激活任务或设置事件时该属性才存在）
    TaskType       messageSigTask;
#endif
// 如果配置了消息到达时设置事件
#if defined(CONFIG_OSEK_MSG_SETEVENT)
    /// 消息到达时被设置的事件（在配置了消息到达设置事件时该属性才存在）
    EventMaskType  messageSigEvent;
#endif
// 如果配置了消息到达时回调函数
#if defined(CONFIG_OSEK_MSG_CALLBACK)
    /// 消息到达时被调用的回调函数（在配置了消息到达回调时该属性才存在）
    OSCALLBACK     messageSigCallBack;
#endif
// 如果配置了消息到达时设置标志
#if defined(CONFIG_OSEK_MSG_FLAG)
    /// 消息到达时被设置的标志（在配置了消息到达设置标志时该属性才存在）
    OSBYTE         messageSigFlag;
#endif
// 如果有队列型消息对象
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
    /// 指向消息队列最后元素的指针（在配置了队列型消息对象时该属性才存在）
    OSBYTEPTR      fifoLast;
    /// 消息队列的长度（在配置了队列型消息对象时该属性才存在）
    OSBYTE         fifoLength;
#endif
}T_OSEK_MESSAGE_ConfigTable_Struct;

/// 消息控制块结构（在配置了消息时该函数才存在）
typedef struct T_OSEK_MESSAGE_ControlBlock_Struct
{
    /// 消息状态信息
    OSBYTE          messageStatus;
    /// 消息配置信息表
	T_OSEK_MESSAGE_ConfigTable messageConfigTable;
// 如果有队列型消息对象
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
	///下一条消息写入队列中的位置（在配置了队列型消息对象时该属性才存在）
    OSBYTEPTR       fifoWrite;
    /// 队列中下一条被读取的消息的位置（在配置了队列型消息对象时该属性才存在）
    OSBYTEPTR       fifoRead;
    /// 队列中已有消息数量（在配置了队列型消息对象时该属性才存在）
    OSBYTE          fifoCount;
#endif

// 如果消息到达标志数大于0
#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0
	/// 该消息对象使用的标志（在配置了消息到达设置标志时该属性才存在）
    OSBYTE          messageFlag;
#endif

}T_OSEK_MESSAGE_ControlBlock_Struct;

// 消息对象控制块表声明
extern T_OSEK_MESSAGE_ControlBlock osekMsg_MessageTable[];
/// 外部消息配置表变量声明（在配置了消息时该函数才存在）
extern const T_OSEK_MESSAGE_ConfigTable osekConfig_MsgTable[];


/// 立即关闭通信模块的宏（在配置了消息时该函数才存在）
#define COM_SHUTDOWN_IMMEDIATE  0

/// 消息状态:消息已经被锁定（在配置了消息时该函数才存在）
#define OSEK_COM_MESSAGE_LOCKED             0x01
/// 消息状态:消息对象处于“忙”状态（在配置了消息时该函数才存在）
#define OSEK_COM_MESSAGE_BUSY               0x02
/// 消息状态:消息溢出（在配置了消息时该函数才存在）
#define OSEK_COM_MESSAGE_OVERFLOW           0x80

/// 无消息到达标志的宏（在配置了消息时该函数才存在）
#define OSEK_COM_NO_FLAG                0xFF

#endif  /* CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0 */

#endif  /* defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY) */

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* _OSEK_MESSAGE_H */


/* end of include file */
//@}
/** @}*/
