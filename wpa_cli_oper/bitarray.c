

#include "bitarray.h"
#include <stdlib.h>
#include <string.h>

//��ʼ��λ����
bitarray_t* bitarray_init_zero(unsigned int n_bits){
    //�����n_bitsλ������Ҫ���ٸ�unsigned int Ԫ�����Դ��
    unsigned int size=(n_bits+BITARRAY_MASK)>>BITARRAY_SHIFT;
    bitarray_t *ret=(bitarray_t*)malloc(size*sizeof(unsigned int));
    memset(ret,0,size);
    return ret;
}

//��չλ����
bitarray_t* bitarray_expand(bitarray_t* ba,unsigned int n_bits_old,unsigned int n_bits_new){
    size_t sz_old=(n_bits_old+BITARRAY_MASK)>>BITARRAY_SHIFT;
    size_t sz_new=(n_bits_new+BITARRAY_MASK)>>BITARRAY_SHIFT;
    char* ptr;
    if(sz_new<=sz_old) return ba;
    ptr=(char*)realloc(ba,sz_new*sizeof(unsigned int));
    memset(ptr+sz_old*sizeof(unsigned int),0,(sz_new-sz_old)*sizeof(unsigned int));
    return (bitarray_t*)ptr;
}

//�ͷ�λ����
void bitarray_free(bitarray_t* ba){
    free(ba);
}

//����λ����ba��bitԪ��Ϊtrue
void bitarray_set(bitarray_t *ba,int bit){
    ba[bit>>BITARRAY_SHIFT]    |=(1u<<(bit & BITARRAY_MASK));
}

//����λ����ba��bitԪ��Ϊfalse
void bitarray_clear(bitarray_t *ba,int bit){
    ba[bit>>BITARRAY_SHIFT] &=~(1u<<(bit & BITARRAY_MASK));
}

unsigned int bitarray_is_set(bitarray_t* ba,int bit){
    return ba[bit>>BITARRAY_SHIFT] & (1u<<(bit & BITARRAY_MASK));
}

const uint32_t SMP_ISALPHA_TABLE[8] =
  { 0, 0, 0x7fffffe, 0x7fffffe, 0, 0, 0, 0 };
const uint32_t SMP_ISALNUM_TABLE[8] =
  { 0, 0x3ff0000, 0x7fffffe, 0x7fffffe, 0, 0, 0, 0 };
const uint32_t SMP_ISSPACE_TABLE[8] = { 0x3e00, 0x1, 0, 0, 0, 0, 0, 0 };
const uint32_t SMP_ISXDIGIT_TABLE[8] =
  { 0, 0x3ff0000, 0x7e, 0x7e, 0, 0, 0, 0 };
const uint32_t SMP_ISDIGIT_TABLE[8] = { 0, 0x3ff0000, 0, 0, 0, 0, 0, 0 };
const uint32_t SMP_ISPRINT_TABLE[8] =
  { 0, 0xffffffff, 0xffffffff, 0x7fffffff, 0, 0, 0, 0x0 };
const uint32_t SMP_ISUPPER_TABLE[8] = { 0, 0, 0x7fffffe, 0, 0, 0, 0, 0 };
const uint32_t SMP_ISLOWER_TABLE[8] = { 0, 0, 0, 0x7fffffe, 0, 0, 0, 0 };

const char SMP_TOUPPER_TABLE[256] = {
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
  16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
  32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
  48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
  64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
  80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
  96,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
  80,81,82,83,84,85,86,87,88,89,90,123,124,125,126,127,
  128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
  160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
  176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
  192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
  208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
  224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
  240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
};
const char SMP_TOLOWER_TABLE[256] = {
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
  16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
  32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
  48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
  64,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
  112,113,114,115,116,117,118,119,120,121,122,91,92,93,94,95,
  96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
  112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
  128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
  160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
  176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
  192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
  208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
  224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
  240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
};

