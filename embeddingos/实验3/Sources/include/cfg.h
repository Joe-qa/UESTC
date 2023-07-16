#ifndef OCC_CFG_H
#define OCC_CFG_H

#define		OSDEFAULTAPPMODE		0


//ISR function
extern		void		Ir_RealTimInt(void);



//Task IDs

#define		Task1		0
#define		Task2		1
#define		Task3		2
#define		Task4		3
#define		Task5		4
#define		Task6		5
#define		Task7		6
#define		Task8		7
#define		Task9		8
#define		Task10	9

//task functions
//extern	void	FuncTaskError(void);
extern	void	FuncTask1(void);
extern	void	FuncTask2(void);
extern	void	FuncTask3(void);
extern	void	FuncTask4(void);
extern	void	FuncTask5(void);
extern	void	FuncTask6(void);
extern	void	FuncTask7(void);
extern	void	FuncTask8(void);
extern	void	FuncTask9(void);
extern	void	FuncTask10(void);
extern  void	FuncIdle(void);


//resource definitions
#define		Resource1		&osekResource_ResourceTable[0]
#define		Resource2		&osekResource_ResourceTable[1]
#define		Resource3		&osekResource_ResourceTable[2]

//COUNTER IDs
#define		SysCounter		0

//ALARM configuration lists
#define		TaskAlarm1		&osekAlarm_AlarmTable[0]
#define		TaskAlarm2		&osekAlarm_AlarmTable[1]
#define		TaskAlarm3		&osekAlarm_AlarmTable[2]
#define		TaskAlarm4		&osekAlarm_AlarmTable[3]
#define		TaskAlarm5		&osekAlarm_AlarmTable[4]
#define		TaskAlarm6		&osekAlarm_AlarmTable[5]
#define		TaskAlarm7		&osekAlarm_AlarmTable[6]
#define		TaskAlarm8		&osekAlarm_AlarmTable[7]





#if defined (OCC_LOCALMESSAGESONLY)

#endif  /* OCC_LOCALMESSAGESONLY */




#endif/*	ifndef OCC_CFG_H	*/



