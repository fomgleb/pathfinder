int mx_strcmp(const char *s1, const char *s2) {
    for (int i = 0; s1[i] != '\0' || s2[i] != '\0'; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

// #include <stdio.h>
// #include <string.h>
// int main() {
//     char str1[] = "sde2";
//     char str2[] = "sdes";
//     printf("mine: %d\n", mx_strcmp(str1, str2));
//     printf("syst: %d\n", strcmp(str1, str2));
//     return 0;
// }

