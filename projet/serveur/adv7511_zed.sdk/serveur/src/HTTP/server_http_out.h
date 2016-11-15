/*
 * server_http_out.h
 *
 * Interface used by server_http_in to build its responses.
 */

#ifndef SERVER_HTTP_OUT_H_
#define SERVER_HTTP_OUT_H_

void HTTP_build_from_REST(const char *rest, char* http);
void HTTP_build_from_code(int code, char *http);

#endif /* SERVER_HTTP_OUT_H_ */
