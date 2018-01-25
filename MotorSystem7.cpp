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
	//g_hw->SetVelocity(g_speed);
	//g_hw->SetDuty(g_speed);
	//g_hw->SetTorque(g_hw->CurrentToTorque(g_speed));
	g_hw->Logoutput();
	//g_hw->WDT_Clear();
	//PORT2.DR.BIT.B4 = 0;
}

void main(void)
{
	SEQUENSE_MODE mode = INITIALIZE_MODE;
	MotorSystem hw;
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
			hw.Begin();
			//hw.SetMode(VELOCITY);
			mode = WAIT_MODE;
			CMT_Init();
			//printf("Initialize End\n");
			break;
			
		case WAIT_MODE:
			//PORT2.DR.BYTE = 
			//while(MTU0.TSR.BIT.TGFC)MTU0.TSR.BIT.TGFC=0;
			//IR(MTU0,TGIC0) = 0;
			volatile float in=0;
			//printf("Duty ?\n");
			//hw.Logoutput();
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

#define Maxon_Profile	0
#define RZ735_Profile	1

#define MotorProfile	RZ735_Profile
void InitMotorSystem(MotorSystem *hw){
	
	hw->rpc = 3.141592 / 2.0 / 500;
	hw->Vcc = 12;
	//*
	
	hw->velocity_limit = 300;
	hw->current_limit = 15;
	
	hw->static_friction = hw->CurrentToTorque(0.0);
	hw->dynamic_friction = hw->CurrentToTorque(0.0);
	hw->friction_velocity_threshold = 0.01;
	
#if	MotorProfile == Maxon_Profile
	//«ñèÀ±@pPIDQC
	//*
	hw->Kt = MAXON_RE40_24V_Kt;
	hw->gpt.SetFrequency(100);		//PWMüg100kHz
	hw->mtu0.SetFrequency(1000);		//¬x§äüg1000Hz(1kHz)
	hw->mtu2.SetFrequency(10000);		//d¬§äüg10000Hz(10kHz)
	
	hw->Current_PID.SetK(4.5);
	hw->Current_PID.SetTi(0.0);
	hw->Current_PID.SetTd(0);
	hw->Current_PID.Setdt( 1.0 / 10000.0);
	hw->Velocity_PID.SetK(3.0);
	hw->Velocity_PID.SetTi(0.2);
	hw->Velocity_PID.SetTd(0);
	hw->Velocity_PID.Setdt(1.0 / 1000.0 );
	//*/
	
	//735
	//*/
#elif	MotorProfile == RZ735_Profile

	hw->Kt = RZ735VA_8519_Kt;
	hw->gpt.SetFrequency(40);
	
	hw->Current_PID.SetK(4.0);
	hw->Current_PID.SetTi(0.05);
	hw->Current_PID.SetTd(0.0);
	hw->Velocity_PID.SetK(1.0);
	hw->Velocity_PID.SetTi(0.2);
	hw->Velocity_PID.SetTd(0.0001);
	//*/
#endif
}

#ifdef __cplusplus
extern "C"{void abort(void){}}
#endif
