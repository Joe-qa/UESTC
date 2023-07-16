/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * 修改记录：
 *	20080401	创建该文件。
 *
 */

/**
 * @file 	mpc555Vector.c
 * @brief
 *	<li>功能：实现mpc555中断向量。 </li>
 *
 */


/**************************** 引用部分 ************************************/
#include "osprop.h"

/*************************** 前向声明部分 *******************************/


/**************************** 定义部分 ************************************/
#if defined(OCC_MPC555)



/**************************** 实现部分 ************************************/
/**
  * @brief
  *		初始化中断向量。
  *
  * @param[in]	无。
  *
  *
  * @return		无。
  *
  */
volatile void  BSP_InitializeVector( )
{



}



asm(".equ ISR_SRR0_OFFSET,   0x0");
asm(".equ ISR_SRR1_OFFSET,   0x4");

asm(".equ ISR_GPR0_OFFSET,	0x8");
asm(".equ ISR_GPR2_OFFSET,	0xC");
asm(".equ ISR_GPR3_OFFSET,	0x10");
asm(".equ ISR_GPR4_OFFSET,   0x14");
asm(".equ ISR_GPR5_OFFSET,   0x18");
asm(".equ ISR_GPR6_OFFSET,   0x1C");
asm(".equ ISR_GPR7_OFFSET,   0x20");
asm(".equ ISR_GPR8_OFFSET,   0x24");
asm(".equ ISR_GPR9_OFFSET ,  0x28");
asm(".equ ISR_GPR10_OFFSET , 0x2C");
asm(".equ ISR_GPR11_OFFSET,  0x30");
asm(".equ ISR_GPR12_OFFSET,  0x34");
asm(".equ ISR_GPR13_OFFSET, 0x38");


asm(".equ  ISR_CR_OFFSET ,    0x3C");
asm(".equ  ISR_CTR_OFFSET ,   0x40");
asm(".equ  ISR_XER_OFFSET ,   0x44");
asm(".equ  ISR_LR_OFFSET ,    0x48");

asm(".equ  ISR_GPR29_OFFSET ,    0x4C");
asm(".equ  ISR_MASK_OFFSET ,    0x50");

asm(".equ  ISR_FRAME_SIZE,    0x54");




asm(".macro BSP_SAVE_REGISTERS");
asm("subi 1,1,ISR_FRAME_SIZE");

asm("stw  3,ISR_GPR3_OFFSET(1)");
asm("stw  4,ISR_GPR4_OFFSET(1)");

asm(" mfsrr0  3");
asm(" mfsrr1  4");
asm("stw  3,ISR_SRR0_OFFSET(1)");
asm("stw  4,ISR_SRR1_OFFSET(1)");

asm("stw  0,ISR_GPR0_OFFSET(1)");
asm("stw  2,ISR_GPR2_OFFSET(1)");
asm("stw  5,ISR_GPR5_OFFSET(1)");
asm("stw  6,ISR_GPR6_OFFSET(1)");
asm("stw  7,ISR_GPR7_OFFSET(1)");
asm("stw  8,ISR_GPR8_OFFSET(1)");
asm("stw  9,ISR_GPR9_OFFSET(1)");
asm("stw  10,ISR_GPR10_OFFSET(1)");
asm("stw  11,ISR_GPR11_OFFSET(1)");
asm("stw  12,ISR_GPR12_OFFSET(1)");
asm("stw  13,ISR_GPR13_OFFSET(1)");
asm("stw  29,ISR_GPR29_OFFSET(1)");

asm("mfctr   3");
asm("mfcr    4");
asm("mflr    5");
asm("mfxer   6");
asm("stw     3,ISR_CTR_OFFSET(1)");
asm("stw     4,ISR_CR_OFFSET(1)");
asm("stw     5,ISR_LR_OFFSET(1)");
asm("stw     6,ISR_XER_OFFSET(1)");
asm(".endm");

