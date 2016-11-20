
#include <stdio.h>
#include "platform.h"
#include "xgpiops.h"
#include "Xscutimer.h"
#include "xscugic.h"
#include "Xil_exception.h"
//#define DEBUG
#include "debug.h"
#include "xparameters.h"

/*Code inspiré du code de TP1*/

/*
 * Boutons-poussoirs 1 accessible par le MIO vers le PS
 * pour usage direct par le ARM.  Les 5 autres BTNU, BTND,
 * BTNC, BTNL et BTNR doivent passer par le PL pour etre utilises.
 */
#define PB1 50
#define LD9 7

/* TODO CHANGER LES VARIABLES*/
static int restartGame;
static int GameStarted = 1;


#define TIMER_LOAD_VALUE  0x28000000

/* variables globales pour les GPIO */
XGpioPs GpioMIO;
int Status;
XGpioPs_Config *GPIO_MIOConfigPtr;


/* variables globales pour le timer */
XScuTimer_Config *TMRConfigPtr;
XScuTimer Timer;

/* variables pour le gestionnaire d'interruptions GIC */
XScuGic InterruptController;
XScuGic_Config *GicConfigPtr;

/******************************************************************************/
/*Fonction du TP1
 * Configuration du GPIO a double-port en plus de celui du Zynq sur MIO
 */
/******************************************************************************/
int configGPIOs () {
  /* en premier, celui accessible par le MIO pour les PB1 et PB2 */
  GPIO_MIOConfigPtr = XGpioPs_LookupConfig( XPAR_PS7_GPIO_0_DEVICE_ID );
  Status = XGpioPs_CfgInitialize(&GpioMIO, GPIO_MIOConfigPtr,GPIO_MIOConfigPtr->BaseAddr );

  if (Status != XST_SUCCESS)
      return XST_FAILURE;

  XGpioPs_SetDirectionPin(&GpioMIO, PB1, 0 );
  XGpioPs_SetDirectionPin(&GpioMIO, LD9, 1 );

  XGpioPs_SetOutputEnablePin(&GpioMIO, PB1, 0 );
  XGpioPs_SetOutputEnablePin(&GpioMIO, LD9, 1 );

  return XST_SUCCESS;
}

/******************************************************************************/
/*Fonction du TP1
* Ajustement de la minuterie "proche" du premier ARM
* et du gestionnaire d'interruption
/******************************************************************************/
void setTimerAndIntr() {
  /* la base avec le timer */
  TMRConfigPtr = XScuTimer_LookupConfig( XPAR_PS7_SCUTIMER_0_DEVICE_ID );
  XScuTimer_CfgInitialize( &Timer,TMRConfigPtr,TMRConfigPtr->BaseAddr );
  XScuTimer_SelfTest(&Timer);

  /* la base avec le gestionnaire d'interruption */
  GicConfigPtr = XScuGic_LookupConfig( XPAR_PS7_SCUGIC_0_DEVICE_ID );
  XScuGic_CfgInitialize(&InterruptController, GicConfigPtr,
					    GicConfigPtr->CpuBaseAddress);
  XScuGic_SelfTest( &InterruptController );

  Xil_ExceptionInit();
  Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler,&InterruptController);
  Xil_ExceptionEnable();

  /* connecter le timer au gestionnaire */
  XScuGic_Connect(&InterruptController, XPAR_SCUTIMER_INTR, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &Timer);

  /* enable the interrupt for the Timer at GIC */
  XScuGic_Enable(&InterruptController, XPAR_SCUTIMER_INTR);

  /* on veut repartir le compteur automatiquement apres chaque interruption */
  XScuTimer_EnableAutoReload( &Timer );
  XScuTimer_EnableInterrupt	( &Timer );

  /* Charger le timer et le partir */
  XScuTimer_LoadTimer(&Timer, TIMER_LOAD_VALUE);
  XScuTimer_Start(&Timer);
}

/******************************************************************************/
/*Initialisation du ZedBoard pour le bouton PB1 et LED9
/******************************************************************************/
void init_ZedBoard(){
	 init_platform_();
	 configGPIOs();
}

/******************************************************************************/
/* Fonction de run appelée dans le main
/******************************************************************************/
void run_ZedBoard()
{
    setTimerAndIntr();

   /* Si PB1 est pessé restart */
	restartGame = XGpioPs_ReadPin( &GpioMIO, PB1 );
	if(restartGame == 1)
	{
		// TODO
		WHERE DBG_PRINT("Restart\n");
		//xil_printf("Restart\n");
	}
	else{
		// TODO
		//DO nothing
		WHERE DBG_PRINT("Do nothing\n");
		//xil_printf("Do nothing\n");
	}


	/* Chercher la valeur de gameStart pour afficher ou pas la LED9  */
   if (GameStarted == 1){
	   XGpioPs_WritePin( &GpioMIO, LD9, 1 );
	   GameStarted = 0;
   }
   else{
	   XGpioPs_WritePin( &GpioMIO, LD9, 0 );
	   GameStarted = 1;
   }

}

/******************************************************************************/
/* A la fin on clean
/******************************************************************************/
void close_ZedBoard()
{
	cleanup_platform();
}
