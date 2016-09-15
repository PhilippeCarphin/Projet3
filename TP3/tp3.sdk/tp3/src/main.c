/*
 * main.c
 *
 *  Created on: 2016-09-15
 *      Author: cllanb
 */

#include "lwip/init.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "netif/etharp.h"


static unsigned char mac[] = { 0x00, 0x18, 0x3e, 0x00, 0x00, 0x20 };

err_t netif_if_init(struct netif *netif)
{
	u8_t i;

	for (i = 0; i < ETHARP_HWADDR_LEN; i++)
		netif->hwaddr[i] = mac[i];
	return ERR_OK;
}

/* COPIÉ & COLLÉ DE L'EXEMPLE. CHANGER PLUS TARD. */
err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	/* echo back the payload */
	/* in this case, we assume that the payload is < TCP_SND_BUF */
	if (tcp_sndbuf(tpcb) > p->len)
		err = tcp_write(tpcb, p->payload, p->len, 1);

	/* free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}

/* COPIÉ & COLLÉ DE L'EXEMPLE. CHANGER PLUS TARD. */
err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}

int main()
{
	struct ip_addr ipaddr, netmask, gw;
	IP4_ADDR(&ipaddr,  132, 207,   89, 20);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      132, 207,   89,  20);

	struct netif server_netif;

	/* platform initializations */

	/* initialize lwIP stack */
	lwip_init();

	/* add network interface to the netif_list. The init function and input are still unclear */
	netif_add(&server_netif, &ipaddr, &netmask, &gw, NULL, (netif_init_fn)(netif_if_init), (netif_input_fn)(ethernet_input));
	netif_set_default(&server_netif);
	netif_set_link_up(&server_netif);
	netif_set_up(&server_netif);

	/* tcp listening */
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 80;

	pcb = tcp_new();
	err = tcp_bind(pcb, IP_ADDR_ANY, port);
	tcp_arg(pcb, NULL);
	pcb = tcp_listen(pcb);

	tcp_accept(pcb, accept_callback);

	while (1)
	{
	}

	return 0;
}
