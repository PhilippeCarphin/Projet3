/*
 * server_http_in.h
 *
 * Interface used by TCP module to send HTTP requests
 * and receive HTTP responses.
 *
 * Request is sent to REST module, then to Chessboard.
 */

#ifndef SERVER_HTTP_IN_H_
#define SERVER_HTTP_IN_H_

int HTTP_dispatchRequest(const char *request, char *HTTP_response);

#endif /* SERVER_HTTP_IN_H_ */