asm(".macro BSP_RESTORE_REGISTERS");

asm("lwz     3,ISR_CTR_OFFSET(1)");
asm("lwz     4,ISR_CR_OFFSET(1)");
asm("lwz     5,ISR_LR_OFFSET(1)");
asm("lwz     6,ISR_XER_OFFSET(1)");
asm("mtctr   3");
asm("mtcr    4");
asm("mtlr    5");
asm("mtxer   6");


asm("lwz  3,ISR_SRR0_OFFSET(1)");
asm("lwz  4,ISR_SRR1_OFFSET(1)");
asm(" mtsrr0  3");
asm(" mtsrr1  4");

asm("lwz  0,ISR_GPR0_OFFSET(1)");
asm("lwz  2,ISR_GPR2_OFFSET(1)");
asm("lwz  3,ISR_GPR3_OFFSET(1)");
asm("lwz  4,ISR_GPR4_OFFSET(1)");
asm("lwz  5,ISR_GPR5_OFFSET(1)");
asm("lwz  6,ISR_GPR6_OFFSET(1)");
asm("lwz  7,ISR_GPR7_OFFSET(1)");
asm("lwz  8,ISR_GPR8_OFFSET(1)");
asm("lwz  9,ISR_GPR9_OFFSET(1)");
asm("lwz  10,ISR_GPR10_OFFSET(1)");
asm("lwz  11,ISR_GPR11_OFFSET(1)");
asm("lwz  12,ISR_GPR12_OFFSET(1)");
asm("lwz  13,ISR_GPR13_OFFSET(1)");
asm("lwz  29,ISR_GPR29_OFFSET(1)");

asm("addi 1,1,ISR_FRAME_SIZE");
asm("rfi");

asm(".endm");

// fpr0-fpr13,fpscr 共计15个寄存器,每个8字节
asm(".equ  ISR_VOLATILE_FPRS_SIZE,    (8*(14+ 1) + 8)");

asm(".macro BSP_SAVE_VOLATILE_FPRS");
asm("subi 1,1,ISR_VOLATILE_FPRS_SIZE");
asm("mr 3,1");
//8字节对齐
asm("addi	3,3,7");
asm("rlwinm	3,3,0,0,28");

asm("mffs	2");
asm("stfd	2, 0(3)");

asm("stfdu	0,8(3)");
asm("stfdu	1,8(3)");
asm("stfdu	2,8(3)");
asm("stfdu	3,8(3)");
asm("stfdu	4,8(3)");
asm("stfdu	5,8(3)");
asm("stfdu	6,8(3)");
asm("stfdu	7,8(3)");
asm("stfdu	8,8(3)");
asm("stfdu	9,8(3)");
asm("stfdu	10,8(3)");
asm("stfdu	11,8(3)");
asm("stfdu	12,8(3)");
asm("stfdu	13,8(3)");

asm(".endm");



asm(".macro BSP_RESTORE_VOLATILE_FPRS");
asm("mr 3,1");
//8字节对齐
asm("addi	3,3,7");
asm("rlwinm	3,3,0,0,28");


asm("lfd	2, 0(3)");
asm("mtfsf	255, 2");


asm("lfdu	0,8(3)");
asm("lfdu	1,8(3)");
asm("lfdu	2,8(3)");
asm("lfdu	3,8(3)");
asm("lfdu	4,8(3)");
asm("lfdu	5,8(3)");
asm("lfdu	6,8(3)");
asm("lfdu	7,8(3)");
asm("lfdu	8,8(3)");
asm("lfdu	9,8(3)");
asm("lfdu	10,8(3)");
asm("lfdu	11,8(3)");
asm("lfdu	12,8(3)");
asm("lfdu	13,8(3)");

asm("addi 1,1,ISR_VOLATILE_FPRS_SIZE");

asm(".endm");



