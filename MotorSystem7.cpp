/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
#include<stdio.h>
#include<math.h>
#include"MotorSystem.h"
#include "ManualControl.hpp"
#include"iodefine.h"

MotorSystem *g_hw;

void InitMotorSystem(MotorSystem *);

void CMT_Init(void)
{
	MSTP(CMT0) = 0;
	CMT0.CMCR.BIT.CKS = 2;
	CMT0.CMCR.BIT.CMIE = 1;
	
	IPR(CMT0,CMI0) = 10;
	IEN(CMT0,CMI0) = 1;
	
	CMT0.CMCOR = 3906-1;
	
	CMT.CMSTR0.BIT.STR0 = 1;
}

volatile float g_speed = 0;

#pragma interrupt Logout(vect = VECT(CMT0,CMI0),enable)
void Logout(void)
{
	//PORT2.DR.BIT.B4 = 1;
	g_hw->SetVelocity(g_speed);
	//g_hw->SetDuty(g_speed);
	//g_hw->SetTorque(g_speed);
	g_hw->WDT_Clear();
	//PORT2.DR.BIT.B4 = 0;
}

void main(void)
{
	SEQUENSE_MODE mode = INITIALIZE_MODE;
	static MotorSystem hw;
	g_hw = &hw;
	
	//ManualControl mc(g_hw);
	printf("Hallo World\n");
	while(1){
		//mc.Run();
		//PORT2.DR.BIT.B4 = 1;
		switch(mode){
		case INITIALIZE_MODE:
			//printf("Initialize Start\n");
			InitMotorSystem(&hw);
			//hw.SetMode(TORQUE);
			mode = WAIT_MODE;
			//CMT_Init();
			//printf("Initialize End\n");
			break;
			
		case WAIT_MODE:
			//PORT2.DR.BYTE = 
			//while(MTU0.TSR.BIT.TGFC)MTU0.TSR.BIT.TGFC=0;
			//IR(MTU0,TGIC0) = 0;
			volatile float in=0;
			printf("Duty ?\n");
			scanf("%f",&in);
			g_speed = in;
			
			//hw.Logoutput();
			break;
		default:
			break;
		}
		//PORT2.DR.BIT.B4 = 0;
	}
}

void InitMotorSystem(MotorSystem *hw){
	hw->Begin();
	
	hw->rpc = 3.141592 / 2.0 / 500;
	hw->Vcc = 12;
	hw->Kt = RZ735VA_9517_Kt;
	//hw->Kt = MAXON_RE40_24V_Kt;
	hw->SetDuty(0);
	//6while(!hw->Calibration());
	//���������@�pPID�Q�C��
	/*
	hw->Current_PID.SetPGain(4.2);
	hw->Current_PID.SetIGain(2.0);
	hw->Current_PID.SetDGain(0);
	hw->Velocity_PID.SetPGain(2.0);
	hw->Velocity_PID.SetIGain(2.0);
	hw->Velocity_PID.SetDGain(0);
	*/
	
	//735
	//*/
	hw->Current_PID.SetPGain(16.0);
	hw->Current_PID.SetIGain(8.0);
	hw->Current_PID.SetDGain(50.0*0.000002);
	hw->Velocity_PID.SetPGain(0.35);
	hw->Velocity_PID.SetIGain(0.01);
	hw->Velocity_PID.SetDGain(0.0*0.000002);
	//*/
	
	//540
	/*/
	hw->Current_PID.SetPGain(7.5);
	hw->Current_PID.SetIGain(1.5);
	hw->Current_PID.SetDGain(0.1*0.000002);
	hw->Velocity_PID.SetPGain(1.5);
	hw->Velocity_PID.SetIGain(0.4);
	hw->Velocity_PID.SetDGain(0.0*0.000002);
	//*/
	
	//hw->Velocity_PID.SetPGain(0.10);
	//hw->Velocity_PID.SetIGain(0.01);
	//hw->Velocity_PID.SetDGain(0.00002);
}

#ifdef __cplusplus
extern "C"{void abort(void){}}
#endif