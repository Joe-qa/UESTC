/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekTarget.c,v $
 * Revision 1.0  2008/03/20 13:37:00
 * 创建文件。
 */

/*
 * @file 	osekTarget.c
 * @brief
 *	<li>功能：实现与具体CPU和目标板相关的功能。</li>
 * @author
 * @date 	2008-03-20
 * <p>部门：
 */


/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"

/*************************** 前向声明部分 ******************************/


/**************************** 定义部分 *********************************/

#if defined(CONFIG_OSEK_TARGET_PPC)

// 全局变量

// 用于记录基本任务的当前可用栈位置
OSDWORD osekTarget_SavedBTSP;

// OS相关中断掩码
OSDWORD osekTarget_OSIntMask;
// 所有中断掩码
OSDWORD osekTarget_AllIntMask;
// 嵌套的所有中断的掩码
OSDWORD osekTarget_NestedAllIntMask;
// 嵌套的OS相关中断的掩码
OSDWORD osekTarget_NestedOsIntMask;


/**************************** 实现部分 *********************************/

#if defined(CONFIG_OSEK_COMPILER_GCC)


/*
 * OSDWORD osekTarget_SaveContext( void * savedContext)
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_SaveContext");
asm("osekTarget_SaveContext:");

// 保存PC(LR)
asm("	mflr    4 ");
asm("stw     4,0(3)");

// 保存栈指针
asm("stwu    1,4(3)");

// 保存r14-r31
asm("stwu    14,4(3)");
asm("stwu    15,4(3)");
asm("stwu    16,4(3)");
asm("stwu    17,4(3)");
asm("stwu    18,4(3)");
asm("stwu    19,4(3)");
asm("stwu    20,4(3)");
asm("stwu    21,4(3)");
asm("stwu    22,4(3)");
asm("stwu    23,4(3)");
asm("stwu    24,4(3)");
asm("stwu    25,4(3)");
asm("stwu    26,4(3)");
asm("stwu    27,4(3)");
asm("stwu    28,4(3)");
asm("stwu    29,4(3)");
asm("stwu    30,4(3)");
asm("stwu    31,4(3)");

#if defined(CONFIG_OSEK_TARGET_FLOAT)
#if defined(CONFIG_OSEK_TARGET_MPC555)
//保存f14-f31
asm("stfdu   14,4(3)");
asm("stfdu   15,8(3)");
asm("stfdu   16,8(3)");
asm("stfdu   17,8(3)");
asm("stfdu   18,8(3)");
asm("stfdu   19,8(3)");
asm("stfdu   20,8(3)");
asm("stfdu   21,8(3)");
asm("stfdu   22,8(3)");
asm("stfdu   23,8(3)");
asm("stfdu   24,8(3)");
asm("stfdu   25,8(3)");
asm("stfdu   26,8(3)");
asm("stfdu   27,8(3)");
asm("stfdu   28,8(3)");
asm("stfdu   29,8(3)");
asm("stfdu   30,8(3)");
asm("stfdu   31,8(3)");
#endif
#endif

// 返回零
asm("li      3,0");
asm("blr");



/*
 * void osekTarget_RestoreContext( void * restoredContext )
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_RestoreContext");
asm("osekTarget_RestoreContext:");

// 恢复LR
asm("lwz     4,0(3)");
asm("mtlr    4");

// 恢复栈指针
asm("lwzu    1,4(3)");

// 恢复r14-r31
asm("lwzu    14,4(3)");
asm("lwzu    15,4(3)");
asm("lwzu    16,4(3)");
asm("lwzu    17,4(3)");
asm("lwzu    18,4(3)");
asm("lwzu    19,4(3)");
asm("lwzu    20,4(3)");
asm("lwzu    21,4(3)");
asm("lwzu    22,4(3)");
asm("lwzu    23,4(3)");
asm("lwzu    24,4(3)");
asm("lwzu    25,4(3)");
asm("lwzu    26,4(3)");
asm("lwzu    27,4(3)");
asm("lwzu    28,4(3)");
asm("lwzu    29,4(3)");
asm("lwzu    30,4(3)");
asm("lwzu    31,4(3)");

#if defined(CONFIG_OSEK_TARGET_FLOAT)
#if defined(CONFIG_OSEK_TARGET_MPC555)
// 恢复f14-f31
asm("lfdu   14,4(3)");
asm("lfdu   15,8(3)");
asm("lfdu   16,8(3)");
asm("lfdu   17,8(3)");
asm("lfdu   18,8(3)");
asm("lfdu   19,8(3)");
asm("lfdu   20,8(3)");
asm("lfdu   21,8(3)");
asm("lfdu   22,8(3)");
asm("lfdu   23,8(3)");
asm("lfdu   24,8(3)");
asm("lfdu   25,8(3)");
asm("lfdu   26,8(3)");
asm("lfdu   27,8(3)");
asm("lfdu   28,8(3)");
asm("lfdu   29,8(3)");
asm("lfdu   30,8(3)");
asm("lfdu   31,8(3)");
#endif
#endif

// 返回1
asm("li      3,1");
asm("blr");



/*
 * void osekTarget_LoadETSP(void* sp)
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_LoadETSP");
asm("osekTarget_LoadETSP:");
asm("mr 1,3");
asm("blr");


/*
 * void osekTarget_SaveBTSP(OSDWORD sp )
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_SaveBTSP");
asm("osekTarget_SaveBTSP:");
asm("subi 3,3,0x20");
asm("mtsprg 0,3 ");
asm("blr");


/*
 * void osekTarget_LoadBTSP( void )
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_LoadBTSP");
asm("osekTarget_LoadBTSP:");
asm("mfsprg 1,0");
asm("blr");


#endif /* #if defined(CONFIG_OSEK_COMPILER_GCC) */


