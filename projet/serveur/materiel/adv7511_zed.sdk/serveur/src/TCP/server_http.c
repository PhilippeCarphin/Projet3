#include <string.h>
#include <stdio.h>
#include "ff.h"
#include "xil_printf.h"
#include "splitTokens.h"

/* used in header building */
static char *err_codes[] = {
		"200 OK",
		"403 Forbidden",
		"404 Not Found",
		"500 Internal Server Error",
		"501 Not Implemented",
		"505 HTTP Version not supported"
};

/* global to avoid having to pass as argument in every function */
static int response_length;

#define HTTP_OK 0
#define HTTP_FORBIDDEN 1
#define HTTP_NOT_FOUND 2
#define HTTP_INTERNAL_SERVER_ERROR 3
#define HTTP_NOT_IMPLEMENTED 4
#define HTTP_VERSION_NOT_SUPPORTED 5

#define MAX_RESPONSE_SIZE 500	 /* for null-terminated responses */
#define MAX_HEADER_SIZE	100		 /* for headers (usually around 50-70B) */
#define BIG_BUFFER_SIZE 10500000 /* for images (maybe it needs to be bigger?) */

const char *decode_request(const char *request, int *response_length);
const char *response_switch(char *request);
const char *build_response(int code, char *content_type, char *content);
const char *build_bmp_response(char *path);
const char *build_header(int code, char *content_type, int content_length);
UINT read_file(char *path, char *buffer, UINT size);

/******************************************************************************
 * Analyses and responds to a HTTP request.
 * NOTE: response_actual_length is changed to the actual response length.
 *****************************************************************************/
const char *decode_request(const char *request, int *response_actual_length)
{
	xil_printf("HTTP request: %s\n", request);

	char **tokens = getTokens(request, " \n\r");
	static char response[MAX_RESPONSE_SIZE];

	/* Only accept GET requests and HTTP/1.1 (for now) */
	if (strcmp(tokens[0] , "GET") != 0)
	{
		strcpy(response, build_response(HTTP_NOT_IMPLEMENTED, "text/html",
						"<h1>501 Not Implemented</h1>\n \
						<p>Only GET requests are accepted for now.</p>"));
	}
	else if (strcmp(tokens[2] , "HTTP/1.1") != 0)
	{
		strcpy(response, build_response(HTTP_VERSION_NOT_SUPPORTED, "text/html",
								"<h1>505 HTTP Version not supported</h1>\n \
								<p>Only HTTP/1.1 requests are accepted.</p>"));
	}
	else
	{
		const char *buf = response_switch(tokens[1]);
		xil_printf("HTTP Response: %s\n\n", buf);

		*response_actual_length = response_length;
		return (const char *)buf;
	}

	xil_printf("HTTP Response: %s\n\n", response);

	/* before returning, we set response_length
	 * (changed from build_response and build_bmp_response)
	 */
	*response_actual_length = response_length;
	return (const char *)response;
}

/******************************************************************************
 * Build a HTTP response for particular requests.
 *****************************************************************************/
const char *response_switch(char *request)
{
	if(strcmp(request , "/test1") == 0)
	{
		return build_response(HTTP_OK, "text/plain", "test 1 est passé!");
	}
	else if(strcmp(request, "/test2") == 0)
	{
		return build_response(HTTP_OK, "text/html", "<a href=\"http://www.polymtl.ca\">Poly Mtl</a>");
	}

	else if(strcmp(request, "/test3") == 0)
	{
		return build_bmp_response("zedboard.bmp");
	}
	else
	{	/* don't accept unknown GET requests (maybe add a case for favicon.ico?) */
		return build_response(HTTP_FORBIDDEN, "text/html",
									"<h1>403 Forbidden</h1>\n \
									<p>Use <a href=\"./test1\">/test1</a>, \
									<a href=\"./test2\">/test2</a> or \
									<a href=\"./test3\">/test3</a> instead.</p>");
	}
}

/******************************************************************************
 * Builds a HTTP response for null-terminated content (for now).
 *****************************************************************************/
const char *build_response(int code, char *content_type, char *content)
{
	static char response[MAX_RESPONSE_SIZE];

	const char *header = build_header(code, content_type, strlen(content));

	strcpy(response, header);
	strcat(response, content);

	/* maybe change strlen later, to add support for non null-terminated content */
	response_length = strlen(response);

	return (const char *)response;
}

/******************************************************************************
 * Builds a HTTP response containing a BMP.
 *****************************************************************************/
const char *build_bmp_response(char *path)
{
	FILINFO fno;
	FRESULT result = f_stat(path, &fno);

	/* Picture not found */
	if (result != FR_OK)
		return build_response(HTTP_NOT_FOUND, "text/html",
				"<h1>404 Not Found</h1>\n \
				<p>Image BMP not found. \
				Make sure the SD Card is in and the path exists.</p>");

	const char *header = build_header(HTTP_OK, "image/bmp", fno.fsize);
	int offset = strlen(header);

	static char big_buffer[BIG_BUFFER_SIZE];
	strcpy(big_buffer, header);
	UINT bytes_read = read_file(path, &big_buffer[offset], fno.fsize);

	response_length = fno.fsize + offset;

	/* Read file failed */
	if (fno.fsize != bytes_read)
		return build_response(HTTP_NOT_FOUND, "text/html",
				"<h1>404 Not Found</h1>\n \
				<p>Failed to read image. \
				Make sure the SD Card is in and the path exists.</p>");

	return big_buffer;
}

/******************************************************************************
 * Builds a minimal HTTP header, containing only the error code, the type and
 * the length of the content. Can be used for any type of response.
 *****************************************************************************/
const char *build_header(int code, char *content_type, int content_length)
{
	static char header[MAX_HEADER_SIZE];
	sprintf(header, "HTTP/1.1 %s\r\nContent-Type : %s\r\nContent-Length : %d\r\n\r\n",
			err_codes[code], content_type, content_length);
	return (const char *)header;
}

/******************************************************************************
 * Wrapper around the FATFS reading functions. Reads a specified file on the
 * mounted SD card.
 *****************************************************************************/
UINT read_file(char *path, char *buffer, UINT size)
{
	FIL fil;
	UINT bytes_read;
	f_open(&fil, path, FA_READ);
	f_read(&fil, buffer, size, &bytes_read);
	f_close(&fil);

	return bytes_read;
}
