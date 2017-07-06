

#ifndef SMP_BITARRAY_H
#define SMP_BITARRAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INLINE
    #ifdef _WIN32
        #define INLINE __inline
    #else
        #define INLINE inline
    #endif
#endif

//�����������SIZE_INT �Ĵ�СΪsizeof(int)�Ĵ�С,��Ҫ���ݲ�ͬ��ƽ̨�����޸�
#define SIZE_INT 4

#if SIZE_INT == 4
#define BITARRAY_SHIFT 5
#elif SIZE_INT == 8
#define BITARRAY_SHIFT 6
#else
#error "int is neither 4 nor 8 bytes."
#endif

//bitarray_mask Ϊ31����63
#define BITARRAY_MASK ((1u<<BITARRAY_SHIFT)-1)

typedef unsigned int bitarray_t;

//��ʼ��λ����
bitarray_t* bitarray_init_zero(unsigned int n_bits);

//��չλ����
bitarray_t* bitarray_expand(bitarray_t* ba,unsigned int n_bits_old,unsigned int n_bits_new);

//�ͷ�λ����
void bitarray_free(bitarray_t* ba);

//����λ����ba��bitԪ��Ϊtrue
void bitarray_set(bitarray_t *ba,int bit);

//����λ����ba��bitԪ��Ϊfalse
void bitarray_clear(bitarray_t *ba,int bit);

unsigned int bitarray_is_set(bitarray_t* ba,int bit);


//����Ǹ���bitarray��˼��ʵ�ֵ�һЩ�򵥹��ܺ���
#define DECLARE_CTYPE_FN(name)                                          \
  static int SMP_##name(char c);                                        \
  extern const uint32_t SMP_##name##_TABLE[];                           \
  static INLINE int SMP_##name(char c) {                                \
    uint8_t u = c;                                                      \
    return !!(SMP_##name##_TABLE[(u >> 5) & 7] & (1u << (u & 31)));     \
  }
  
DECLARE_CTYPE_FN(ISALPHA)
DECLARE_CTYPE_FN(ISALNUM)
DECLARE_CTYPE_FN(ISSPACE)
DECLARE_CTYPE_FN(ISDIGIT)
DECLARE_CTYPE_FN(ISXDIGIT)
DECLARE_CTYPE_FN(ISPRINT)
DECLARE_CTYPE_FN(ISLOWER)
DECLARE_CTYPE_FN(ISUPPER)
extern const char SMP_TOUPPER_TABLE[];
extern const char SMP_TOLOWER_TABLE[];
#define SMP_TOLOWER(c) (SMP_TOLOWER_TABLE[(uint8_t)c])
#define SMP_TOUPPER(c) (SMP_TOUPPER_TABLE[(uint8_t)c])

#ifdef __cplusplus
}
#endif

#endif