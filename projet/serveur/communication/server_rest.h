#ifndef SERVER_REST_H_
#define SERVER_REST_H_

enum request_type
{
	NEW_GAME,
	MOVE,
	PROMOTE,
	GET_TIME,
	GET_SUMMARY,
	GET_BOARD,
	POST_BOARD,
	GET_DETAILS,
	START,
	END,
	ERROR
};

int REST_handle_request(enum request_type type, const char *data);

#endif /* SERVER_HTTP_IN_H_ */
