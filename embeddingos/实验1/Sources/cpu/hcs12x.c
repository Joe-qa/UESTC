#if defined(CONFIG_OSEK_TARGET_HCS12X)

#include "MC9S12XEP100.h"
#include "termio.c"

#pragma CODE_SEG NON_BANKED 
#define SYNR_VALUE 2
#define REFDV_VALUE 1
#define POST_VALUE 1

extern OSBYTE osekConfig_InterruptTypeTable[];
extern T_OSEK_TASK_Entry	osekConfig_InterruptEntryTable[];
extern OSWORD osekInterrupt_IsrStackTop;

OSWORD temp_sp;
void PLL_Init();

void BSP_TimerInit(void) {
   /*We choose timer0 of HCS12XEP100,
   1)We use PLL to change clock to 12MHz,
   and bus clock is 6MHz
   choose PIT channel0,choose microtimer0,
   open channel0 interrupt,set interrupt period 1ms*/ 
    PLL_Init();
    PITCFLMT=0;  
    PITCE=0x1;   
    PITMUX=0;
    PITINTE=0x1;
    PITMTLD0= 200-1;
    PITLD0=30-1;
    PITCFLMT=0x80;
}


static void clearIntFlag(int num){

 	//clear interrupt flag
 	switch(num) {
 	
 	  //clear PIT flag
 	  case 66:PITTF|=0x1;break;
 	  case 67:PITTF|=0x2;break;
 	  case 68:PITTF|=0x4;break;
 	  case 69:PITTF|=0x8;break;
 	  case 80:PITTF|=0x10;break;
 	  case 81:PITTF|=0x20;break;
 	  case 82:PITTF|=0x40;break;
 	  case 83:PITTF|=0x80;break;
 	  //clear Enhanced Capture Timer Interrupt Flag
 	  case 8:ECT_TFLG1|=0x1;break;
 	  case 9:ECT_TFLG1|=0x2;break;
 	  case 10:ECT_TFLG1|=0x4;break;
 	  case 11:ECT_TFLG1|=0x8;break;
 	  case 12:ECT_TFLG1|=0x10;break;
 	  case 13:ECT_TFLG1|=0x20;break;
 	  case 14:ECT_TFLG1|=0x40;break;
 	  case 15:ECT_TFLG1|=0x80;break;
 	  case 16:ECT_TFLG2|=0x80;break;
 	  case 17:ECT_PAFLG|=0x2;break;
 	  case 18:ECT_PAFLG!=0x1;break;
 	  //clear Timer Interrupt Flag
 	  case 85:TIM_TFLG1|=0x1;break;
 	  case 86:TIM_TFLG1|=0x2;break;
 	  case 87:TIM_TFLG1|=0x4;break;
 	  case 88:TIM_TFLG1|=0x8;break;
 	  case 89:TIM_TFLG1|=0x10;break;
 	  case 90:TIM_TFLG1|=0x20;break;
 	  case 91:TIM_TFLG1|=0x40;break;
 	  case 92:TIM_TFLG1|=0x80;break;
 	  case 93:TIM_TFLG2|=0x80;break;
 	  case 94:TIM_PAFLG|=0x2;break;
 	  case 95:TIM_PAFLG!=0x1;break;
 	  //clear SCI Interrupt Flags
 	  
 	  	
 	}
}
void isrHandler(unsigned char num)
{

  clearIntFlag(num);
 	if(osekConfig_InterruptTypeTable[num]==1)
	{
	
		(*osekConfig_InterruptEntryTable[num])();
				
		
	}
	
	else if(osekConfig_InterruptTypeTable[num] ==2)
	{

		__asm
		{
				LDAA num                                //num->A
				LDAB #3                                //3->B
				MUL                                   //A*B->A:B
				ADDD #osekConfig_InterruptEntryTable  //D+M(osekConfig_InterruptEntryTable,osekConfig_InterruptEntryTable+1)->D
				TFR D,Y                               //D->Y
				
				//if(osekInterrupt_NestedLevl==0) then change stack pointer
				LDD   osekInterrupt_NestedLevl:2
				BNE   Nest    ////如果  osekInterrupt_NestedLevl 不为0 跳转到Nest
				LDD   osekInterrupt_NestedLevl
				BNE   Nest
				STS temp_sp       //SP->M:M+1（temp_sp）
				LDS	osekInterrupt_IsrStackTop    //将中断栈指针入sp
				
				//osekInterrupt_NestedLevl++;
Nest:		LDD   osekInterrupt_NestedLevl:2
				LDX   osekInterrupt_NestedLevl
				INCB
				STD   osekInterrupt_NestedLevl:2
				STX   osekInterrupt_NestedLevl
				
		}
		//Call ISR to process interrupt request
    EnableInterrupts;
		__asm(CALL [0,Y]);                 //调用中断处理函数
		DisableInterrupts;
		
		osekInterrupt_NestedLevl--;      
		
		//if NestedLevel is 0,we should restore task sp
		if(osekInterrupt_NestedLevl==0) 
		{
  		  __asm(LDS temp_sp);            ////回复sp指针
		}
		//NestedLevel is 0 and TaskSwitch is needed
		if((osekInterrupt_NestedLevl==0)
		&&(osekTask_IsrTriggeredTaskSwitchNecessary!=0))
		{
			/*clear TaskSwich varible*/
			osekTask_IsrTriggeredTaskSwitchNecessary=0;
			osekTask_Dispatch();

		}
			
  }
}


