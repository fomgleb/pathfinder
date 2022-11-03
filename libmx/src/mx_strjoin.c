#include "../inc/libmx.h"

char *mx_strjoin(char const *s1, char const *s2) {
	if (s1 == NULL && s2 != NULL) return mx_strdup(s2);
	if (s1 != NULL && s2 == NULL) return mx_strdup(s1);
	if (s1 == NULL && s2 == NULL) return NULL;
	char *new_str = mx_strdup(s1);
	if (new_str == NULL) return NULL;
	return mx_strcat(new_str, s2);
}

