#include "server_http_out.h"
#include "http_codes.h"
#include "macros.h"

#include <string.h>
#include <stdio.h>

#define MAX_LENGTH 2048
#define MSG_LEN 50

const char *build_response(int code, char *content_type, const char *content);
const char *build_header(int code, char *content_type, int content_length);
void get_http_message(int code, char *msg);

/******************************************************************************
 * Put an HTTP header onto input and send it to TCP module.
 *****************************************************************************/
void HTTP_build_from_REST(const char *rest, char* http)
{
	strcpy(http, build_response(HTTP_OK, "text/plain", rest));
	
	PRINT("%s\n", http);
}

/******************************************************************************
 * Creates an HTTP error response from error code and send it to TCP module.
 *****************************************************************************/
void HTTP_build_from_code(int code, char *http)
{
	strcpy(http, build_response(code, "text/plain", ""));
	
	PRINT("%s\n", http);
}

/******************************************************************************
 * Builds a HTTP response for null-terminated content (for now).
 *****************************************************************************/
const char *build_response(int code, char *content_type, const char *content)
{
	static char response[MAX_LENGTH];
	response[0] = '\0';

	const char *header = build_header(code, content_type, strlen(content));

	strcpy(response, header);
	strcat(response, content);

	return (const char *)response;
}

/******************************************************************************
 * Builds a minimal HTTP header, containing only the error code, the type and
 * the length of the content. Can be used for any type of response.
 *****************************************************************************/
const char *build_header(int code, char *content_type, int content_length)
{
	static char header[MAX_LENGTH], message[MSG_LEN];
	
	get_http_message(code, message);
	sprintf(header, "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %d\r\nConnection: Closed\r\n\r\n",
			 message, content_type, content_length);
	return (const char *)header;
}

/******************************************************************************
 * Maps each http code with appropriate messages.
 *****************************************************************************/
void get_http_message(int code, char *msg)
{
	char *m;
	
	if (code == HTTP_OK)
		m = "200 OK";
	else if (code == HTTP_BAD_REQUEST)
		m = "400 Bad Request";
	else if (code == HTTP_UNAUTHORIZED)
		m = "401 Unauthorized";
	else if (code == HTTP_FORBIDDEN)
		m = "403 Forbidden";
	else if (code == HTTP_NOT_FOUND)
		m = "404 Not Found";
	else if (code == HTTP_NOT_ACCEPTABLE)
		m = "406 Not Acceptable";
	else if (code == HTTP_REQUEST_TIMEOUT)
		m = "408 Request Time-out";
	else if (code == HTTP_INTERNAL_SERVER_ERROR)
		m = "500 Internal Server Error";
	else if (code == HTTP_NOT_IMPLEMENTED)
		m = "501 Not Implemented";
	else if (code == HTTP_VERSION_NOT_SUPPORTED)
		m = "505 HTTP Version Not Supported";
	else if (code == HTTP_IM_A_TEAPOT)
		m = "418 I'm A Teapot";
	else
		m = "500 Internal Server Error";
	
	sprintf(msg,"%s",m);
}
