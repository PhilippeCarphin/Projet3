
#include <stdio.h>
#include "platform.h"
#include "xgpiops.h"
#include "Xscutimer.h"
#include "xscugic.h"
#include "Xil_exception.h"
#include "test_hdmi.h"
#include "test_http.h"
//#define DEBUG
//#define TESTING
#include "debug.h"
#include "xparameters.h"
#include "chessboard.h"
#include "test_check.h"

//Code inspiré du code de TP1

/*
 * Boutons-poussoirs 1 accessible par le MIO vers le PS
 * pour usage direct par le ARM.  Les 5 autres BTNU, BTND,
 * BTNC, BTNL et BTNR doivent passer par le PL pour etre utilises.
 */
#define PB1 50
#define LD9 7

static int currentButtonValue;
static int oldButtonValue = 0;
static int flag = 0;

// variables globales pour les GPIO
XGpioPs GpioMIO;
int Status;
XGpioPs_Config *GPIO_MIOConfigPtr;

/*******************************************************************************
 *Fonction du TP1
 * Configuration du GPIO a double-port en plus de celui du Zynq sur MIO
 *
*******************************************************************************/
int configGPIOs () {

  // en premier, celui accessible par le MIO pour les PB1 et PB2
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

/*******************************************************************************
 *Initialisation du ZedBoard pour le bouton PB1 et LED9
*******************************************************************************/
void init_ZedBoard(){
	 //init_platform_();
	configGPIOs();

}

/*******************************************************************************
 * Fonction de run appelée dans le main
*******************************************************************************/
void run_ZedBoard()
{
    // Valeur du bouton PB1
    currentButtonValue = XGpioPs_ReadPin( &GpioMIO, PB1 );

	if(oldButtonValue!=currentButtonValue)
	{
		flag = 1;
	}

	if(currentButtonValue && flag)
	{
#ifdef TESTING
		//test_hdmi();
		//test_full_promotion();
		//test_en_passant();
		test_castling2();
#else

		// TODO RESTART TABLETTE
		reset_game();
		WHERE DBG_PRINT("Restart\n");

#endif
	}
	oldButtonValue = currentButtonValue;
	flag = 0;
}

/*******************************************************************************
 * Fonction appelée lors du debut de la game pour allumé la led LED9
*******************************************************************************/
void start_game_led()
{
	 XGpioPs_WritePin( &GpioMIO, LD9, 1 );
}

/*******************************************************************************
 * Fonction appelée lors de la fin de la game pour éteindre la led LED9
*******************************************************************************/
void end_game_led()
{
	 XGpioPs_WritePin( &GpioMIO, LD9, 0 );
}

/*******************************************************************************
 * A la fin on clean
*******************************************************************************/
void close_ZedBoard()
{
	cleanup_platform();
}
