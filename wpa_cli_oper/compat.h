

#ifndef COMPAT_H
#define COMPAT_H

#include <stdarg.h>
#ifdef _WIN32
#include <winsock.h> //timeval
#else
#include <stddef.h>
#include <sys/time.h> //timeval
#endif

#ifdef __cplusplus
extern "C" {
#endif

char *smp_strndup(const char *s, size_t n);

char *smp_strdup(const char *s);

size_t smp_strlcpy(char* dst,const char* src,size_t siz);

void smp_strstrip(char *s, const char *strip);

void smp_gettimeofday(struct timeval* timeval);

int smp_snprintf(char* str,size_t size,const char* format,...);

int smp_vsnprintf(char* str,size_t size,const char* format,va_list args);

int smp_sunday_search(const char* s1,const char* s2);

#ifdef __cplusplus
}
#endif

#endif 
