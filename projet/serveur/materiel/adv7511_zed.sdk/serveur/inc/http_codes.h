/*
 * http_codes.h
 *
 *  Created on: 2016-11-01
 *      Author: frouee
 */

#ifndef HTTP_CODES_H_
#define HTTP_CODES_H_

enum HTTP_CODE { 	HTTP_OK = 200,
					HTTP_BAD_REQUEST = 400,
					HTTP_FORBIDDEN = 403,
					HTTP_NOT_FOUND = 404,
					HTTP_NOT_ACCEPTABLE = 406,
					HTTP_REQUEST_TIMEOUT = 408,
					HTTP_INTERNAL_SERVER_ERROR = 500,
					HTTP_NOT_IMPLEMENTED = 501,
					HTTP_VERSION_NOT_SUPPORTED = 505
};

#endif /* HTTP_CODES_H_ */