#endif /* #if defined(CONFIG_OSEK_TARGET_PPC) */



/**************************** 定义部分 *********************************/

#if defined(CONFIG_OSEK_TARGET_X86)

// 全局变量

// 用于记录基本任务的当前可用栈位置
OSDWORD osekTarget_SavedBTSP;

// OS相关中断掩码
OSDWORD osekTarget_OSIntMask;
// 所有中断掩码
OSDWORD osekTarget_AllIntMask;
// 嵌套的所有中断的掩码
OSDWORD osekTarget_NestedAllIntMask;
// 嵌套的OS相关中断的掩码
OSDWORD osekTarget_NestedOsIntMask;


/**************************** 实现部分 *********************************/

#if defined(CONFIG_OSEK_COMPILER_GCC)


/*
 * OSDWORD osekTarget_SaveContext( void *context)
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_SaveContext");
asm("osekTarget_SaveContext:");
asm("movl 4(%esp),%eax");
asm("pushf");
asm("popl 0(%eax)");
asm("movl %esp,4(%eax)");
asm("movl %ebp,8(%eax)");
asm("movl %ebx,0xc(%eax)");
asm("movl %esi,0x10(%eax)");
asm("movl %edi,0x14(%eax)");
asm("movl (%esp),%edi");
asm("movl %edi,0x18(%eax)");
#if defined(CONFIG_OSEK_TARGET_FLOAT)
asm("fsave 0x1c(%eax)");
#endif
asm("movl $0,%eax");
asm("ret");



/*
 * void osekTarget_RestoreContext( void *context )
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_RestoreContext");
asm("osekTarget_RestoreContext:");
asm("movl 4(%esp),%eax");
asm("pushl 0(%eax)");
asm("popf ");
asm("movl 4(%eax),%esp");
asm("movl 8(%eax),%ebp");
asm("movl 0xc(%eax),%ebx");
asm("movl 0x10(%eax),%esi");
asm("movl 0x14(%eax),%edi");
asm("addl $4,%esp");
asm("pushl 0x18(%eax)");
#if defined(CONFIG_OSEK_TARGET_FLOAT)
asm("frstor 0x1c(%eax)");
#endif
asm("movl $1,%eax");
asm("ret");



/*
 * void osekTarget_LoadETSP(void* sp)
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_LoadETSP");
asm("osekTarget_LoadETSP:");
asm("movl (%esp),%ecx");
asm("movl 4(%esp),%esp");
asm("movl %esp,%ebp");
asm("pushl %ecx");
asm("ret");


/*
 * void osekTarget_LoadBTSP( void )
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_LoadBTSP");
asm("osekTarget_LoadBTSP:");
asm("movl (%esp),%ecx");
asm("movl osekTarget_SavedBTSP,%esp");
asm("movl %esp,%ebp");
asm("pushl %ecx");
asm("ret");


/*
 * void osekTarget_IsrHandler( void )
 * 注意：在进入本函数时ecx保存在当前栈中，并且ecx的当前值为中断向量号
 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_IsrHandler");
asm("osekTarget_IsrHandler:");

// 继续保存需要保存的上下文
asm("pushl %eax");
asm("pushl %edx");

// 设置中断屏蔽码
asm(".equ PIC_MASTER_IMR_IO_PORT ,0x21");
asm(".equ PIC_SLAVE_IMR_IO_PORT,0xa1");
asm(".equ PIC_MASTER_COMMAND_IO_PORT,0x20");
asm(".equ PIC_SLAVE_COMMAND_IO_PORT, 0xa0");

asm(".equ PIC_EOI,0x20");

// 保存原来的屏蔽码
asm("movw	BSP_i8259Mask, %ax ");
asm("pushl	%eax	");

// 根据中断向量计算新的屏蔽码
asm("movw	BSP_i8259MaskTable(,%ecx,2), %dx");
asm("orw	%dx, %ax");

// 设置新的中断屏蔽码
asm("movw   %ax, BSP_i8259Mask");
asm("outb	$PIC_MASTER_IMR_IO_PORT");
asm("movb	%ah, %al");
asm("outb	$PIC_SLAVE_IMR_IO_PORT");

// 中断应答，如果是从片中断还需要应答从片，但主片必需应答
asm("movb	$PIC_EOI, %al");
asm("cmpl   $7, %ecx");
asm("jbe	.master");
asm("outb   $PIC_SLAVE_COMMAND_IO_PORT");
asm(".master:");
asm("outb   $PIC_MASTER_COMMAND_IO_PORT");

// 根据配置的中断类型进行跳转
asm("movb	osekConfig_InterruptTypeTable(%ecx),%al");
asm("cmpb	$1,%al");
asm("je		.isr1");

// 将当前栈的栈指针保存到ebp中
asm("pushl	%ebp");
asm("movl   %esp,%ebp");

// 判定是否嵌套，如果嵌套则不再进行中断栈的切换
asm("cmpl   $0, osekInterrupt_NestedLevl ");
asm("jne    .nested ");
asm("movl   osekInterrupt_IsrStackTop, %esp");

// 使嵌套深度加1
asm(".nested:");
asm("incl osekInterrupt_NestedLevl");
asm("sti");
asm("movl osekConfig_InterruptEntryTable(,%ecx,4),%eax");
asm("subl $0xc,%esp");
asm("call *%eax");
asm("cli");

// 恢复栈指针
asm("movl	%ebp,%esp");
asm("popl	%ebp");

// 恢复中断屏蔽码
asm("popl	%eax");
asm("movw	%ax, BSP_i8259Mask");
asm("outb	$PIC_MASTER_IMR_IO_PORT");
asm("movb	%ah, %al");
asm("outb	$PIC_SLAVE_IMR_IO_PORT");

// 中断不嵌套并且需要进行切换时进行任务切换
asm("decl	osekInterrupt_NestedLevl");
asm("jne    .exit");
asm("cmpb   $0,osekTask_IsrTriggeredTaskSwitchNecessary");
asm("jne	.schedule");
asm("jmp	.exit");

// 进行任务切换调度
asm(".schedule:");
asm("movb	$0,osekTask_IsrTriggeredTaskSwitchNecessary");
asm("subl	$0xc,%esp");
asm("call	osekTask_Dispatch");
asm("addl	$0xc,%esp");
asm("jmp	.exit");

asm(".isr1:");
asm("movl	osekConfig_InterruptEntryTable(,%ecx,4),%eax");
asm("subl	$0xc,%esp");
asm("call	*%eax");
asm("addl	$0xc,%esp");

// 恢复中断屏蔽码
asm(".restorePIC:");
asm("popl	%eax");
asm("movw	%ax, BSP_i8259Mask");
asm("outb	$PIC_MASTER_IMR_IO_PORT");
asm("movb	%ah, %al");
asm("outb	$PIC_SLAVE_IMR_IO_PORT");

// 恢复中断上下文并退出中断
asm(".exit:");
asm("popl	%edx");
asm("popl	%eax");
asm("popl	%ecx");
asm("iret");



/* 外部中断向量 */

