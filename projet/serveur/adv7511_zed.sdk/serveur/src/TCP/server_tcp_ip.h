/*
 * serverTcpIp.h
 *
 *  Created on: 2016-09-20
 *      Author: guvacc
 */

#ifndef SERVERTCPIP_H_
#define SERVERTCPIP_H_

#include "lwip/err.h"
#include "lwip/tcp.h"


int startServer();
err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
err_t write_callback(void *arg, struct tcp_pcb *tpcb, u16_t len);

#endif /* SERVERTCPIP_H_ */
