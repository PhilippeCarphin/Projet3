/*
 * main.c
 *
 *  Created on: 2016-09-20
 *      Author: guvacc
 */

#define EMAC_BASEADDR XPAR_XEMACPS_0_BASEADDR

#include <stdio.h>

#include "xparameters.h"

#include "netif/xadapter.h"
#include "xil_printf.h"

#include "lwip/tcp.h"
#include "xil_cache.h"
#include "lostFunctionDeclaration.h"
#include "platform.h"
#include "server_tcp_ip.h"
#include "sd_card.h"

extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;
struct netif server_netif;

int initialiseServer(struct ip_addr *ipaddr,struct ip_addr *netmask,struct ip_addr *gw, unsigned char *mac_ethernet_address)
{
		// we intialisse the plaform
		init_platform();

		// we intialise lwip
		lwip_init();

		//we add the mac adress
		if (!xemac_add(&server_netif, ipaddr, netmask, gw, mac_ethernet_address, EMAC_BASEADDR))
		{
			xil_printf("Error coulden't set up the mac adress");
			return -1;
		}

		// we enable the interrupt after having added the mac adress
		netif_set_default(&server_netif);

		/* now enable interrupts */
		platform_enable_interrupts();

		/* specify that the network if is up */
		netif_set_up(&server_netif);

		return 0;
}

int init_TCP()
{
	struct ip_addr ipaddr, netmask, gw;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] =
	{ 0x00, 0x18, 0x3e, 0x00, 0x00, 0x27 };

	/* initlize IP addresses to be used */
	IP4_ADDR(&ipaddr,  132, 207, 89, 27);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      132, 207, 89,  1);

	if(initialiseServer(&ipaddr, &netmask, &gw, mac_ethernet_address ) == -1)
	{
		xil_printf("Initialization failed");
	}
	mount_sdcard();
	//we start the application

	if(startServer() != 0)
	{
		xil_printf("Error during the server start");
	}
	xil_printf("Server Started");
	/* receive and process packets */
	return 0;
}

int run_TCP()
{
	if (TcpFastTmrFlag) {
		tcp_fasttmr();
		TcpFastTmrFlag = 0;
	}
	if (TcpSlowTmrFlag) {
		tcp_slowtmr();
		TcpSlowTmrFlag = 0;
	}
	xemacif_input(&server_netif);
	return 0;
}

int close_TCP(){
	return 0;
}
