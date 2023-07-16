#ifndef OCC_CFG_H
#define OCC_CFG_H

#define		OSDEFAULTAPPMODE		0


//ISR functions
extern		void		Ir_RealTimInt(void);



//Task IDs

#define		ini_LLD		  0
#define		HLS_ini		  1
#define		HLS_inisyn	2
#define   HLS_ini2    3
#define   ini_LLD2    4
#define   period50    5
#define   period100   6
#define   period200   7
#define   period1000  8
#define   HLS_syn2 9
#define   HLS_rstsyn  10
#define   HLS_firstsyn  11
#define   HLS_syn  12
#define   COM_task1  13
#define   COM_task2  14
#define   COM_task3  15
#define   COM_task4  16

//task functions
//extern	void	FuncTaskError(void);
extern	void	FuncTaskini_LLD(void);
extern	void	FuncTaskHLS_ini(void);
extern	void	FuncTaskHLS_inisyn(void);
extern	void	FuncTaskHLS_ini2(void);
extern	void	FuncTaskini_LLD2(void);
extern	void	FuncTaskperiod50(void);
extern	void	FuncTaskperiod100(void);
extern	void	FuncTaskperiod200(void);
extern	void	FuncTaskperiod1000(void);
extern	void	FuncTaskHLS_syn2(void);
extern	void	FuncTaskHLS_rstsyn(void);
extern	void	FuncTaskHLS_firstsyn(void);
extern	void	FuncTaskHLS_syn(void);
extern	void	FuncTaskCOM_task1(void);
extern	void	FuncTaskCOM_task2(void);
extern	void	FuncTaskCOM_task3(void);
extern	void	FuncTaskCOM_task4(void);
extern  void	FuncIdle(void);




//COUNTER IDs
#define		SysCounter		0


//ALARM configuration lists
#define		TaskAlarm1		&osekAlarm_AlarmTable[0]
#define		TaskAlarm2		&osekAlarm_AlarmTable[1]

#define		TaskAlarm3		&osekAlarm_AlarmTable[2]

#define		TaskAlarm4		&osekAlarm_AlarmTable[3]




#if defined (OCC_LOCALMESSAGESONLY)
//message IDs
#define		Msg1		&osekMsg_MessageTable[0]
#define		Msg2		&osekMsg_MessageTable[1]
#define		Msg3		&osekMsg_MessageTable[2]
#define		Msg4		&osekMsg_MessageTable[3]


#endif  /* OCC_LOCALMESSAGESONLY */




#endif/*	ifndef OCC_CFG_H	*/



