/*
 * test_http.c
 *
 *  Created on: 2016-11-01
 *      Author: frouee
 */

#include "server_http_in.h"
#include "test_http.h"
#include "xil_io.h"

/* Note: the Host field need not be accurate for tests to succeed */

/******************************************************************************
 * Runs all HTTP test in this file, one after the other.
 *****************************************************************************/
void test_http_all()
{
	test_empty();
	test_invalid_version();
	test_bad_request();
	test_new_game();
	test_get_board();
	test_start();
	test_move();
	test_promote();
	test_get_time();
	test_get_summary();
	test_post_board();
	test_get_details();
	test_end();
}

/******************************************************************************
 * Request: [EMPTY REQUEST]
 * Expected: HTTP/1.1 400 Bad Request
 *			 Content-Type : text/plain
 *			 Content-Length : 0
 *****************************************************************************/
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

/******************************************************************************
 * Request: GET /status/board
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.0
 *			Content-Length: 0
 *			
 * Expected: HTTP/1.1 505 HTTP Version Not Supported
 *			 Content-Type : text/plain
 *			 Content-Length : 0
 *****************************************************************************/
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

/******************************************************************************
 * Request: SALUT /status/board
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.0
 *			Content-Length: 0 
 *
 * Expected: HTTP/1.1 400 Bad Request
 *			 Content-Type : text/plain
 *			 Content-Length : 0 
 *****************************************************************************/
