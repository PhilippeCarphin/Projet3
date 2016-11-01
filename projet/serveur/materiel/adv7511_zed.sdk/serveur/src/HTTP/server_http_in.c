#include "server_http_in.h"
#include "server_rest.h"
#include "server_http_out.h"
#include "chessboard_rest_protocol.h"
#include "http_codes.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LENGTH 2048

void *get_header(const char *request, char *header);
int validate_request(const char *header);
int get_body_length(const char *header);
void *get_body(const char *request, char *body, int len);
enum request_type get_type(const char *header);

/******************************************************************************
 * Strips the header of a received request and sends the REST part off to the
 * rest module
 *****************************************************************************/
int HTTP_dispatchRequest(const char *request, char *HTTP_response)
{
	static char header[MAX_LENGTH];
	static char body[MAX_LENGTH];
	int len, err;
		
	/* get header */
	get_header(request, header);	
	if ((err = validate_request(header)) != HTTP_OK)
	{
		HTTP_code_to_HTTP(err, HTTP_response);
		return -1;
	}
	
	/* get body */
	if ((len = get_body_length(header)) > 0)
	{
		get_body(request, body, len);		
	}
	
	/* get type */
	enum request_type type = get_type(header);
	if (type == ERROR)
	{
		HTTP_code_to_HTTP(HTTP_BAD_REQUEST, HTTP_response);
		return -1;
	}

	/* send request to REST module */
	char REST_response[1000];
	if (len > 0) 
		err = REST_handle_request(type, body, REST_response);
	else
		err = REST_handle_request(type, header, REST_response);

	if(err == OK) {
		HTTP_REST_to_HTTP(REST_response, HTTP_response);
		return 0;
	}
	else
	{
		HTTP_code_to_HTTP(err, HTTP_response);
		return 0;
	}
}

/******************************************************************************
 * Gets the header of a received request
 *****************************************************************************/
void *get_header(const char *request, char *header)
{
	char *p;
	int len;
	
	/* find ending of header */
	p = strstr(request, "\r\n\r\n");	
	if (!p) 
	{
		/* no body: request is its own header */
		strcpy(header, request);
		return header;
	}
	
	/* copy header and null-terminate it*/
	len = (p - request);	
	memcpy(header, request, len);
	header[len] = 0;	
	return header;
}

/******************************************************************************
 * Validates a request from its header
 *****************************************************************************/
int validate_request(const char *header)
{
	/* HTTP version must be 1.1 */
	if (!strstr(header, "HTTP/1.1"))
		return HTTP_VERSION_NOT_SUPPORTED;
	return HTTP_OK;
}

/******************************************************************************
 * Gets http body length from header. Returns 0 if no body
 *****************************************************************************/
int get_body_length(const char *header)
{
	char *p;
	static char buf[MAX_LENGTH];
	int len;
	
	p = strstr(header, "Content-Length");

	/* length not found: no body */
	if (!p)
		return 0;
	
	/* length found: return it */
	sscanf(p, "%s %d", buf, &len);
	return len;
}

/******************************************************************************
 * Gets the body of a received request
 * Caller needs to know the body length in advance
 *****************************************************************************/
void *get_body(const char *request, char *body, int len)
{
	char *p;
	
	/* find beginning of body */
	p = strstr(request, "\r\n\r\n") + 4;
	
	/* copy into body and null-terminate it */
	memcpy(body, p, len);
	body[len] = 0;
	return body;
}

/******************************************************************************
 * Gets a request type from its header
 *****************************************************************************/
enum request_type get_type(const char *header)
{
	if (strstr(header, "POST /new_game"))
		return NEW_GAME;
	if (strstr(header, "POST /move"))
		return MOVE;
	if (strstr(header, "POST /promote"))
		return PROMOTE;
	if (strstr(header, "GET /time"))
		return GET_TIME;
	if (strstr(header, "GET /status/summary"))
		return GET_SUMMARY;
	if (strstr(header, "GET /status/board"))
		return GET_BOARD;
	if (strstr(header, "POST /status/board"))
		return POST_BOARD;
	if (strstr(header, "GET /game_details"))
		return GET_DETAILS;
	if (strstr(header, "POST /game_start"))
		return START;
	if (strstr(header, "POST /game_end"))
		return END;
	
	return ERROR;
}