asm(".macro OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY  vector ");
asm("	.p2align 4 ");
asm("   .global osekTarget_IsrPrologue_\\vector ");
asm("	osekTarget_IsrPrologue_\\vector: ");
asm("	pushl   %ecx ");
asm("	movl    $\\vector, %ecx ");
asm("	jmp   osekTarget_IsrHandler ");
asm(".endm");

asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 0");

asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 1");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 2");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 3");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 4");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 5");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 6");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 7");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 8");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 9");

asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 10");

asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 11");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 12");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 13");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 14");
asm("OSEK_TARGET_DISTINCT_INTERRUPT_ENTRY 15");



/* 异常处理部分 */

asm(".text");
asm(".align  4");
asm(".global osekTarget_ExceptionHandler");
asm("	osekTarget_ExceptionHandler:");
asm("	pusha	");
asm("	pushl	%esp	");
asm("	call	osekTarget_ExceptionMonitor");
asm("	addl	$4, %esp");
asm("	popa	");
asm("	addl	$8, %esp	");
asm("	iret");



/* 异常向量 */

asm(".macro OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY vector ");
asm("	.p2align 4 ");
asm("	.global osekTarget_ExceptionPrologue_\\vector	");
asm("	osekTarget_ExceptionPrologue_\\vector:	");
asm("	pushl	$\\vector	");
asm("	jmp   osekTarget_ExceptionHandler");
asm(".endm");


