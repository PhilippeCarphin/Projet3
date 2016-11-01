#ifndef JSON_TOKENS_H_
#define JSON_TOKENS_H_

int jsoneq(const char *json, jsmntok_t *tok, const char *s);
int token_to_string(char *dst, const char *data, const jsmntok_t *token);
int token_to_bool(int *dst, const char *data, const jsmntok_t *token);
int token_to_int(int *dst, const char *data, const jsmntok_t *token);

#endif /* JSON_TOKENS_H_ */