void BSP_UartInit(void) {
  
  SCI.SCxBDH=(unsigned char)(39>>8);
  SCI.SCxBDL=(unsigned char)39;
  SCI.SCxCR2=0x0C;
}

//晶振频率8M，设置PLL频率12M，总线频率6M
void PLL_Init() 
{
    CLKSEL_PLLSEL = 0x0;						
    REFDV = REFDV_VALUE;				// 选择分频以及倍频系数
    SYNR = SYNR_VALUE;
    POSTDIV = POST_VALUE;
    while(!CRGFLG_LOCK);			    //等待锁定
    CLKSEL_PLLSEL = 0x1;				// 选择锁相环时钟为系统时钟    
}

#pragma CODE_SEG NON_BANKED   
#define HCS12X_INT_VECTOR(num)   interrupt num void HCS12X_ISR_##num(void){ isrHandler(num);}

HCS12X_INT_VECTOR(5)
HCS12X_INT_VECTOR(6)
HCS12X_INT_VECTOR(7)
HCS12X_INT_VECTOR(8)
HCS12X_INT_VECTOR(9)
HCS12X_INT_VECTOR(10)
HCS12X_INT_VECTOR(11)
HCS12X_INT_VECTOR(12)
HCS12X_INT_VECTOR(13)
HCS12X_INT_VECTOR(14)
HCS12X_INT_VECTOR(15)
HCS12X_INT_VECTOR(16)
HCS12X_INT_VECTOR(17)
HCS12X_INT_VECTOR(18)
HCS12X_INT_VECTOR(19)
HCS12X_INT_VECTOR(20)
HCS12X_INT_VECTOR(21)
HCS12X_INT_VECTOR(22)
HCS12X_INT_VECTOR(23)
HCS12X_INT_VECTOR(24)
HCS12X_INT_VECTOR(25)
HCS12X_INT_VECTOR(26)
HCS12X_INT_VECTOR(27)
HCS12X_INT_VECTOR(28)
HCS12X_INT_VECTOR(29)
HCS12X_INT_VECTOR(30)
HCS12X_INT_VECTOR(31)
HCS12X_INT_VECTOR(32)
HCS12X_INT_VECTOR(33)
HCS12X_INT_VECTOR(34)
HCS12X_INT_VECTOR(35)
HCS12X_INT_VECTOR(36)
HCS12X_INT_VECTOR(37)
HCS12X_INT_VECTOR(38)
HCS12X_INT_VECTOR(39)
HCS12X_INT_VECTOR(40)
HCS12X_INT_VECTOR(41)
HCS12X_INT_VECTOR(42)
HCS12X_INT_VECTOR(43)
HCS12X_INT_VECTOR(44)
HCS12X_INT_VECTOR(45)
HCS12X_INT_VECTOR(46)
HCS12X_INT_VECTOR(47)
HCS12X_INT_VECTOR(48)
HCS12X_INT_VECTOR(49)
HCS12X_INT_VECTOR(50)
HCS12X_INT_VECTOR(51)
HCS12X_INT_VECTOR(52)
HCS12X_INT_VECTOR(53)
HCS12X_INT_VECTOR(54)
HCS12X_INT_VECTOR(55)
HCS12X_INT_VECTOR(56)
HCS12X_INT_VECTOR(57)
HCS12X_INT_VECTOR(58)
HCS12X_INT_VECTOR(59)
HCS12X_INT_VECTOR(60)
HCS12X_INT_VECTOR(61)
HCS12X_INT_VECTOR(62)
HCS12X_INT_VECTOR(63)
HCS12X_INT_VECTOR(64)
HCS12X_INT_VECTOR(65)
HCS12X_INT_VECTOR(66)
HCS12X_INT_VECTOR(67)
HCS12X_INT_VECTOR(68)
HCS12X_INT_VECTOR(69)
HCS12X_INT_VECTOR(70)
HCS12X_INT_VECTOR(71)
HCS12X_INT_VECTOR(72)
HCS12X_INT_VECTOR(73)
HCS12X_INT_VECTOR(74)
HCS12X_INT_VECTOR(75)
HCS12X_INT_VECTOR(76)
HCS12X_INT_VECTOR(77)
HCS12X_INT_VECTOR(78)
HCS12X_INT_VECTOR(79)
HCS12X_INT_VECTOR(80)
HCS12X_INT_VECTOR(81)
HCS12X_INT_VECTOR(82)
HCS12X_INT_VECTOR(83)
HCS12X_INT_VECTOR(84)
HCS12X_INT_VECTOR(85)
HCS12X_INT_VECTOR(86)
HCS12X_INT_VECTOR(87)
HCS12X_INT_VECTOR(88)
HCS12X_INT_VECTOR(89)
HCS12X_INT_VECTOR(90)
HCS12X_INT_VECTOR(91)
HCS12X_INT_VECTOR(92)
HCS12X_INT_VECTOR(93)
HCS12X_INT_VECTOR(94)
HCS12X_INT_VECTOR(95)
HCS12X_INT_VECTOR(96)
HCS12X_INT_VECTOR(97)
HCS12X_INT_VECTOR(98)
HCS12X_INT_VECTOR(99)
HCS12X_INT_VECTOR(100)
HCS12X_INT_VECTOR(101)
HCS12X_INT_VECTOR(102)
HCS12X_INT_VECTOR(103)
HCS12X_INT_VECTOR(104)
HCS12X_INT_VECTOR(105)
HCS12X_INT_VECTOR(106)
HCS12X_INT_VECTOR(107)
HCS12X_INT_VECTOR(108)
HCS12X_INT_VECTOR(109)
HCS12X_INT_VECTOR(110)
HCS12X_INT_VECTOR(111)
HCS12X_INT_VECTOR(112)
HCS12X_INT_VECTOR(113)
HCS12X_INT_VECTOR(114)
HCS12X_INT_VECTOR(115)
HCS12X_INT_VECTOR(116)
HCS12X_INT_VECTOR(117)
HCS12X_INT_VECTOR(118)
HCS12X_INT_VECTOR(119)

#pragma CODE_SEG DEFAULT

#endif //#if defined(CONFIG_OSEK_TARGET_HCS12X)
 
 