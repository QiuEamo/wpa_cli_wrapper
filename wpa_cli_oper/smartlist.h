

#ifndef SMP_SMARTLIST_H
#define SMP_SMARTLIST_H

//�����Ѿ���win7 �� linux �ϲ��Թ�

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <stdint.h>
#else
#include <sys/types.h>
#endif

#ifndef INLINE
    #ifdef _WIN32
        #define INLINE __inline
    #else
        #define INLINE inline
    #endif
#endif

#define STRUCT_VAR_P(st, off) ((void*) ( ((char*)(st)) + (off) ) )
#define STRUCT_VAR_OFFSET(st,p) ((off_t)((char*)&(((st*)0)->p)-(char*)0))

typedef struct smartlist_t {
  void **list;
  int num_used;
  int capacity;
} smartlist_t;

//������������
smartlist_t * smartlist_new(void);

//�ͷ���������
void smartlist_free(smartlist_t *sl);

//�������
void smartlist_clear(smartlist_t *sl);

//��ָ��Ԫ����ӵ����
void smartlist_add(smartlist_t *sl, void *element);

//��s2�е�����Ԫ����ӵ�s1��
void smartlist_add_all(smartlist_t *sl, const smartlist_t *s2);

//�Ƴ�ָ��Ԫ��
void smartlist_remove(smartlist_t *sl, const void *element);

//�������һ��Ԫ��
void *smartlist_pop_last(smartlist_t *sl);

//sl��Ԫ�ص���
void smartlist_reverse(smartlist_t *sl);

//�Ƴ�sl��ָ�����ַ���
void smartlist_string_remove(smartlist_t *sl, const char *element);

//�ж��Ƿ����ָ��Ԫ��
int smartlist_contains(const smartlist_t *sl, const void *element);

//�Ƿ����ָ���ַ���
int smartlist_contains_string(const smartlist_t *sl, const char *element);

//�õ��ַ����������е�λ��
int smartlist_string_pos(const smartlist_t *, const char *elt);

//���Դ�Сд,�ж��Ƿ����ָ���ַ���
int smartlist_contains_string_case(const smartlist_t *sl, const char *element);

////�ж϶����Ѿ������sl1��sl2��ŵ��ַ��������Ƿ���ͬ
int smartlist_strings_eq(const smartlist_t *sl1, const smartlist_t *sl2);

//�ж��Ƿ����ָ�����ڴ������,
int smartlist_contains_buf(const smartlist_t *sl, const char *element,int len);

//�ж϶����Ѿ������sl1��sl2��ŵ����������Ƿ���ͬ
int smartlist_ints_eq(const smartlist_t *sl1, const smartlist_t *sl2);

//�ж�sl1��sl2�Ƿ������ͬ��Ԫ��
int smartlist_overlap(const smartlist_t *sl1, const smartlist_t *sl2);

//�Ƴ�sl1�еĲ���sl2�е�Ԫ��,���������Ķ�����sl2�д���
void smartlist_intersect(smartlist_t *sl1, const smartlist_t *sl2);

//�Ƴ�sl1����sl2��Ҳ���ڵ�Ԫ��,���������Ķ�����sl2��
void smartlist_subtract(smartlist_t *sl1, const smartlist_t *sl2);

#define smartlist_len(sl) ((sl)->num_used)
#define smartlist_get(sl, idx) ((sl)->list[idx])
#define smartlist_set(sl, idx, val) ((sl)->list[idx] = (val))

static INLINE void smartlist_swap(smartlist_t *sl, int idx1, int idx2)
{
  if (idx1 != idx2) {
    void *elt = smartlist_get(sl, idx1);
    smartlist_set(sl, idx1, smartlist_get(sl, idx2));
    smartlist_set(sl, idx2, elt);
  }
}

//ɾ��ָ���±��Ԫ��
void smartlist_del(smartlist_t *sl, int idx);

//�����Ѿ������sl,ɾ��ָ���±��Ԫ��,ɾ��֮����Ȼ����
void smartlist_del_keeporder(smartlist_t *sl, int idx);

//��ָ��Ԫ�ز��뵽ָ���±�λ��
void smartlist_insert(smartlist_t *sl, int idx, void *val);

//��������������ݽ�������
void smartlist_sort(smartlist_t *sl,
                    int (*compare)(const void **a, const void **b));

//ͳ�Ƴ�������Ԫ��,�����س��ֵĴ���
void *smartlist_get_most_frequent_(const smartlist_t *sl,
                    int (*compare)(const void **a, const void **b),
                    int *count_out);

