#ifndef SERVER_HTTP_OUT_H_
#define SERVER_HTTP_OUT_H_

void HTTP_REST_to_HTTP(const char *rest, char* http);
void HTTP_code_to_HTTP(int code, char *http);

#endif /* SERVER_HTTP_OUT_H_ */
