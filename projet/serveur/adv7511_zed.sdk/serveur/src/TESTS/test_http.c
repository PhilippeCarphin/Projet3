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
 *
 *****************************************************************************/
static void assert_test(const char *test_name, const char *actual, const char *expected)
{
	if (strcmp(expected, actual) == 0)
		xil_printf("\n%s test: %s\n\n", test_name, "PASSED");
	else
		xil_printf("\n%s test: %s\n\n", test_name, "FAILED");
}

/******************************************************************************
 *
 *****************************************************************************/
void test_http(const char *test_name, const char *request, const char *expected, int show_details)
{
	char response[2048];

	if (show_details)
	{
		xil_printf("\n\n******************* %s *******************\n", test_name);
		xil_printf("HTTP Request: \n%s", request);
	}

	HTTP_dispatchRequest(request, response);
	assert_test(test_name, response, expected);

	if (show_details)
	{
		xil_printf("HTTP Response: \n%s", response);
		xil_printf("EXPECTED: \n%s", expected);
		xil_printf("****************************************************\n");
	}
}

/******************************************************************************
 * Runs all HTTP test in this file, one after the other.
 *****************************************************************************/
void test_http_all()
{
	/* Invalid requests */
	test_empty(0);
	test_invalid_version(0);
	test_bad_request(0);

	/* Complete features */
	test_new_game(0);
	test_start(0);
	test_get_details();
	test_get_summary();
	test_get_board();
	test_move();
	//test_promote();	/* not yet implemented */
	//test_get_time();  /* not yet implemented */
	test_post_board();
	test_end();
}

/******************************************************************************
 * Request: [EMPTY REQUEST]
 * Expected: HTTP/1.1 400 Bad Request
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 *****************************************************************************/
void test_empty(int details)
{
	char test_name[] = "Empty request";
	char request[] = "";
	char expected[] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 0\r\nConnection: Closed\r\n\r\n";

	test_http(test_name, request, expected, details);
}

/******************************************************************************
 * Request: GET /status/board
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.0
 *			Content-Length: 0
 *			
 * Expected: HTTP/1.1 505 HTTP Version Not Supported
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 *****************************************************************************/
void test_invalid_version(int details)
{
	char test_name[] = "Invalid version";
	char request[] = "GET /status/board\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.0\r\nContent-Length: 0\r\n\r\n";
	char expected[] = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Type: text/plain\r\nContent-Length: 0\r\nConnection: Closed\r\n\r\n";

	test_http(test_name, request, expected, details);
}

