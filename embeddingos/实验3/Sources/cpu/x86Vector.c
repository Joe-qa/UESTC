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
 * @file 	x86Vector.c
 * @brief
 *	<li>功能：实现x86中断向量。 </li>
 *
 */


/**************************** 引用部分 ************************************/
#include "osprop.h"

/*************************** 前向声明部分 *******************************/
#if defined(OCC_PC386)


extern void osekTarget_ExceptionPrologue_0();
extern void osekTarget_ExceptionPrologue_1();
extern void osekTarget_ExceptionPrologue_2();
extern void osekTarget_ExceptionPrologue_3();
extern void osekTarget_ExceptionPrologue_4();
extern void osekTarget_ExceptionPrologue_5();
extern void osekTarget_ExceptionPrologue_6();
extern void osekTarget_ExceptionPrologue_7();
extern void osekTarget_ExceptionPrologue_8();
extern void osekTarget_ExceptionPrologue_9();
extern void osekTarget_ExceptionPrologue_10();
extern void osekTarget_ExceptionPrologue_11();
extern void osekTarget_ExceptionPrologue_12();
extern void osekTarget_ExceptionPrologue_13();
extern void osekTarget_ExceptionPrologue_14();
extern void osekTarget_ExceptionPrologue_16();
extern void osekTarget_ExceptionPrologue_17();
extern void osekTarget_ExceptionPrologue_18();


extern void osekTarget_IsrPrologue_0();
extern void osekTarget_IsrPrologue_1();
extern void osekTarget_IsrPrologue_2();
extern void osekTarget_IsrPrologue_3();
extern void osekTarget_IsrPrologue_4();
extern void osekTarget_IsrPrologue_5();
extern void osekTarget_IsrPrologue_6();
extern void osekTarget_IsrPrologue_7();
extern void osekTarget_IsrPrologue_8();
extern void osekTarget_IsrPrologue_9();
extern void osekTarget_IsrPrologue_10();
extern void osekTarget_IsrPrologue_11();
extern void osekTarget_IsrPrologue_12();
extern void osekTarget_IsrPrologue_13();
extern void osekTarget_IsrPrologue_14();
extern void osekTarget_IsrPrologue_15();
/**************************** 定义部分 ************************************/
typedef void (*BSP_Handler)(void);

/*
static BSP_Handler exceptionEntryTable[] = {
	 osekTarget_ExceptionPrologue_0,
	 osekTarget_ExceptionPrologue_1,
	 osekTarget_ExceptionPrologue_2,
	 osekTarget_ExceptionPrologue_3,
	 osekTarget_ExceptionPrologue_4,
	 osekTarget_ExceptionPrologue_5,
	 osekTarget_ExceptionPrologue_6,
	 osekTarget_ExceptionPrologue_7,
	 osekTarget_ExceptionPrologue_8,
	 osekTarget_ExceptionPrologue_9,
	 osekTarget_ExceptionPrologue_10,
	 osekTarget_ExceptionPrologue_11,
	 osekTarget_ExceptionPrologue_12,
	 osekTarget_ExceptionPrologue_13,
	 osekTarget_ExceptionPrologue_14,
	 osekTarget_ExceptionPrologue_16,
	 osekTarget_ExceptionPrologue_17,
	 osekTarget_ExceptionPrologue_18,
};

*/

static BSP_Handler IsrEntryTable[] = {
osekTarget_IsrPrologue_0,
osekTarget_IsrPrologue_1,
osekTarget_IsrPrologue_2,
osekTarget_IsrPrologue_3,
osekTarget_IsrPrologue_4,
osekTarget_IsrPrologue_5,
osekTarget_IsrPrologue_6,
osekTarget_IsrPrologue_7,
osekTarget_IsrPrologue_8,
osekTarget_IsrPrologue_9,
osekTarget_IsrPrologue_10,
osekTarget_IsrPrologue_11,
osekTarget_IsrPrologue_12,
osekTarget_IsrPrologue_13,
osekTarget_IsrPrologue_14,
osekTarget_IsrPrologue_15
};


/*===================================================================
 *    Interrupt_gate_descriptor
 ===================================================================*/

typedef struct {
  unsigned int low_offsets_bits : 16;
  unsigned int segment_selector : 16;
  unsigned int fixed_value_bits : 8;
  unsigned int gate_type	: 5;
  unsigned int privilege	: 2;
  unsigned int present		: 1;
  unsigned int high_offsets_bits: 16;
}interrupt_gate_descriptor;


/*===================================================================
 *        Segment_descriptors
 ===================================================================*/

typedef struct {
  unsigned int limit_15_0 		: 16;
  unsigned int base_address_15_0	: 16;
  unsigned int base_address_23_16	: 8;
  unsigned int type			: 4;
  unsigned int descriptor_type		: 1;
  unsigned int privilege		: 2;
  unsigned int present			: 1;
  unsigned int limit_19_16		: 4;
  unsigned int available		: 1;
  unsigned int fixed_value_bits		: 1;
  unsigned int operation_size		: 1;
  unsigned int granularity		: 1;
  unsigned int base_address_31_24	: 8;
}segment_descriptors;

/**************************** 实现部分 ************************************/
/**
  * @brief
  *		获取IDT。
  *
  * @param[in]	无。
  *
  *
  * @return		无。
  *
  */
void BSP_GetIdtr (interrupt_gate_descriptor** table, unsigned* limit);
asm(".text");
asm("BSP_GetIdtr:");
asm("movl	4(%esp), %ecx	");    /* get location where table address */
asm("movl	8(%esp), %edx	");    /* get location table size must be stored */
asm("subl	$6, %esp");		    /* let room to prepare 48 bit IDTR */
asm("sidt	(%esp)");		    /* get 48 bit IDTR value */
asm("movl	2(%esp), %eax");	    /* get base */
asm("movl	%eax, (%ecx)");
asm("movzwl	(%esp), %eax");	    /* get limit */
asm("movl	%eax, (%edx)");
asm("addl	$6, %esp");		    /* restore %esp */
asm("ret");

/**
  * @brief
  *		获取CS。
  *
  * @param[in]	无。
  *
  *
  * @return		无。
  *
  */
static inline unsigned short i386_get_cs()
{
  register unsigned short segment = 0;

  asm volatile ( "movw %%cs,%0" : "=r" (segment) : "0" (segment) );

  return segment;
}

/**
  * @brief
  *		创建中断描述符。
  *
  * @param[in]	无。
  *
  *
  * @return		无。
  *
  */
static void create_interrupt_gate_descriptor (interrupt_gate_descriptor* idtEntry,
				       void* hdl)
{
    idtEntry->low_offsets_bits	= (((unsigned) hdl) & 0xffff);
    idtEntry->segment_selector	= i386_get_cs();
    idtEntry->fixed_value_bits	= 0;
    idtEntry->gate_type		= 0xe;
    idtEntry->privilege		= 0;
    idtEntry->present		= 1;
    idtEntry->high_offsets_bits	= ((((unsigned) hdl) >> 16) & 0xffff);
}


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
void  BSP_InitializeVector( )
{
    unsigned int          i;
    interrupt_gate_descriptor  idtEntry,*idt_entry_tbl;
    unsigned int          limit;


    BSP_GetIdtr(&idt_entry_tbl, &limit);



    for (i = 0x20; i < 0x20 + 16; i++)
    {
        create_interrupt_gate_descriptor (&idtEntry, (void*)IsrEntryTable[i-0x20]);
        idt_entry_tbl[i] = idtEntry;
    }


}


#endif


