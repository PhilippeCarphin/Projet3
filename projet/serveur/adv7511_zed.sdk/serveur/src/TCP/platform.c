/*
 * platform.c
 *	File which serve to link the different drivers and ethernet stuff with timers
 *  Created on: 2016-09-20
 *      Author: guvacc
 */
#include "xparameters.h"
#include "xparameters_ps.h"	/* defines XPAR values */
#include "xil_cache.h"
#include "xscugic.h"
#include "lwip/tcp.h"
#include "xil_printf.h"
#include "netif/xadapter.h"
#include "xscutimer.h"
#include "xtmrctr.h"
#include "sd_card.h"
#include "platform.h"

//#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID//////////////////
//#define TIMER_DEVICE_ID		XPAR_SCUTIMER_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_AXI_TIMER_0_DEVICE_ID
//#define INTC_BASE_ADDR		XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_BASE_ADDR		XPAR_AXI_TIMER_0_BASEADDR
//#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_0_DIST_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_0_DIST_BASEADDR  ////// not used?
//#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR////////////////////////////

#define RESET_RX_CNTR_LIMIT	400

void tcp_fasttmr(void);
void tcp_slowtmr(void);

//static XScuTimer TimerInstance;
static XTmrCtr TimerInstance;

XScuGic InterruptController;
XScuGic_Config *GicConfigPtr;

static int ResetRxCntr = 0;

volatile int TcpFastTmrFlag = 0;
volatile int TcpSlowTmrFlag = 0;
volatile int UpdateDisplayFlag = 0;


/*
 * platform.c
 *	Code that we need to call to set up the the interrupts and the timer for the server to function properly
 *  Created on: 2016-09-20
 *      Author: guvacc
 */
void init_platform()
{
	platform_setup_timer();
	platform_setup_interrupts();
}

// callback function for the timer in this case to set the different flags
//void timer_callback(XScuTimer * TimerInstance,struct netif *server_netif)
void timer_callback(XTmrCtr * TimerInstance,struct netif *server_netif)
{
	/* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified
	 * by lwIP. It is not important that the timing is absoluetly accurate.
	 */
	static int odd = 1;

	odd = !odd;
	ResetRxCntr++;
	UpdateDisplayFlag++;
	if (odd)
	{
		TcpSlowTmrFlag = 1;

	}
	else
	{
		 TcpFastTmrFlag = 1;
	}

	/* For providing an SW alternative for the SI #692601. Under heavy
	 * Rx traffic if at some point the Rx path becomes unresponsive, the
	 * following API call will ensures a SW reset of the Rx path. The
	 * API xemacpsif_resetrx_on_no_rxdata is called every 100 milliseconds.
	 * This ensures that if the above HW bug is hit, in the worst case,
	 * the Rx path cannot become unresponsive for more than 100
	 * milliseconds.
	 */
	if (ResetRxCntr >= RESET_RX_CNTR_LIMIT) {
		xemacpsif_resetrx_on_no_rxdata(server_netif);
		ResetRxCntr = 0;
	}
	//XScuTimer_ClearInterruptStatus(TimerInstance);//////////////////////////////////////////////////
}

// function to link the interupt handler to the hardware and to the timer_callback function on interrupt
void platform_setup_interrupts()
{
	XScuGic_DeviceInitialize(INTC_DEVICE_ID);
	GicConfigPtr = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );
	XScuGic_CfgInitialize(&InterruptController, GicConfigPtr,
						GicConfigPtr->CpuBaseAddress);
	XScuGic_SelfTest( &InterruptController);

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			(Xil_ExceptionHandler)XScuGic_DeviceInterruptHandler,
			(void *)INTC_DEVICE_ID);
	Xil_ExceptionEnable();
	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR, TIMER_IRPT_INTR,
					(Xil_ExceptionHandler)XTmrCtr_InterruptHandler,
					(void *)&TimerInstance);
	XTmrCtr_SetHandler( &TimerInstance,
					(XTmrCtr_Handler)timer_callback,
					(void *)&TimerInstance);
	/*
	 * Enable the interrupt for scu timer.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TIMER_IRPT_INTR);
	return;

	//XScuGic_RegisterHandler(&ScuGic, 0, (Xil_ExceptionHandler)XTmrCtr_InterruptHandler, (void *)&TimerInstance);
}

//Function to setup the timer and initialise it, and also initialise it's timeout value
void platform_setup_timer()
{
	int Status = XST_SUCCESS;
	//XScuTimer_Config *ConfigPtr;
	XTmrCtr_Config *ConfigPtr;
	int TimerLoadValue = 0;

	//ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	ConfigPtr = XTmrCtr_LookupConfig(TIMER_DEVICE_ID);
	//Status = XScuTimer_CfgInitialize(&TimerInstance, ConfigPtr,
	//		ConfigPtr->BaseAddr);
	XTmrCtr_CfgInitialize(&TimerInstance, ConfigPtr,
			ConfigPtr->BaseAddress);
	//if (Status != XST_SUCCESS) {
	//	xil_printf("In %s: Scutimer Cfg initialization failed...\r\n",
	//	__func__);
	//}

	//Status = XScuTimer_SelfTest(&TimerInstance);
	Status = XTmrCtr_SelfTest(&TimerInstance, 0); // test the timer 0
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: AXI Timer 0 Self test failed...\r\n",
		__func__);
		return;
	}

	//XScuTimer_EnableAutoReload(&TimerInstance);
	XTmrCtr_SetOptions(&TimerInstance, 0, XTC_AUTO_RELOAD_OPTION | XTC_INT_MODE_OPTION); // we use the timer 0 of this instance and enable the auto reload and interruption.
	/*
	 * Set for 250 milli seconds timeout.
	 */
	TimerLoadValue = XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 8;

	//XScuTimer_LoadTimer(&TimerInstance, TimerLoadValue);
	XTmrCtr_SetResetValue(&TimerInstance, 0, TimerLoadValue); // use the timer 0
	return;
}

// Function to enable the interrupts we just setup earlier
void platform_enable_interrupts()
{
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
	//XScuTimer_EnableInterrupt(&TimerInstance);
	// interrupt output already enabled in the Timer
	//XScuTimer_Start(&TimerInstance);
	XTmrCtr_Start(&TimerInstance, 0);
}



