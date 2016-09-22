/*
 * server_protocol.h
 *
 *  Created on: 2016-09-19
 *      Author: sabark
 */

#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

const char *decode_request(const char *request, int *response_length);

#endif /* SERVER_PROTOCOL_H_ */
