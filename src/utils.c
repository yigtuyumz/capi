#include "utils.h"

int
utils_isalnum(int val)
{
    if (utils_isalpha(val) || utils_isdigit(val)) {
        return (1);
    }

    return (0);
}

int
utils_isalpha(int val)
{
    if (utils_isupper(val) || utils_islower(val)) {
        return (1);
    }

    return (0);
}

int
utils_isascii(int val)
{
    unsigned char c = val;

    if (c <= 127) {
        return (1);
    }

    return (0);
}

int
utils_isblank(int val)
{
    unsigned char c = val;

    if (c == '\t' || c == ' ') {
        return (1);
    }

    return (0);
}

int
utils_iscntrl(int val)
{
    unsigned char c = val;

    if (c == 127 || c <= 31) {
        return (1);
    }

    return (0);
}

int
utils_isdigit(int val)
{
    unsigned char c = val;

    if (c >= '0' && c <= '9') {
        return (1);
    }

    return (0);
}

int
utils_isgraph(int val)
{
    unsigned char c = val;

    if (c >= 33 && c <= 126) {
        return (1);
    }

    return (0);
}

int
utils_islower(int val)
{
    unsigned char c = val;

    if (c >= 'a' && c <= 'z') {
        return (1);
    }

    return (0);
}

int
utils_isprime(unsigned int nb)
{
    if (nb <= 1) {
        return (0);
    }

    if (nb <= 3) {
        return (1);
    }

    if (nb % 2 == 0 || nb % 3 == 0) {
        return (0);
    }

    unsigned int i = 5;

    while ((i * i) <= nb) {
        if (((nb % i) == 0) || (nb % (i + 2) == 0)) {
            return (0);
        }

        i += 6;
    }

    return (1);
}

int
utils_isprint(int val)
{
    unsigned char c = val;

    if (c >= 32 && c <= 126) {
        return (1);
    }

    return (0);
}

int
utils_ispunct(int val)
{
    unsigned char c = val;

    if ((c >= 33 && c <= 47) ||
        (c >= 58 && c <= 64) ||
        (c >= 91 && c <= 96) || (c >= 123 && c <= 126)) {
        return (1);
    }

    return (0);
}

int
utils_isspace(int val)
{
    unsigned char c = val;

    if (c == 32 || (c >= 9 && c <= 13)) {
        return (1);
    }

    return (0);
}

int
utils_isupper(int val)
{
    unsigned char c = val;

    if (c >= 'A' && c <= 'Z') {
        return (1);
    }

    return (0);
}

int
utils_isxdigit(int val)
{
    unsigned char c = val;

    if ((c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
        return (1);
    }

    return (0);
}

double
utils_floor(double x)
{
    return ((int) (x + 0x40000000) - 0x40000000);
}

int
utils_constrain(int val, int min, int max)
{
    if (val <= min) {
        return (min);
    } else if (val >= max) {
        return (max);
    }

    return (val);
}

int
utils_sq(void *nb)
{
    return (*((int *) nb) * *((int *) nb));
}

void
utils_bzero(void *s, size_t n)
{
    while (n--) {
        *((unsigned char *) s + n) = 0;
    }
}

void *
utils_memcpy(void *dest, const void *src, size_t n)
{
    while (n--) {
        *((unsigned char *) dest + n) = *((unsigned char *) src + n);
    }

    return (dest);
}

void *
utils_memset(void *dest, int c, size_t n)
{
    while (n--) {
        *((unsigned char *) dest + n) = c;
    }

    return (dest);
}

void
utils_swapn(void *a, void *b, size_t n)
{
    while (n--) {
        *((unsigned char *) a + n) =
            *((unsigned char *) a + n) ^ *((unsigned char *) b + n);
        *((unsigned char *) b + n) =
            *((unsigned char *) a + n) ^ *((unsigned char *) b + n);
        *((unsigned char *) a + n) =
            *((unsigned char *) a + n) ^ *((unsigned char *) b + n);
    }
}

int
utils_atoi(const char *nbr)
{
    int sign = 0;
    int nb = 0;

    while (utils_isspace(*nbr) || *(nbr) == '-' || *(nbr) == '+') {
        if (*nbr == '-') {
            sign++;
        } else if (*nbr == '+') {
            sign--;
        }
        nbr++;
    }

    while (utils_isdigit(*nbr)) {
        nb = (nb * 10) + (*nbr - 48);
        nbr++;
    }

    if (sign > 0) {
        return (-nb);
    }

    return (nb);
}

char *
utils_itoa(int nb)
{
    static char buffer[21];
    char *p = buffer + 20;

    if (nb >= 0) {
        while (nb != 0) {
            *(--p) = '0' + (nb % 10);
            nb /= 10;
        }

        return (p);
    }

    while (nb != 0) {
        *(--p) = '0' - (nb % 10);
        nb /= 10;
    }

    *(--p) = '-';

    return (p);
}

ssize_t
utils_putchar(int fd, char c)
{
    return (write(fd, &c, 1));
}

void
utils_putnbr(int fd, int nb)
{
    if (nb < 0) {
        utils_putchar(fd, '-');
        if (nb == ~0x7FFFFFFF) {
            nb = -147483648;
            utils_putchar(fd, '2');
        }
        nb *= -1;
    }

    if (nb >= 10) {
        utils_putnbr(fd, (nb / 10));
        nb = (nb % 10);
    }

    if (nb < 10) {
        utils_putchar(fd, (nb + 48));
    }
}

ssize_t
utils_putstr(int fd, const char *str)
{
    return (write(fd, str, utils_strlen(str)));
}

void
utils_vaput(int fd, char *fmt, ...)
{
    va_list ap;
    char c;
    char *s;
    int d;

    va_start(ap, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 's') {
                s = va_arg(ap, char *);

                utils_putstr(fd, s);
            } else if (*fmt == 'd') {
                d = va_arg(ap, int);

                utils_putnbr(fd, d);
            } else if (*fmt == 'c') {
                c = va_arg(ap, int);

                utils_putchar(fd, c);
            } else {
                utils_putchar(fd, *fmt);
            }
        } else {
            utils_putchar(fd, *fmt);
        }
        fmt++;
    }
    va_end(ap);
}

