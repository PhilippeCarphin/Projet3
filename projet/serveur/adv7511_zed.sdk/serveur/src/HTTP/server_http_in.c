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
void *get_body(const char *request, char *body, char *pswd, int len);
enum request_type get_type(const char *header, int *need_header);

/******************************************************************************
 * Strips the header of a received request and sends the REST part off to the
 * rest module
 *****************************************************************************/
int HTTP_dispatchRequest(const char *request, char *HTTP_response)
{
	static char header[MAX_LENGTH];	/* header part of request */
	static char body[MAX_LENGTH];	/* body part of request */
	static char pswd[MAX_LENGTH];	/* body part of request */
	int len, err, need_header;
		
	pswd[0] = '\0';
	/* get header */
	get_header(request, header);	
	if ((err = validate_request(header)) != HTTP_OK)
	{
		HTTP_build_from_code(err, HTTP_response);	/* Invalid request */
		return err;
	}
	
	/* get body */
	if ((len = get_body_length(header)) > 0)
	{
		get_body(request, body, pswd, len);	/* There is a body */
	}
	
	/* get type */
	enum request_type type = get_type(header, &need_header);
	if (type == ERROR)	/* could not identify type */
	{
		HTTP_build_from_code(HTTP_BAD_REQUEST, HTTP_response);
		return HTTP_BAD_REQUEST;
	}

	/* send request to REST module */
	char REST_response[MAX_LENGTH];
	if (need_header == 0) /* request BODY is the important part */
		err = REST_handle_request(type, body, pswd, REST_response);
	else		/* request HEADER is the important part */
		err = REST_handle_request(type, header, pswd, REST_response);

	if(err == OK)	/* REST module responded OK */
	{
		HTTP_build_from_REST(REST_response, HTTP_response);
		return 0;
	}
	else if (err < 0)	/* REST's response is invalid */
	{
		HTTP_build_from_code(HTTP_INTERNAL_SERVER_ERROR, HTTP_response);
		return HTTP_INTERNAL_SERVER_ERROR;
	}
	else	/* REST module's response is valid, but not OK */
	{
		HTTP_build_from_code(err, HTTP_response);
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
	/* Empty header: bad request */
	if (header[0] == '\0')
		return HTTP_BAD_REQUEST;
	
	/* HTTP version must be 1.1
	if (!strstr(header, "HTTP/1.1"))
		return HTTP_VERSION_NOT_SUPPORTED;*/
	
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
void *get_body(const char *request, char *body, char *pswd, int len)
{
	char *p, *q;
	
	/* find beginning of body */
	p = strstr(request, "\r\n\r\n") + 4;
	sscanf(p, "%[^\t\r\n]\r\n\r\n", pswd);

	q = strstr(p, "\r\n\r\n") + 4;
	
	/* copy into body and null-terminate it */
	memcpy(body, q, len - strlen(pswd));
	body[len-strlen(pswd)] = 0;
	return body;
}

/******************************************************************************
 * Gets a request type from its header
 *****************************************************************************/
enum request_type get_type(const char *header, int *need_header)
{
	if (strstr(header, "POST /new_game"))
	{
		*need_header = 0;
		return NEW_GAME;
	}
	if (strstr(header, "POST /join_game"))
	{
		*need_header = 0;
		return JOIN;
	}
	if (strstr(header, "POST /move"))
	{
		*need_header = 1;
		return MOVE;
	}
	if (strstr(header, "POST /promote"))
	{
		*need_header = 1;
		return PROMOTE;
	}
	if (strstr(header, "GET /time"))
	{
		*need_header = 1;
		return GET_TIME;
	}
	if (strstr(header, "GET /status/summary"))
	{
		*need_header = 0;
		return GET_SUMMARY;
	}
	if (strstr(header, "GET /status/board"))
	{
		*need_header = 0;
		return GET_BOARD;
	}
	if (strstr(header, "POST /status/board"))
	{
		*need_header = 0;
		return POST_BOARD;
	}
	if (strstr(header, "GET /game_details"))
	{
		*need_header = 0;
		return GET_DETAILS;
	}
	if (strstr(header, "POST /game_start"))
	{
		*need_header = 0;
		return START;
	}
	if (strstr(header, "POST /game_end"))
	{
		*need_header = 0;
		return END;
	}
	
	return ERROR;
}
