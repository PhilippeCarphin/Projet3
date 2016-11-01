/*
 * test_http.c
 *
 *  Created on: 2016-11-01
 *      Author: frouee
 */

#include "server_http_in.h"
#include "xil_printf.h"

void test_get_board()
{
	int result;
	char request[] = "GET /status/board\r\nHTTP/1.1";
	char response[2048];

	xil_printf("\n******************* TEST MINIMAL *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("******************************************************\n");
}

void test_empty()
{
	int result;
	char request[] = "";
	char response[2048];

	xil_printf("\n******************* TEST EMPTY *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("****************************************************\n");
}

void test_invalid_version()
{
	int result;
	char request[] = "GET /status/board\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.0\r\nContent-Length: 0\r\n\r\n";
	char response[2048];

	xil_printf("\n******************* TEST INVALID VERSION *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("**************************************************************\n");
}

void test_bad_request()
{
	int result;
	char request[] = "SALUT /board\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 0\r\n\r\n";
	char response[2048];

	xil_printf("\n******************* TEST BAD REQUEST *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("**********************************************************\n");
}

void test_new_game()
{
	int result;
	char request[] = "POST /new_game\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 229\r\n\r\n{\"player1\": Francis,\"player2\": J.RandomUser,\"round\": 1,\"location\": Polymtl,\"secret_code\": motdepasse,\"twoTablet\": no,\"enPassant\": no,\"timerFormat\": {\"time\":	90,	\"increment\":30,\"limit\":	40,\"overtime\":	30,	\"overtimeIncrement\": 30}}\r\n";
	char response[2048];

	xil_printf("\n******************* TEST NEW GAME *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}