asm(".equ MPC555_SIMASK ,0x2fc014");
asm(".equ MPC555_SIVEC ,0x2FC01C");


asm(".text");
asm(".align  4");
asm(".global bspIsrHandler");
asm("bspIsrHandler:");

// 保存中断上下文
asm("BSP_SAVE_REGISTERS");

// 保存中断屏蔽码
asm("addis	5, 0, MPC555_SIMASK@ha");
asm("lwz    3, MPC555_SIMASK@l(5)");
asm("stw 3,ISR_MASK_OFFSET(1)");

// 获取中断向量,中断向量必需保存在r4中,因为后面的中断类型判定,用户中断入口程序查找都要用到.
asm("addis	6, 0, MPC555_SIVEC@ha");
asm("lwz    4, MPC555_SIVEC@l(6)");
asm("rlwinm  4,4,6,26,31"); // 转换为中断序号(vector = vector >> 26).

// 设置新的中断屏蔽码,屏蔽掉同级和优先级低的中断(中断序号在r4中,原来的屏蔽码在r3中)
// (~(0xFFFFFFFF >> (vector))) & mask);
asm("li      6,-1");
asm("srw     6,6,4");
asm("andc    6,3,6");
asm("stw     6,MPC555_SIMASK@l(5)");



// 根据配置的中断类型进行跳转,中断序号在r4中
asm("addis	6, 0, osekConfig_InterruptTypeTable@ha");
asm("add 6,6,4");
asm("lbz    5, osekConfig_InterruptTypeTable@l(6)");
asm("cmpwi   5,2");
asm("beq    .isr2");

// ISR1类的中断不进行中断栈切换,直接获取中断处理程序的入口地址(中断序号在r4中),然后跳转到入口执行.
asm(".isr1:");
asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");

asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");

// 恢复中断屏蔽码
asm("lwz 3,ISR_MASK_OFFSET(1)");
asm("addis	6, 0, MPC555_SIMASK@ha");
asm("stw     3, MPC555_SIMASK@l(6)");
asm("b .exit");


// ISR2类中断需要进行栈切换,并且在中断处理程序退出时要进行任务切换的判定处理.
asm(".isr2:");

// 将r1保存在r29中,便于栈切换后的恢复
asm("mr 29,1");

// 判断是否需要进行栈切换
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("cmpwi   7,0");
asm("bne    .nested");

// 还未嵌套则进行切换栈
asm("addis	8, 0, osekInterrupt_IsrStackTop@ha");
asm("lwz	1, osekInterrupt_IsrStackTop@l(8)");

// 对嵌套深度加1,原有的值在r7中
asm(".nested:");
asm("addi 7,7,1");
asm("stw 7,osekInterrupt_NestedLevl@l(6)");

// 获取中断处理程序的入口地址(中断序号在r4中),然后跳转到入口执行
asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");
asm("subi   1,1,0x8 ");
asm("bctrl");

// 屏蔽全局中断:MSR[EE]=0, MSR[RI]=1
asm("mtspr 81, 1");

// 恢复栈
asm("mr  1,29");

// 恢复中断屏蔽码
asm("lwz 3,ISR_MASK_OFFSET(1)");
asm("addis	6, 0, MPC555_SIMASK@ha");
asm("stw     3, MPC555_SIMASK@l(6)");

// 嵌套深度减1
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("subi    7,7,1");
asm("stw     7, osekInterrupt_NestedLevl@l(6)");

// 判定当前是否还有中断嵌套,如果有中断嵌套则不进行任务切换调度处理.
asm("cmpwi   7,0");
asm("bne     .exit");

// 判断是否需要进行任务切换,如果需要则进行重调度
asm("addis	6, 0, osekTask_IsrTriggeredTaskSwitchNecessary@ha");
asm("lwz	7, osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");
asm("cmpwi   7,0");
asm("beq     .exit");

// 重调度主要通过调用osekTask_Dispatch完成
asm(".schedule:");