#define smartlist_get_most_frequent(sl, compare) \
  smartlist_get_most_frequent_((sl), (compare), NULL)

//ȥ���������ظ���Ԫ��
void smartlist_uniq(smartlist_t *sl,
                    int (*compare)(const void **a, const void **b),
                    void (*free_fn)(void *elt));

//���ַ��������������
void smartlist_sort_strings(smartlist_t *sl);

//����ָ���С��������
void smartlist_sort_pointers(smartlist_t *sl);

//ͳ�Ƴ������ε��ַ���
char *smartlist_get_most_frequent_string(smartlist_t *sl);

//ͳ�Ƴ������ε��ַ���,�����س��ֵĴ���
char *smartlist_get_most_frequent_string_(smartlist_t *sl, int *count_out);


//ȥ���ظ����ֵ��ַ���
void smartlist_uniq_strings(smartlist_t *sl);

//���������sl,ͨ��key�ؼ��ֽ��ж��ֲ���,���ҵ���Ԫ��ͨ��member����
void *smartlist_bsearch(smartlist_t *sl, const void *key,
                        int (*compare)(const void *key, const void **member));

//���������sl,ͨ��key�ؼ��ֽ��ж��ֲ���,�����ҵ���Ԫ���������е�λ��
int smartlist_bsearch_idx(const smartlist_t *sl, const void *key,
                          int (*compare)(const void *key, const void **member),
                          int *found_out);

//��Ԫ�ؼ��뵽����
void smartlist_pqueue_add(smartlist_t *sl,
                          int (*compare)(const void *a, const void *b),
                          int idx_field_offset,
                          void *item);

//�Ƴ��Զ�Ԫ��
void *smartlist_pqueue_pop(smartlist_t *sl,
                           int (*compare)(const void *a, const void *b),
                           int idx_field_offset);

//�Ƴ�����ָ��Ԫ��
void smartlist_pqueue_remove(smartlist_t *sl,
                             int (*compare)(const void *a, const void *b),
                             int idx_field_offset,
                             void *item);


#define SPLIT_SKIP_SPACE   0x01
#define SPLIT_IGNORE_BLANK 0x02
#define SPLIT_STRIP_SPACE  0x04


//��sep�ָ��ַ���str,���ָ�������ַ���������뵽sl��
int smartlist_split_string(smartlist_t *sl, const char *str, const char *sep,
                           int flags, int max);

//��sl�е��ַ����������ӳ�Ϊһ���µ��ַ���
char * smartlist_join_strings(smartlist_t *sl, const char *join, int terminate,
                              size_t *len_out);

//��sl�е��ַ����������ӳ�Ϊһ���µ��ַ���
char *smartlist_join_strings2(smartlist_t *sl, const char *join,
                              size_t join_len, int terminate, size_t *len_out);


#define STMT_NIL (void)0
#define STMT_VOID(a) while (0) { (void)(a); }

#ifdef __GNUC__
    #define STMT_BEGIN (void) ({
    #define STMT_END })
#elif defined(sun) || defined(__sun__)
    #define STMT_BEGIN if (1) {
    #define STMT_END } else STMT_NIL
#else
    #define STMT_BEGIN do {
    #define STMT_END } while (0)
#endif


#define SMARTLIST_FOREACH_BEGIN(sl, type, var)  \
  STMT_BEGIN                                                           \
    int var ## _sl_idx,var## _cur_del=0,var ## _sl_len=(sl)->num_used; \
    type var;                                                          \
    for (var ## _sl_idx = 0; var ## _sl_idx < var ## _sl_len;          \
         ++var ## _sl_idx) {                                           \
         var ## _cur_del=0;                                            \
         var = (type)(sl)->list[var ## _sl_idx];



#define SMARTLIST_REMOVE_CUR(var) \
    var##_cur_del=1;



#define SMARTLIST_FOREACH_END(var)              \
    var = NULL;                                 \
  }                                             \
  STMT_END




#define SMARTLIST_FOREACH(sl, type, var, cmd)                   \
  SMARTLIST_FOREACH_BEGIN(sl,type,var) {                        \
    cmd;                                                        \
    if(var ##_cur_del){                                         \
        smartlist_remove((sl),var);                             \
        --var ## _sl_idx;                                       \
    }                                                           \
    var ## _sl_len=(sl)->num_used;                              \
  } SMARTLIST_FOREACH_END(var)

#ifdef __cplusplus
}
#endif

#endif
