/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekTarget.h,v $
 * Revision 1.0  2008/03/12 11:24:00
 * 创建文件
*/

/**
 * @file 	osekTarget.h
 * @brief
 *	<li>功能：定义CPU相关的数据结构、宏、申明等。</li>
 * @author
 * @date 	2008-03-12
 * <p>部门：
 */


#ifndef _OSEK_TARGET_H
#define _OSEK_TARGET_H

#include "hidef.h"
/**************************** 引用部分 *********************************/


#ifdef __cplusplus
extern "C" {
#endif

/**************************** 声明部分 *********************************/
#if defined(CONFIG_OSEK_TARGET_X86) || defined(CONFIG_OSEK_TARGET_PPC)
// 内部函数
OSDWORD osekTarget_SaveContext( void * savedContext );
void osekTarget_RestoreContext( void * restoredContext );

// 下面的函数用于第一次启动扩展任务和基本任务时调整任务的栈
void osekTarget_LoadETSP(void* sp) ;
void osekTarget_LoadBTSP( void );
void osekTarget_SaveBTSP(OSDWORD sp );

// 变量声明
extern OSDWORD osekTarget_AllIntMask;
extern OSDWORD osekTarget_NestedAllIntMask;
extern OSDWORD osekTarget_NestedOsIntMask;
#if defined(CONFIG_OSEK_TARGET_X86)
extern OSDWORD osekTarget_SavedBTSP;
#endif
#endif

/**************************** 定义部分 *********************************/

#if defined(CONFIG_OSEK_TARGET_PPC)


/// PPC的任务上下文结构，在配置了PPC目标机时该结构才存在
typedef struct T_OSEK_TARGET_TaskContext_struct
{
	/// pc寄存器
	OSDWORD pc;
	/// sp寄存器
	OSDWORD sp;
	/// gpr14-gpr31
	OSDWORD gpr[18];
#if defined(CONFIG_OSEK_TARGET_FLOAT)
	/// fpr14-fpr31，在配置了浮点处理时该属性才存在
	double  fpr[18];
#endif/* #if defined(CONFIG_OSEK_TARGET_FLOAT)  */
}T_OSEK_TARGET_TaskContext_struct;


#define OSEK_TARGET_SaveBTSP(sp) osekTarget_SaveBTSP(sp)


// 下面的宏用于内核中访问临界区数据时开关中断使用，原则上应该只关闭ISR2类的中断

#if defined(CONFIG_OSEK_COMPILER_GCC)

#if defined(CONFIG_OSEK_TARGET_MPC5554)

#define OSEK_TARGET_OSIntSave( osIntSave ) OSDWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave) asm("mfmsr %0" : "=r"(osIntSave):); asm ("wrteei 0")
#define OSEK_TARGET_EnableOSInt(osIntSave)  asm("mtmsr %0": :"r"(osIntSave))

#define OSEK_TARGET_DisableOSIntWithoutSave() asm ("wrteei 0")
#define OSEK_TARGET_EnableOSIntWithoutSave()  asm ("wrteei 1")

#define OSEK_TARGET_DisableAllInt() asm("mfmsr %0" : "=r"(osekTarget_AllIntMask):); asm ("wrteei 0")
#define OSEK_TARGET_EnableAllInt()  asm("mtmsr %0": :"r"(osekTarget_AllIntMask))

#define OSEK_TARGET_DisableNestedAllInt()  asm("mfmsr %0" : "=r"(osekTarget_NestedAllIntMask):); asm ("wrteei 0")
#define OSEK_TARGET_EnableNestedAllInt()   asm("mtmsr %0": :"r"(osekTarget_NestedAllIntMask))

#define OSEK_TARGET_DisableNestedOsInt() asm("mfmsr %0" : "=r"(osekTarget_NestedOsIntMask):); asm ("wrteei 0")
#define OSEK_TARGET_EnableNestedOsInt()  asm("mtmsr %0": :"r"(osekTarget_NestedOsIntMask))

#endif //#if defined(CONFIG_OSEK_TARGET_MPC5554)


#if defined(CONFIG_OSEK_TARGET_MPC555)

#define OSEK_TARGET_OSIntSave( osIntSave ) OSDWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave) asm("mfmsr %0" : "=r"(osIntSave):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableOSInt(osIntSave)  asm("mtmsr %0": :"r"(osIntSave))

#define OSEK_TARGET_DisableOSIntWithoutSave() asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableOSIntWithoutSave()  asm ("mtspr 80, 0")

#define OSEK_TARGET_DisableAllInt() asm("mfmsr %0" : "=r"(osekTarget_AllIntMask):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableAllInt()  asm("mtmsr %0": :"r"(osekTarget_AllIntMask))

#define OSEK_TARGET_DisableNestedAllInt()  asm("mfmsr %0" : "=r"(osekTarget_NestedAllIntMask):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableNestedAllInt()   asm("mtmsr %0": :"r"(osekTarget_NestedAllIntMask))

#define OSEK_TARGET_DisableNestedOsInt() asm("mfmsr %0" : "=r"(osekTarget_NestedOsIntMask):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableNestedOsInt()  asm("mtmsr %0": :"r"(osekTarget_NestedOsIntMask))

#endif /* #if defined(CONFIG_OSEK_TARGET_MPC555) */

#endif /* #if defined(CONFIG_OSEK_COMPILER_GCC) */

#endif /* defined(CONFIG_OSEK_TARGET_PPC)  */



#if defined(CONFIG_OSEK_TARGET_X86)


/// X86的任务上下文结构，在配置了X86目标机时该结构才存在
typedef struct T_OSEK_TARGET_TaskContext_struct
{
	/// eflags寄存器
	OSDWORD eflags;
	/// sp寄存器
	OSDWORD sp;
	/// ebp寄存器
	OSDWORD ebp;
	/// ebx寄存器
	OSDWORD ebx;
	/// esi寄存器
	OSDWORD esi;
	/// edi寄存器
	OSDWORD edi;
	/// pc寄存器
	OSDWORD pc;
#if defined(CONFIG_OSEK_TARGET_FLOAT)
	/// 浮点寄存器，在配置了浮点处理时该属性才存在
	OSBYTE  fpr[108];
#endif/* #if defined(CONFIG_OSEK_TARGET_FLOAT)  */

}T_OSEK_TARGET_TaskContext_struct;


/// X86的异常框架结构，在配置了X86目标机时该结构才存在
typedef struct T_OSEK_TARGET_ExceptionFrame_struct{
	/// edi寄存器
	OSDWORD  edi;
	/// esi寄存器
	OSDWORD  esi;
	/// ebp寄存器
	OSDWORD  ebp;
	/// esp0寄存器
	OSDWORD  esp0;
	/// ebx寄存器
	OSDWORD  ebx;
	/// edx寄存器
	OSDWORD  edx;
	/// ecx寄存器
	OSDWORD  ecx;
	/// eax寄存器
	OSDWORD  eax;
	/// idtIndex寄存器
	OSDWORD  idtIndex;
	/// faultCode寄存器
	OSDWORD  faultCode;
	/// eip寄存器
	OSDWORD  eip;
	/// cs寄存器
	OSDWORD  cs;
	/// eflags寄存器
	OSDWORD  eflags;
}T_OSEK_TARGET_ExceptionFrame_struct;


#define OSEK_TARGET_SaveBTSP(sp) (osekTarget_SavedBTSP = (sp) - (OSDWORD)0x20)


// 下面的宏用于内核中访问临界区数据时开关中断使用，原则上应该只关闭ISR2类的中断

#if defined(CONFIG_OSEK_COMPILER_GCC)

#define OSEK_TARGET_OSIntSave( osIntSave ) OSDWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave)  asm("pushf"); asm("cli"); asm("popl %0" : "=g"(osIntSave):)
#define OSEK_TARGET_EnableOSInt(osIntSave)   asm("push %0": :"g"(osIntSave));asm("popf")

#define OSEK_TARGET_DisableOSIntWithoutSave()  asm("cli")
#define OSEK_TARGET_EnableOSIntWithoutSave()   asm("sti")

#define OSEK_TARGET_DisableAllInt() asm("pushf");asm("popl osekTarget_AllIntMask");asm("cli")
#define OSEK_TARGET_EnableAllInt()  asm("pushl osekTarget_AllIntMask"); asm("popf")

#define OSEK_TARGET_DisableNestedAllInt() asm("pushf");asm("popl osekTarget_NestedAllIntMask");asm("cli")
#define OSEK_TARGET_EnableNestedAllInt()  asm("pushl osekTarget_NestedAllIntMask"); asm("popf")

#define OSEK_TARGET_DisableNestedOsInt() asm("pushf");asm("popl osekTarget_NestedOsIntMask");asm("cli")
#define OSEK_TARGET_EnableNestedOsInt()  asm("pushl osekTarget_NestedOsIntMask"); asm("popf")

#endif /* #if defined(CONFIG_OSEK_COMPILER_GCC) */


#endif /* defined(CONFIG_OSEK_TARGET_X86)  */

//**********if defined(TARGET_HCS12X)*************
#if defined(CONFIG_OSEK_TARGET_HCS12X)

// 内部函数
OSDWORD osekTarget_SaveContext( void * savedContext );
void osekTarget_RestoreContext( void * restoredContext );
int fun(void * savedContext );

// 下面的函数用于第一次启动扩展任务和基本任务时调整任务的栈
void osekTarget_LoadETSP(void* sp) ;
void osekTarget_LoadBTSP( void );
void osekTarget_SaveBTSP(OSWORD sp );

// 变量声明
extern OSWORD osekTarget_AllIntMask;
extern OSWORD osekTarget_NestedAllIntMask;
extern OSWORD osekTarget_NestedOsIntMask;
extern OSWORD osekTarget_SavedBTSP;


typedef struct T_OSEK_TARGET_TaskContext_struct
{


OSWORD CCR;
OSWORD sp;
OSWORD X;
OSWORD Y;
OSBYTE PAGE;
OSWORD pc;


}T_OSEK_TARGET_TaskContext_struct;  





#define OSEK_TARGET_SaveBTSP(sp) 	  	(osekTarget_SavedBTSP = (sp) - (OSWORD)0x20)
#define osekTarget_LoadBTSP()	  	  	asm(LDS osekTarget_SavedBTSP)
#define osekTarget_LoadETSP(sp)   		{unsigned char*temp=sp;asm(LDS temp);}


#define OSEK_TARGET_OSIntSave( osIntSave ) OSWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave)  {asm PSHCW ;asm PULD ; asm STD osIntSave;DisableInterrupts}
#define OSEK_TARGET_EnableOSInt(osIntSave)   {asm(LDD osIntSave);asm(PSHD);asm(PULCW);}


#define OSEK_TARGET_DisableOSIntWithoutSave()  DisableInterrupts
#define OSEK_TARGET_EnableOSIntWithoutSave()   EnableInterrupts

#define OSEK_TARGET_DisableAllInt()   {asm PSHCW;asm PULD; asm STD osekTarget_AllIntMask ;DisableInterrupts;} 
#define OSEK_TARGET_EnableAllInt() 	  {asm LDD osekTarget_AllIntMask ;asm PSHD ;asm PULCW;} 

#define OSEK_TARGET_DisableNestedAllInt()	{asm(PSHCW);asm(PULD);asm(STD osekTarget_NestedAllIntMask);DisableInterrupts;}
#define OSEK_TARGET_EnableNestedAllInt()    {asm(LDD osekTarget_NestedAllIntMask);asm(PSHD);asm(PULCW);}

#define OSEK_TARGET_DisableNestedOsInt()   {asm(PSHCW);asm(PULD);asm(STD osekTarget_NestedOsIntMask);DisableInterrupts;}
#define OSEK_TARGET_EnableNestedOsInt()    {asm(LDD osekTarget_NestedOsIntMask);asm(PSHD);asm(PULCW);}

/*CLEAR_IPL is used to clear IPL bits when a new task is activated in ISR state*/ 
#define CLEAR_IPL {asm(TFR CCRH,A);asm(ANDA #$F8);asm(TFR A,CCRH);}

#endif 
//**********endif TARGET_HCS12X*************





#ifdef __cplusplus
}
#endif

#endif