// 将调度标志清零
asm("xor 7,7,7");
asm("stw 7,osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");

// 使能浮点
asm("mfmsr 2");
asm("ori  2,2,0x2000");
asm("mtmsr   2");

// 保存Volatile的浮点寄存器
asm("BSP_SAVE_VOLATILE_FPRS");


// 进行重调度
asm("subi   1,1,0x8 ");
asm("bl osekTask_Dispatch");
asm("addi 1,1,0x8");


// 恢复Volatile的浮点寄存器
asm("BSP_RESTORE_VOLATILE_FPRS");


// 恢复中断上下文,并且中断返回
asm(".exit:");
asm("BSP_RESTORE_REGISTERS");


asm(".section \".text\",\"ax\" ");

asm(".global __vectors_table_start");
asm("__vectors_table_start:");
asm("VECTOR_BASE:");
/*
************************************************************************
*                                                                      *
*     0100:  SYSTEM RESET                                              *
*                                                                      *
************************************************************************
*/
asm(".skip	0x0100-(.-VECTOR_BASE)");
asm(".global __reset");
asm("__reset:");
asm("	b  _start");


/*
************************************************************************
*                                                                      *
*     0200  Machine Check                                              *
*                                                                      *
************************************************************************
*/

asm(".skip	0x0200-(.-VECTOR_BASE)");
asm("b .");

/*
************************************************************************
*                                                                      *
*     0300:  DATA ACCESS ERROR	                                       *
*                                                                      *
************************************************************************
*/
asm(".skip	0x0300-(.-VECTOR_BASE)");

asm("b .");

/*
************************************************************************
*                                                                      *
*    0400:  INSTRUCTION ACCESS ERROR                                   *
*                                                                      *
************************************************************************
*/

asm(".skip	0x0400-(.-VECTOR_BASE)");

asm("b  .");
/*
************************************************************************
*                                                                      *
*   0500:  EXTERNAL INTERRUPT                                          *
*                                                                      *
************************************************************************
*/
asm(".skip	0x0500-(.-VECTOR_BASE)");

asm(" b bspIsrHandler");




/*
************************************************************************
*                                                                      *
*   0600:  ALIGNMENT ERROR                                             *
*                                                                      *
************************************************************************
*/
asm(".skip	0x0600-(.-VECTOR_BASE)");
asm("b .");
/*
************************************************************************
*                                                                      *
*   0700:  PROGRAM ERROR                                               *
*                                                                      *
************************************************************************
*/
asm(".skip	0x0700-(.-VECTOR_BASE)");
asm("b  .");

/*
************************************************************************
**                                                                     *
** 0800:  FLOATING-POINT UNAVAILABLE                                   *
**                                                                     *
************************************************************************
*/
asm(".skip	0x0800-(.-VECTOR_BASE)");
asm("b .");

/*
************************************************************************
**                                                                     *
**   0900:  DECREMENTER INTERRUPT                                      *
**                                                                     *
************************************************************************
*/

asm(".skip	0x0900-(.-VECTOR_BASE)");
asm("rfi");

/*
************************************************************************
**                                                                     *
**  0x0C00: System Call Interrupt                                      *
**                                                                     *
************************************************************************
*/

asm(".skip	0x0C00-(.-VECTOR_BASE)");
asm("b   .	");
/*
************************************************************************
**                                                                     *
**  0D00:  TRACE                                                       *
**                                                                     *
************************************************************************
*/
asm(".skip	0x0D00-(.-VECTOR_BASE)");
asm("b   .");

/*
************************************************************************
*                                                                      *
*  1000:  INSTRUCTION TRANSLATION MISS                                 *
*                                                                      *
************************************************************************
*/
asm(".skip	0x01000-(.-VECTOR_BASE)");
asm("  b  .");

asm(".skip	0x02000-(.-VECTOR_BASE)");

asm("_vectors_table_end:");


#endif
