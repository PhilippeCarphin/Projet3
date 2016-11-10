/*
 * test_http.h
 *
 *  Created on: 2016-11-01
 *      Author: frouee
 */

#ifndef TEST_HTTP_H_
#define TEST_HTTP_H_

/* One function to run them all */
void test_http_all();

/* Invalid requests */
void test_empty();
void test_invalid_version();
void test_bad_request();

/* Complete features */
void test_new_game();
void test_start();
void test_get_board();
void test_get_summary();
void test_get_details();
void test_post_board();
void test_end();

/* Not yet complete */
void test_move();
void test_promote();
void test_get_time();

/* More precise testing */
void test_game();
void test_movePlayer1(char* coord);
void test_movePlayer2(char* coord);

#endif /* TEST_HTTP_H_ */
