/****************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * 修改记录：
 *	20061116	创建该文件。
 *
 */

/**
 * @file 	mpc5554Vector.c
 * @brief
 *	<li>功能：实现mpc5554中断向量。 </li>
 *
 */


/**************************** 引用部分 ************************************/
#include "osprop.h"

/*************************** 前向声明部分 *******************************/


/**************************** 定义部分 ************************************/
#if defined(OCC_MPC5554)


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


/**************************** 实现部分 ************************************/


asm(".macro BSP_SAVE_REGISTERS");
asm("subi 1,1,ISR_FRAME_SIZE");			// r1为SP

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




asm(".equ INTC_IACKR ,0xfff48010");
asm(".equ INTC_EOIR ,0xfff48018");

asm(".text");
asm(".align  4");

asm(".global bspIsrHandler");
asm("bspIsrHandler:");

// 保存中断上下文
asm("BSP_SAVE_REGISTERS");

// 获取中断向量,中断向量必需保存在r4中,因为后面的中断类型判定,用户中断入口程序查找都要用到.
asm("addis	6, 0, INTC_IACKR@ha");
asm("lwz    4, INTC_IACKR@l(6)");
asm("rlwinm  4,4,30,2,31");		// 计算获得INTVEC号 INTVEC>>2


/* 根据配置的中断类型进行跳转,中断序号在r4中 */
asm("addis	6, 0, osekConfig_InterruptTypeTable@ha");
asm("add 6,6,4");
asm("lbz    5, osekConfig_InterruptTypeTable@l(6)");
asm("cmpwi   5,2");
asm("beq    .isr2");

// ISR1类的中断不进行中断栈切换,直接获取中断处理程序的入口地址(中断序号在r4中),然后跳转到入口执行.
asm(".isr1:");

//使能全局中断
asm("wrteei 1");

asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");

asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");

// 中断请求结束标志清零
asm("mbar 0");
asm("lis	3, INTC_EOIR@ha");
asm("li		4, 0x0");

//屏蔽全局中断
asm("wrteei 0");

asm("stw	4, INTC_EOIR@l(3)");
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

//使能全局中断
asm("wrteei 1");

//获取中断处理程序的入口地址(中断序号在r4中),然后跳转到入口执行
asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");
asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");

//恢复栈
asm("mr  1,29");

// 恢复中断屏蔽码
asm("mbar 	0");
asm("lis	3, INTC_EOIR@ha");
asm("li		4, 0x0");

//屏蔽全局中断
asm("wrteei 0");

asm("stw	4, INTC_EOIR@l(3)");

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


// 进行重调度
asm("subi   1,1,0x8 ");
asm("bl osekTask_Dispatch");
asm("addi 1,1,0x8");


// 恢复中断上下文,并且中断返回
asm(".exit:");
asm("BSP_RESTORE_REGISTERS");


// FIT异常处理函数
asm(".text");
asm(".align  4");
asm(".global FITHandler");
asm("FITHandler:");

// 保存中断上下文
asm("BSP_SAVE_REGISTERS");


// 获取中断向量,中断向量必需保存在r4中,因为后面的中断类型判定,用户中断入口程序查找都要用到.
asm("lis 4, 0x0000");
asm("ori 4,4, 0x0134");


/* 根据配置的中断类型进行跳转,中断序号在r4中 */
asm("addis	6, 0, osekConfig_InterruptTypeTable@ha");
asm("add 6,6,4");
asm("lbz    5, osekConfig_InterruptTypeTable@l(6)");
asm("cmpwi   5,2");
asm("beq    .fitisr2");

// ISR1类的中断不进行中断栈切换,直接获取中断处理程序的入口地址(中断序号在r4中),然后跳转到入口执行.
asm(".fitisr1:");

asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");

asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");

asm("b .fitexit");


// ISR2类中断需要进行栈切换,并且在中断处理程序退出时要进行任务切换的判定处理.
asm(".fitisr2:");

// 将r1保存在r29中,便于栈切换后的恢复
asm("mr 29,1");

// 判断是否需要进行栈切换
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("cmpwi   7,0");
asm("bne    .fitnested");

// 还未嵌套则进行切换栈
asm("addis	8, 0, osekInterrupt_IsrStackTop@ha");
asm("lwz	1, osekInterrupt_IsrStackTop@l(8)");

// 对嵌套深度加1,原有的值在r7中
asm(".fitnested:");
asm("addi 7,7,1");
asm("stw 7,osekInterrupt_NestedLevl@l(6)");


//获取中断处理程序的入口地址(中断序号在r4中),然后跳转到入口执行
asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");
asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");


//恢复栈
asm("mr  1,29");

// 嵌套深度减1
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("subi    7,7,1");
asm("stw     7, osekInterrupt_NestedLevl@l(6)");

// 判定当前是否还有中断嵌套,如果有中断嵌套则不进行任务切换调度处理.
asm("cmpwi   7,0");
asm("bne     .fitexit");

// 判断是否需要进行任务切换,如果需要则进行重调度
asm("addis	6, 0, osekTask_IsrTriggeredTaskSwitchNecessary@ha");
asm("lwz	7, osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");
asm("cmpwi   7,0");
asm("beq     .fitexit");

// 重调度主要通过调用osekTask_Dispatch完成
asm(".fitschedule:");

// 将调度标志清零
asm("xor 7,7,7");
asm("stw 7,osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");


// 进行重调度
asm("subi   1,1,0x8 ");
asm("bl osekTask_Dispatch");
asm("addi 1,1,0x8");

// 恢复中断上下文,并且中断返回
asm(".fitexit:");
asm("BSP_RESTORE_REGISTERS");


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
  asm ("lis 5, bspIsrHandler@h");   /* IVPR = address base used with IVOR's */
  asm ("mtIVPR 5 ");

  asm ("lis 5, bspIsrHandler@h"); /* IVOR4 = address of handler */
  asm ("ori 5, 5, bspIsrHandler@l");
  asm ("mtIVOR4 5");

  asm ("lis 0, FITHandler@h"); /* IVOR11 = address of handler */
  asm ("ori 0, 0, FITHandler@l");
  asm ("mtIVOR11 0");

}




#endif	// #if defined(OCC_MPC5554)
