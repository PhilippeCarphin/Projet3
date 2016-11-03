#include "json_tokens.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/******************************************************************************
 * Compares the content of a token with a string. Returns 0 if found.
 * Implementation is taken from one of jsmn library's examples.
 *****************************************************************************/
int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

/******************************************************************************
 * Converts a token into a string
 *****************************************************************************/
int token_to_string(char *dst, const char *data, const jsmntok_t *token)
{
	return sprintf(dst, "%.*s", token->end - token->start, data + token->start);
}

/******************************************************************************
 * Converts a token into a c boolean (0 for false, 1 for true)
 *****************************************************************************/
int token_to_bool(int *dst, const char *data, const jsmntok_t *token)
{
	char boolean[4];	/* json bool is yes\0 or no\0 */
	int len = sprintf(boolean, "%.*s", token->end - token->start, data + token->start);
	
	if (strcmp(boolean, "yes") == 0)
		*dst = 1;
	else if (strcmp(boolean, "no") == 0)
		*dst = 0;
	
	return len;
}

/******************************************************************************
 * Converts a token into an integer
 *****************************************************************************/
int token_to_int(int *dst, const char *data, const jsmntok_t *token)
{	
	char integer[10];	/* should not be very high */
	int len = sprintf(integer, "%.*s", token->end - token->start, data + token->start);
	*dst = (int)strtol(integer, (char **)NULL, 10);
	
	return len;
}