void test_bad_request()
{
	int result;
	//char request[] = "SALUT /board\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 0\r\n\r\n";
	char request[] = "GET /fdsddsa";
	char response[2048];

	xil_printf("\n******************* TEST BAD REQUEST *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("**********************************************************\n");
}

/******************************************************************************
 * Request: POST /new_game
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.1
 *			Content-Length: 229
 *			
 *			{
 *				"player1": Francis,
 *				"player2": J.RandomUser,
 *				"round": 1,
 *				"location": Polymtl,
 *				"secret_code": motdepasse,
 *				"twoTablet": no,
 *				"enPassant": no,
 *				"timerFormat": 
 *				{
 *					"time":	90,	
 *					"increment":30,
 *					"limit":	40,
 *					"overtime":	30,	
 *					"overtimeIncrement": 30
 *				}
 *			}
 *	
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : 0
 *****************************************************************************/
void test_new_game()
{
	int result;
	char request[] = "POST /new_game\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 247\r\n\r\nmotdepasse\r\n\r\n{\"player1\": Francis,\"player2\": J.RandomUser,\"round\": 1,\"location\": Polymtl,\"secret_code\": motdepasse,\"twoTablet\": no,\"enPassant\": no,\"timerFormat\": {\"time\":	90,	\"increment\":30,\"limit\":	40,\"overtime\":	30,	\"overtimeIncrement\": 30}}\r\n";
	char response[2048];

	xil_printf("\n******************* TEST NEW GAME *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: POST /game_start
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.1
 *			Content-Length: 0
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : 0
 * 
 * NOTE: new_game should be called before this
 *****************************************************************************/
void test_start()
{
	int result;
	char request[] = "POST /game_start\r\nHost: 192.168.145.114\r\nConection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST START **********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: GET /status/board
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *****************************************************************************/
void test_get_board()
{
	int result;
	char request[] = "GET /status/board\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST GET BOARD *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("******************************************************\n");
}

/******************************************************************************
 *
 *****************************************************************************/
void test_movePlayer1(char* coord)
{	
	int result;
	char src[100];
	char request[150];
    strcpy(src,  coord);
	strcpy(request, "POST /move/1/");
	strcat(request,src);
    strcpy(src,  "\r\nHTTP/1.1");
    strcat(request,src);
    strcat(request, "\r\n\r\nmotdepasse");
	char response[2048];

	xil_printf("\n******************* TEST MOVE ***********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 *
 *****************************************************************************/
void test_movePlayer2(char* coord)
{
	int result;
	char src[100];
	char request[150];
    strcpy(src,  coord);
	strcpy(request, "POST /move/2/");
	strcat(request,src);
    strcpy(src,  "\r\nHTTP/1.1\r\nContent-Length: 10");
    strcat(request,src);
    strcat(request, "\r\n\r\nmotdepasse");
	char response[2048];

	xil_printf("\n******************* TEST MOVE ***********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: POST /move/1/a2-a3
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 * NOTE: there should be a player 1's pawn/rook/king/queen in a2
 *****************************************************************************/
void test_move()
{
	int result;
	char request[] = "POST /move/2/b1-a3\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST MOVE ***********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 *
 *****************************************************************************/
void test_game()
{
	test_new_game();
	test_start();
	test_get_board();
	test_movePlayer1("e2-e4");
	test_movePlayer2("e7-e5");
	test_movePlayer1("d1-h5");
	test_movePlayer2("b8-c6");
	test_movePlayer1("f1-c4");
	test_movePlayer2("g8-f6");
	test_get_board();
	test_movePlayer1("h5-f7");
	test_movePlayer2("d7-d5");
	test_movePlayer1("e4-d5");
	test_get_board();
	test_movePlayer2("e8-e7");
	/*test_movePlayer1("h2-h3");
	test_movePlayer2("a8-b8");
	test_movePlayer1("h1-h2");
	test_get_board();*/


}

/******************************************************************************
 *
 *****************************************************************************/
void test_pawns()
{
	test_new_game();
	test_get_details();
	test_start();
	test_get_board();

	/* out of bounds */
	test_movePlayer1("h2-i2");
	test_get_board();

	/* ennemy piece */
	test_movePlayer1("h7-h5");
	test_get_board();

	/* invalid move */
	test_movePlayer1("e2-f4");
	test_get_board();

	/* not a move */
	test_movePlayer1("e2-e2");
	test_get_board();

	/* valid move (1) */
	test_movePlayer1("e2-e3");
	test_get_board();

	/* valid move (2) */
	test_movePlayer2("e7-e5");
	test_get_board();

	/* capture nothing */
	test_movePlayer1("d2-c3");
	test_get_board();

	/* capture ennemy piece */
	test_movePlayer1("d2-d4");
	test_movePlayer2("e5-d4");
	test_get_board();

	/* capture own piece */
	test_movePlayer1("f2-e3");
	test_get_board();

	test_end();
}

/******************************************************************************
 *
 *****************************************************************************/
void test_knight()
{

	test_new_game();
	test_get_details();
	test_start();
	test_get_board();

	/* out of bounds */
	/* ennemy piece */
	/* invalid move */
	/* not a move */
	/* valid move */
	/* capture ennemy piece */
	/* capture own piece */

	test_end();
}


/******************************************************************************
 * Request: POST /promote/1/queen
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : 0
 *
 * NOTE: game should be put into the proper state before calling this
 *****************************************************************************/
void test_promote()
{
	int result;
	char request[] = "POST /promote/1/queen\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST PROMOTE ********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: GET /time/1
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 *****************************************************************************/
void test_get_time()
{
	int result;
	char request[] = "GET /time/1\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST GET TIME *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: GET /status/summary
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 *****************************************************************************/
void test_get_summary()
{
	int result;
	char request[] = "GET /status/summary\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST GET SUMMARY ****************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: GET /game_details
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 *****************************************************************************/
void test_get_details()
{
	int result;
	char request[] = "GET /game_details\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST GET DETAILS ****************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: POST /status/board
 *			HTTP/1.1
 *			Content-Length: 334
 *
 *			{
 *				"turn": 1,
 *				"move_no": 2,
 *				"king1": a1,
 *				"queen1": a2,
 *				"bishop1A": a3,
 *				"bishop1B": a4,
 *				"rook1A": a5,
 *				"rook1B": a6,
 *				"knight1A": a7,
 *				"knight1B": a8,
 *				"pawn1": [ b1, b2, b3, b4, b5, b6, b7, b8 ],
 *				"king2": c1,
 *				"queen2": c2,
 *				"bishop2A": c3,
 *				"bishop2B": c4,
 *				"rook2A": c5,
 *				"rook2B": c6,
 *				"knight2A": c7,
 *				"knight2B": c8,
 *				"pawn2": [ d1, d2, d3, d4, d5, d6, d7, d8 ]				
 *			}
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : 0
 *****************************************************************************/
void test_post_board()
{
	int result;
	char request[] = "POST /status/board\r\nHTTP/1.1\r\nContent-Length: 352\r\n\r\nmotdepasse\r\n\r\n{\"turn\": 1,\"move_no\": 2,\"king1\": a1,\"queen1\": a2,\"bishop1A\": a3,\"bishop1B\": a4,\"rook1A\": a5,\"rook1B\": a6,\"knight1A\": a7,\"knight1B\": a8,\"pawn1\": [ b1, b2, b3, b4, b5, b6, b7, b8 ],\"king2\": c1,\"queen2\": c2,\"bishop2A\": c3,\"bishop2B\": c4,\"rook2A\": c5,\"rook2B\": c6,\"knight2A\": c7,\"knight2B\": c8,\"pawn2\": [ d1, d2, d3, d4, d5, d6, d7, d8 ]}";
	char response[2048];

	xil_printf("\n******************* TEST POST BOARD *****************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: POST /game_end
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.1
 *			Content-Length: 0
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type : text/plain
 *			 Content-Length : 0
 * 
 * NOTE: a game should be already started
 *****************************************************************************/
void test_end()
{
	int result;
	char request[] = "POST /game_end\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse\r\n\r\n";
	char response[2048];

	xil_printf("\n******************* TEST END ************************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	result = HTTP_dispatchRequest(request, response);
	xil_printf("Returned code: \n%d\n\n", result);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}
