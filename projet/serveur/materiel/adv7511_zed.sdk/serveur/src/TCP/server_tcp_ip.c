/*
 * serverTcpIp.c
 *
 *  Created on: 2016-09-20
 *      Author: guvacc
 */
#include "server_tcp_ip.h"
#include <stdio.h>
#include "server_http_in.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "xil_printf.h"

// Struct used for all callbacks
struct CallBackArgs
{
	//int connection; // number of connections that we have
	int index; // index of the current position from where we are transmiting in the HTTP response
	int responseLength; // length of the bufferPointer
	int maxPacketLength; // max length of a packet
	const char* response; // pointer to the start of the response

};

/******************************************************************************
 * Function to start the tcp/ip server
 *****************************************************************************/
int startServer()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 80;

	/* create new TCP PCB structure */
	pcb = tcp_new();
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ADDR_ANY, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguement to the call back function for now */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	tcp_nagle_disable(pcb);

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}


/******************************************************************************
 * Callback when we recieve a ack when in big file transmission mode
 * note : write a packet with len size each time we recieve a ack until message is over
 *****************************************************************************/
err_t large_file_ack_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct CallBackArgs* callBackInformation = (struct CallBackArgs*)arg;
	err_t error;
	callBackInformation->index += len;
	if (callBackInformation->index + callBackInformation->maxPacketLength < callBackInformation->responseLength)
	{
		/* try to write the next part of our response */
		error = tcp_write(tpcb, callBackInformation->response + callBackInformation->index, callBackInformation->maxPacketLength, TCP_WRITE_FLAG_COPY);
		if (error != ERR_OK)
		{
			return error;  // if we can't write we will try again the next time this callback is called
		}
	}
	else
	{
		/* try to write the final part of our response */
		int readingLength = callBackInformation->responseLength - callBackInformation->index;
		error = tcp_write(tpcb, callBackInformation->response + callBackInformation->index, readingLength, TCP_WRITE_FLAG_COPY);
		if (error != ERR_OK)
		{
			return error;  // if we can't write we will try again the next time this callback is called
		}
		// change the callback function, our response has been sent
		tcp_sent(tpcb,NULL);
	}
	return ERR_OK;
}

/******************************************************************************
 * Callback Function when we receive a packet
 *****************************************************************************/
err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	static struct CallBackArgs args;
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	//tcp_recved(tpcb, p->len);
	int maxPacketLength = tcp_sndbuf(tpcb);
	/* decode the expected HTTP request and generate the appropriate response */

	char HTTP_response[1000];

	xil_printf("\n===============================================================================\n");

	xil_printf("Received request:\n%s\n", p->payload);

	HTTP_dispatchRequest(p->payload, HTTP_response);

	xil_printf("Response:\n%s\n",HTTP_response);

	int len = strlen(HTTP_response);

	// appel de fonction
	//      http_dispatch_request(p->payload, HTTP_response)
	// maintenant response contient la reponse
	// tcp_write( HTTP_response)
	/* if the response is too large to be sent in one packet, set a callback function to sent the next parts*/
	if (len > maxPacketLength)
	{

		/* set the parameters for our callback */
		args.index = 0;
		args.maxPacketLength = maxPacketLength;
		args.response = HTTP_response;
		args.responseLength = len;
		err = tcp_write(tpcb, HTTP_response + args.index, args.maxPacketLength, TCP_WRITE_FLAG_COPY);
		tcp_output(tpcb);
		/* set our struct as the callback argument */
		tcp_arg(tpcb,(void*)&args);
		/* set the callback function */
		tcp_sent(tpcb,large_file_ack_callback);
	}
	else
	{
		err = tcp_write(tpcb, HTTP_response, len, TCP_WRITE_FLAG_COPY);
	}



	/* free the received pbuf */
	pbuf_free(p);

	return err;
}

/******************************************************************************
 * Function which accept the connection and set the recieve callback for further transmission
 *****************************************************************************/
err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	// not used for now but mightb e used later
	//static int connection = 1;
	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);
	/* increment for subsequent accepted connections */
	//connection++;
	return ERR_OK;
}