/******************************************************************************
 * Request: SALUT /status/board
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.0
 *			Content-Length: 0 
 *
 * Expected: HTTP/1.1 400 Bad Request
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 *****************************************************************************/
void test_bad_request(int details)
{
	char test_name[] = "Bad request";
	char request[] = "GET /fdsddsa";
	char expected[] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 0\r\nConnection: Closed\r\n\r\n";

	test_http(test_name, request, expected, details);
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
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 *****************************************************************************/
void test_new_game(int details)
{
	char test_name[] = "New game";
	char request[] = "POST /new_game\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 265\r\n\r\nmotdepasse\r\n\r\n{\"player1\": \"Francis Ouellet\",\"player2\": \"J.RandomUser\",\"round\": \"1\",\"location\": \"Polymtl\",\"secret_code\": \"motdepasse\",\"twoTablet\": no,\"enPassant\": no,\"timerFormat\": {\"time\":	90,	\"increment\":30,\"limit\":	40,\"overtime\":	30,	\"overtimeIncrement\": 30}}\r\n";
	//char request[] = "POST /new_game HTTP/1.1\r\nContent-Type: application/json\r\nAccept: application/json\r\nUser-Agent: Dalvik/2.1.0 (Linux; U; Android 5.0.2; Nexus 9 Build/LRX22L)\r\nHost: 132.207.89.21\r\nConnection: Keep-Alive\r\nAccept-Encoding: gzip\r\nContent-Length: 243\r\n\r\nhelloworld\r\n\r\n{\"player1\":\"haha\",\"player2\":\"TODO\",\"round\":\"TODO\",\"location\":\"\",\"secret_code\":\"helloworld\",\"twoTablet\":false,\"enPassant\":false,\"timerFormat\":{\"time\": 90,\"increment\": 30,\"limit\": 40,\"overtime\": 40,\"overtimeIncrement\": 40}}";
	char expected[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 0\r\nConnection: Closed\r\n\r\n";

	test_http(test_name, request, expected, details);
}

/******************************************************************************
 * Request: POST /game_start
 *			Host: 192.168.145.114
 *			Connection: Keep-Alive
 *			HTTP/1.1
 *			Content-Length: 0
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 * 
 * NOTE: new_game should be called before this
 *****************************************************************************/
void test_start(int details)
{
	char test_name[] = "Start game";
	char request[] = "POST /game_start\r\nHost: 192.168.145.114\r\nConection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char expected[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 0\r\nConnection: Closed\r\n\r\n";

	test_http(test_name, request, expected, details);
}

/******************************************************************************
 * Request: GET /status/board
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type: text/plain
 *			 Content-Length: [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *****************************************************************************/
void test_get_board()
{
	char request[] = "GET /status/board\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n\n******************* Get board test *******************\n");
	xil_printf("HTTP Request: \n%s", request);
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s", response);
	xil_printf("******************************************************\n");
}

/******************************************************************************
 *
 *****************************************************************************/
void test_movePlayer1(char* coord)
{	

	char src[100];
	char request[150];
    strcpy(src,  coord);
	strcpy(request, "POST /move/1/");
	strcat(request,src);
    strcpy(src,  "\r\nHTTP/1.1\r\nContent-Length: 10");
    strcat(request,src);
    strcat(request, "\r\n\r\nmotdepasse");
	char response[2048];

	xil_printf("\n******************* TEST MOVE ***********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 *
 *****************************************************************************/
void test_movePlayer2(char* coord)
{

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
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: POST /move/1/a2-a3
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type: text/plain
 *			 Content-Length: [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 * NOTE: there should be a player 1's pawn/rook/king/queen in a2
 *****************************************************************************/
void test_move()
{

	char request[] = "POST /move/1/b1-a3\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST MOVE ***********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 *
 *****************************************************************************/
void test_game()
{
	test_new_game(0);
	test_start(0);
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
	test_new_game(0);
	test_get_details();
	test_start(0);
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
	test_new_game(0);
	test_get_details();
	test_start(0);
	test_get_board();

	/* out of bounds */
	test_movePlayer1("g1-i2");
	test_get_board();

	/* ennemy piece */
	test_movePlayer1("g8-f6");
	test_get_board();

	/* invalid move */
	test_movePlayer1("g1-g3");
	test_get_board();

	/* not a move */
	test_movePlayer1("g1-g1");
	test_get_board();

	/* valid move */
	test_movePlayer1("g1-f3");
	test_get_board();

	/* capture ennemy piece */
	test_movePlayer2("b8-c6");
	test_movePlayer1("f3-e5");
	test_movePlayer2("c6-e5");
	test_get_board();

	/* capture own piece */
	test_movePlayer1("b1-d2");
	test_get_board();

	test_end();
}

/******************************************************************************
 *
 *****************************************************************************/
void test_bishop()
{
	test_new_game(0);
	test_get_details();
	test_start(0);
	test_get_board();

	/* out of bounds */
	test_movePlayer1("d2-d3");
	test_movePlayer2("d7-d5");
	test_movePlayer1("c1-i7");
	test_get_board();

	/* ennemy piece */
	test_movePlayer1("c8-f5");
	test_get_board();

	/* invalid move */
	test_movePlayer1("c1-b3");
	test_get_board();

	/* piece in the way */
	test_movePlayer1("c1-a3");
	test_get_board();

	/* not a move */
	test_movePlayer1("c1-c1");
	test_get_board();

	/* valid move */
	test_movePlayer1("c1-f4");
	test_get_board();

	/* capture ennemy piece */
	test_movePlayer2("c8-f5");
	test_movePlayer1("f4-c7");
	test_get_board();

	/* capture own piece */
	test_movePlayer2("f5-h7");
	test_get_board();

	test_end();
}

/******************************************************************************
 *
 *****************************************************************************/
void test_rook()
{
	test_new_game(0);
	test_get_details();
	test_start(0);
	test_get_board();

	/* set board for proper testing */
	test_movePlayer1("h2-h4");
	test_movePlayer2("h7-h5");
	test_movePlayer1("g2-g4");
	test_movePlayer2("g7-g5");
	test_movePlayer1("h4-g5");
	test_movePlayer2("h5-g4");
	test_get_board();

	/* out of bounds */
	test_movePlayer1("h1-k1");
	test_get_board();

	/* ennemy piece */
	test_movePlayer1("h8-h1");
	test_get_board();

	/* invalid move */
	test_movePlayer1("h1-g3");
	test_get_board();

	/* not a move */
	test_movePlayer1("h1-h1");
	test_get_board();

	/* valid move */
	test_movePlayer1("h1-h7");
	test_get_board();

	/* capture ennemy piece */
	test_movePlayer2("b8-c6");
	test_movePlayer1("h7-f7");
	test_get_board();

	/* capture own piece */
	test_movePlayer2("h8-g8");
	test_get_board();

	test_end();
}

/******************************************************************************
 *
 *****************************************************************************/
void test_queen()
{
	test_new_game(0);
	test_get_details();
	test_start(0);
	test_get_board();

	/* set board for proper testing */
	test_movePlayer1("e2-e3");
	test_movePlayer2("e7-e5");
	test_get_board();

	/* out of bounds */
	test_movePlayer1("d1-i6");
	test_get_board();

	/* ennemy piece */
	test_movePlayer1("d8-g5");
	test_get_board();

	/* invalid move */
	test_movePlayer1("d1-c3");
	test_get_board();

	/* not a move */
	test_movePlayer1("d1-d1");
	test_get_board();

	/* valid move (1) */
	test_movePlayer1("d1-g4");
	test_movePlayer2("d8-h4");
	test_get_board();

	/* valid move (2) */
	test_movePlayer1("g4-c4");
	test_get_board();

	/* capture ennemy piece */
	test_movePlayer2("h4-f2");
	test_get_board();

	/* capture own piece */
	test_movePlayer1("c4-c2");
	test_get_board();

	test_get_summary();
	test_end();
}

/******************************************************************************
 *
 *****************************************************************************/
void test_king()
{
	test_new_game(0);
	test_get_details();
	test_start(0);
	test_get_board();

	/* set board for proper testing */

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
 *
 *****************************************************************************/
void test_full_promotion()
{

	static int sequence = 1;

	switch (sequence)
	{
	case 1: test_new_game(1);
	break;
	case 2:test_start(1);
	break;

	case 3:test_movePlayer1("e2-e4");
	break;
	case 4:test_movePlayer2("d7-d5");
	break;

	case 5:test_movePlayer1("e4-d5");
	break;
	case 6:test_movePlayer2("d8-d7");
	break;

	case 7:test_movePlayer1("d5-d6");
	break;
	case 8:test_movePlayer2("d7-e6");
	break;

	case 9:test_movePlayer1("d6-d7");
	break;
	case 10:test_movePlayer2("e6-e5");
	break;

	case 11:test_movePlayer1("d7-d8");
	test_promote();
	break;
	case 12:test_movePlayer2("e5-d5");
	break;

	case 13:test_movePlayer1("d8-d5");
	break;

	case 14:test_end();
	sequence = 0;
	break;

	default:
	break;
	}

	sequence++;
}

/******************************************************************************
 * Request: POST /promote/1/queen
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 *
 * NOTE: game should be put into the proper state before calling this
 *****************************************************************************/
void test_promote()
{

	char request[] = "POST /promote/1/queen\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST PROMOTE ********************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: GET /time/1
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type: text/plain
 *			 Content-Length: [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 *****************************************************************************/
void test_get_time()
{

	char request[] = "GET /time/1\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST GET TIME *******************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: GET /status/summary
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type: text/plain
 *			 Content-Length: [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 *****************************************************************************/
void test_get_summary()
{

	char request[] = "GET /status/summary\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST GET SUMMARY ****************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}

/******************************************************************************
 * Request: GET /game_details
 *			HTTP/1.1
 *
 * Expected: HTTP/1.1 200 OK
 *			 Content-Type: text/plain
 *			 Content-Length: [SIZE OF FOLLOWING JSON]
 *
 *			 {
 *				[SHOUlD BE COHERENT WITH CURRENT STATUS OF GAME]
 *			 }
 *
 *****************************************************************************/
void test_get_details()
{

	char request[] = "GET /game_details\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST GET DETAILS ****************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
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
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 *****************************************************************************/
void test_post_board()
{

	char request[] = "POST /status/board\r\nHTTP/1.1\r\nContent-Length: 352\r\n\r\nmotdepasse\r\n\r\n{\"turn\": 1,\"move_no\": 2,\"king1\": a1,\"queen1\": a2,\"bishop1A\": a3,\"bishop1B\": a4,\"rook1A\": a5,\"rook1B\": a6,\"knight1A\": a7,\"knight1B\": a8,\"pawn1\": [ b1, b2, b3, b4, b5, b6, b7, b8 ],\"king2\": c1,\"queen2\": c2,\"bishop2A\": c3,\"bishop2B\": c4,\"rook2A\": c5,\"rook2B\": c6,\"knight2A\": c7,\"knight2B\": c8,\"pawn2\": [ d1, d2, d3, d4, d5, d6, d7, d8 ]}";
	char response[2048];

	xil_printf("\n******************* TEST POST BOARD *****************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
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
 *			 Content-Type: text/plain
 *			 Content-Length: 0
 * 
 * NOTE: a game should be already started
 *****************************************************************************/
void test_end()
{

	char request[] = "POST /game_end\r\nHost: 192.168.145.114\r\nConnection: Keep-Alive\r\nHTTP/1.1\r\nContent-Length: 10\r\n\r\nmotdepasse";
	char response[2048];

	xil_printf("\n******************* TEST END ************************\n");
	xil_printf("HTTP Request: \n%s\n\n", request);
	HTTP_dispatchRequest(request, response);
	xil_printf("HTTP Response: \n%s\n\n", response);
	xil_printf("*******************************************************\n");
}