asm(".macro OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY vector ");
asm("	.p2align 4 ");
asm("	.global osekTarget_ExceptionPrologue_\\vector	");
asm(" 	osekTarget_ExceptionPrologue_\\vector:	");
asm("	pushl	$0	");
asm("	pushl	$\\vector	");
asm("  	jmp   osekTarget_ExceptionHandler");
asm(".endm");


/*
 * Divide Error
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 0");

/*
 * Debug Exception
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 1");

/*
 * NMI
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 2");

/*
 * Breakpoint
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 3");

/*
 * Overflow
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 4");

/*
 * Bound Range Exceeded
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 5");

/*
 * Invalid Opcode
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 6");

/*
 * No Math Coproc
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 7");

/*
 * Double Fault
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 8");

/*
 * Coprocessor segment overrun
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 9");

/*
 * Invalid TSS
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 10");

/*
 * Segment Not Present
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 11");

/*
 * Stack segment Fault
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 12");

/*
 * General Protection Fault
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 13");

/*
 * Page Fault
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 14");

/*
 * Floating point error (NB 15 is reserved it is therefor skipped)
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY 16");

/*
 * Aligment Check
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 17");

/*
 * Machine Check
 */
asm("OSEK_TARGET_DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY 18");



void osekTarget_ExceptionMonitor(T_OSEK_TARGET_ExceptionFrame *exceptionFrame)
{
	while(1)
	{
	}
}


#endif /* #if defined(CONFIG_OSEK_COMPILER_GCC) */


#endif /* #if defined(CONFIG_OSEK_TARGET_X86) */

/*************************** 前向声明部分 ******************************/
#if defined(CONFIG_OSEK_TARGET_HCS12X)

#include <mc9s12xep100.h>
// 全局变量
// 用于记录基本任务的当前可用栈位置
OSWORD osekTarget_SavedBTSP;

// OS相关中断掩码
OSWORD osekTarget_OSIntMask;
// 所有中断掩码
OSWORD osekTarget_AllIntMask;
// 嵌套的所有中断的掩码
OSWORD osekTarget_NestedAllIntMask;
// 嵌套的OS相关中断的掩码
OSWORD osekTarget_NestedOsIntMask;



#pragma CODE_SEG NON_BANKED
OSDWORD osekTarget_SaveContext( void *context) 
  {

  
   __asm {

XGDX       ;       
STD 4,X    ;   //store x
TFR CCRW,D ;
STD 0,x    ;   //store  ccr
STY 6,X    ;   //store y

LDAA 0,SP  ;
STAA 8,X   ;  //store  ppage
LDD 1,SP   ;
STD 9,X    ;  //store pc
LEAY 3,SP  ;
STY  2,X   ;  //store sp
   
   }
   
return 0;
                  
 }
  
  
void osekTarget_RestoreContext( void *context )
  {
   
    __asm {

XGDX;

LDD 0,X ;
TFR D,CCRW;    //resore ccr
LDS 2,X;       //resore sp
LDY 6,X  ;     //resore y
LDD  9,X ;
PSHD     ;     //restore pc
LDAA 8,X ;
PSHA     ;     //restore ppage
LDX 4,x  ;     //resore x

//write 1 to register D (used in dispatch func) 
LDD #$1  ;

RTC      ;
  } 
  
  
  } 
  
  
void int_mask_save(unsigned int *value) {

  *value=1;
  
 

}
void int_mask_restore(unsigned int value){
 if(value==66) {  
    PITINTE=0;
 }else {
    PITINTE=1;
 }
    
}
void int_fit_restore(unsigned int value){

}
void int_vector_save(unsigned int *value){
  
}

#pragma CODE_SEG DEFAULT

#endif //#if defined(CONFIG_OSEK_TARGET_HCS12X)