char *
utils_strcat(char *dest, const char *src)
{
    size_t i = utils_strlen(dest);

    while (*(src)) {
        *(dest + i++) = *(src++);
    }
    *(dest + i) = 0;

    return (dest);
}

char *
utils_strchr(const char *str, int val)
{
    while (*(str)) {
        if (*(str) == (char) val) {
            return ((char *) str);
        }
        str++;
    }

    return (NULL);
}

int
utils_strcmp(const char *a, const char *b)
{
    size_t i = 0;

    while (*(a + i) && (*(a + i) - *(b + i) == 0)) {
        i++;
    }

    return (*(a + i) - *(b + i));
}

char *
utils_strcpy(char *restrict dest, const char *restrict src)
{
    while (*src) {
        *(dest++) = *(src++);
    }
    *(dest) = 0;

    return (dest);
}

char *
utils_strdup(const char *str)
{
    char *ret_str = (char *) malloc(utils_strlen(str) + 1);

    if (ret_str == NULL) {
        return (NULL);
    }

    size_t counter = 0;

    while (*(str + counter)) {
        *(ret_str + counter) = *(str + counter);
        counter++;
    }

    *(ret_str + counter) = 0;

    return (ret_str);
}

size_t
utils_strlen(const char *str)
{
    size_t len = 0;

    while (*(str + len)) {
        len++;
    }

    return (len);
}

char *
utils_strncat(char *dest, const char *src, size_t n)
{
    size_t i = utils_strlen(dest);
    size_t j = 0;

    while (*(src) && j < n) {
        *(dest + i + j) = *(src + j);
        j++;
    }

    *(dest + i + j) = 0;
    return (dest);
}

int
utils_strncmp(const char *a, const char *b, size_t n)
{
    size_t i = 0;

    while (*(a + i) && (*(a + i) == *(b + i)) && i < n) {
        i++;
    }

    if (i == n) {
        return (0);
    }

    return (*(a + i) - *(b + i));
}

char *
utils_strncpy(char *restrict dest, const char *restrict src, size_t n)
{
    size_t i = 0;

    while (i < n && *(src + i)) {
        *(dest + i) = *(src + i);
        i++;
    }

    while (i < n) {
        *(dest + i) = 0;
        i++;
    }

    return (dest);
}

char *
utils_strndup(const char *str, size_t len)
{
    char *ret_str = (char *) malloc(utils_strnlen(str, len) + 1);

    if (ret_str == NULL) {
        return (NULL);
    }

    size_t counter = 0;

    while (*(str + counter) && counter < len) {
        *(ret_str + counter) = *(str + counter);
        counter++;
    }

    *(ret_str + counter) = 0;

    return (ret_str);
}

size_t
utils_strnlen(const char *str, size_t n)
{
    size_t len = 0;

    while (*(str + len) && len < n) {
        len++;
    }

    return (len);
}

char *
utils_strnstr(const char *haystack, const char *needle, size_t n)
{
    if (*(needle) == 0) {
        return ((char *) haystack);
    }

    size_t i = 0;
    char *a;
    char *b = (char *) needle;

    while (*(haystack) && i < n) {
        a = (char *) haystack;
        while (*(haystack) && *(haystack) == *(needle)) {
            haystack++;
            needle++;
        }
        if (*(needle) == 0) {
            return (a);
        }
        haystack = ++a;
        needle = b;
        i++;
    }

    return (NULL);
}

char *
utils_strrchr(const char *str, int val)
{
    char *s = ((void *) 0);

    while (*(str)) {
        if (*(str) == (char) val) {
            s = (char *) str;
        }
        str++;
    }

    return (s);
}

char *
utils_strstr(const char *haystack, const char *needle)
{
    if (*(needle) == 0) {
        return ((char *) haystack);
    }

    char *h;
    char *n = (char *) needle;

    while (*haystack) {
        h = (char *) haystack;

        while (*(haystack) && *(haystack) == *(needle)) {
            haystack++;
            needle++;
        }

        if (*(needle) == 0) {
            return (h);
        }

        haystack = ++h;
        needle = n;
    }

    return (((void *) 0));
}
