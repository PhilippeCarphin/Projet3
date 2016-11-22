/*
 * test_http.h
 *
 *  Created on: 2016-11-01
 *      Author: frouee
 */

#ifndef TEST_HTTP_H_
#define TEST_HTTP_H_

/* Function used by all others; could be used in main for more control over testing */
void test_http(const char *test_name, const char *request, const char *expected, int show_details);

/* One function to run them all */
void test_http_all();

/* Invalid requests */
void test_empty(int details);
void test_invalid_version(int details);
void test_bad_request(int details);

/* Complete features */
void test_new_game(int details);
void test_start(int details);
void test_get_board();
void test_get_summary();
void test_get_details();
void test_move();
void test_post_board();
void test_end();

/* Not yet complete */
void test_promote();
void test_get_time();

/* More precise testing */
void test_game();
void test_pawns();
void test_knight();
void test_bishop();
void test_rook();
void test_queen();
void test_king();
void test_movePlayer1(char* coord);
void test_movePlayer2(char* coord);
void test_full_promotion();

#endif /* TEST_HTTP_H_ */
