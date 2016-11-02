/*
 * test_http.h
 *
 *  Created on: 2016-11-01
 *      Author: frouee
 */

#ifndef TEST_HTTP_H_
#define TEST_HTTP_H_

/* Invalid requests */
void test_empty();
void test_invalid_version();
void test_bad_request();

/* Complete features */
void test_new_game();
void test_get_board();

/* Not yet complete */
void test_start();
void test_move();
void test_promote();
void test_get_time();
void test_get_summary();
void test_post_board();
void test_get_details();
void test_end();

#endif /* TEST_HTTP_H_ */
