

#include "compat.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
//#include <sysinfoapi.h>
#else
#include <sys/time.h>
#define SIZE_MAX 4294967295u
#endif

#define SIZE_T_CEILING ((size_t)(SIZE_MAX-16))

char *smp_strndup(const char *s, size_t n)
{
  char *dup;
  dup = (char*)malloc((n+1));
  strncpy(dup, s, n);
  dup[n]='\0';
  return dup;
}

char *smp_strdup(const char *s){
    size_t n=strlen(s);
    return smp_strndup(s,n);
}

size_t smp_strlcpy(char* dst,const char* src,size_t siz){
    register char* d=dst;
    register const char* s=src;
    register size_t n=siz;

    if(n==0) return strlen(s);
    while(*s!='\0'){
        if(n!=1){
            *d++=*s;
            --n;
        }
        s++;
    }
    *d='\0';
    return (s-src);
}

void smp_strstrip(char *s, const char *strip){
  char *read = s;
  while (*read) {
    if (strchr(strip, *read)) {
      ++read;
    } else {
      *s++ = *read++;
    }
  }
  *s = '\0';
}

void smp_gettimeofday(struct timeval* ptimeval){
#ifdef _WIN32
#ifdef _MSC_VER
#define U64_LITERAL(n) n##ui64
#else
#define U64_LITERAL(n) n##llu
#endif
#define EPOCH_BIAS U64_LITERAL(116444736000000000)
#define UNITS_PER_SEC U64_LITERAL(10000000)
#define USEC_PER_SEC U64_LITERAL(1000000)
#define UNITS_PER_USEC U64_LITERAL(10)
  union {
    uint64_t ft_64;
    FILETIME ft_ft;
  } ft;
  GetSystemTimeAsFileTime(&ft.ft_ft);
  if (ft.ft_64 < EPOCH_BIAS) {
    printf("System time is before 1970; failing.");
    exit(1);
  }
  ft.ft_64 -= EPOCH_BIAS;
  ptimeval->tv_sec = (unsigned) (ft.ft_64 / UNITS_PER_SEC);
  ptimeval->tv_usec = (unsigned) ((ft.ft_64 / UNITS_PER_USEC) % USEC_PER_SEC);
#else
    if(gettimeofday(ptimeval,NULL)){
        printf("gettimeofday failed.");
        exit(1);
    }
#endif
}


int smp_snprintf(char* str,size_t size,const char* format,...){
    va_list ap;
    int r;
    va_start(ap,format);
    r=smp_vsnprintf(str,size,format,ap);
    va_end(ap);
    return r;
}

int smp_vsnprintf(char* str,size_t size,const char* format,va_list args){
    int r;
    if(0==size) return -1;
    if(size>SIZE_T_CEILING) return -1;

#ifdef _WIN32
    r=_vsnprintf(str,size,format,args);
#else
    r=vsnprintf(str,size,format,args);
#endif
    str[size-1]=0;
    if(r<0||r>=size)
        return -1;
    return r;

}


int smp_sunday_search(const char* s1,const char* s2){
    int nlen1=strlen(s1),nlen2=strlen(s2);
    int i=0,j=0,ind=0;

    while(ind+nlen2<=nlen1){

        for(i=ind;j<nlen2;++i,++j){
            if(s1[i]==s2[j])
                continue;
            else
                break;
        }

        if(j==nlen2) 
            return ind;
        else{
            int k=nlen2-1;
            const char end=s1[ind+nlen2];
            while(k>=0){
                if(s2[k]==end) break;
                --k;
            }

            j=0;

            if(k>0) ind=ind+nlen2-k;
            else ind=ind+nlen2+1;
        }

    }
    return -1;
}