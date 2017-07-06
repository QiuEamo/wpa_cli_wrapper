

#ifndef SMP_SMARTLIST_H
#define SMP_SMARTLIST_H

//功能已经在win7 和 linux 上测试过

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

//构造容器对象
smartlist_t * smartlist_new(void);

//释放容器对象
void smartlist_free(smartlist_t *sl);

//清空容器
void smartlist_clear(smartlist_t *sl);

//将指定元素添加到最后
void smartlist_add(smartlist_t *sl, void *element);

//将s2中的所有元素添加到s1中
void smartlist_add_all(smartlist_t *sl, const smartlist_t *s2);

//移除指定元素
void smartlist_remove(smartlist_t *sl, const void *element);

//弹出最后一个元素
void *smartlist_pop_last(smartlist_t *sl);

//sl中元素倒序
void smartlist_reverse(smartlist_t *sl);

//移除sl中指定的字符串
void smartlist_string_remove(smartlist_t *sl, const char *element);

//判断是否包含指定元素
int smartlist_contains(const smartlist_t *sl, const void *element);

//是否包含指定字符串
int smartlist_contains_string(const smartlist_t *sl, const char *element);

//得到字符串在容器中的位置
int smartlist_string_pos(const smartlist_t *, const char *elt);

//忽略大小写,判断是否包含指定字符串
int smartlist_contains_string_case(const smartlist_t *sl, const char *element);

////判断对于已经排序的sl1和sl2存放的字符串数组是否相同
int smartlist_strings_eq(const smartlist_t *sl1, const smartlist_t *sl2);

//判断是否包含指定的内存的内容,
int smartlist_contains_buf(const smartlist_t *sl, const char *element,int len);

//判断对于已经排序的sl1和sl2存放的整数数组是否相同
int smartlist_ints_eq(const smartlist_t *sl1, const smartlist_t *sl2);

//判断sl1和sl2是否包含相同的元素
int smartlist_overlap(const smartlist_t *sl1, const smartlist_t *sl2);

//移除sl1中的不在sl2中的元素,保留下来的都能在sl2中存在
void smartlist_intersect(smartlist_t *sl1, const smartlist_t *sl2);

//移除sl1中在sl2中也存在的元素,保留下来的都不在sl2中
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

//删除指定下标的元素
void smartlist_del(smartlist_t *sl, int idx);

//对于已经排序的sl,删除指定下标的元素,删除之后仍然有序
void smartlist_del_keeporder(smartlist_t *sl, int idx);

//将指定元素插入到指定下标位置
void smartlist_insert(smartlist_t *sl, int idx, void *val);

//对容器里面的内容进行排序
void smartlist_sort(smartlist_t *sl,
                    int (*compare)(const void **a, const void **b));

//统计出现最多的元素,并返回出现的次数
void *smartlist_get_most_frequent_(const smartlist_t *sl,
                    int (*compare)(const void **a, const void **b),
                    int *count_out);

#define smartlist_get_most_frequent(sl, compare) \
  smartlist_get_most_frequent_((sl), (compare), NULL)

//去除容器中重复的元素
void smartlist_uniq(smartlist_t *sl,
                    int (*compare)(const void **a, const void **b),
                    void (*free_fn)(void *elt));

//对字符串数组进行排序
void smartlist_sort_strings(smartlist_t *sl);

//按照指针大小进行排序
void smartlist_sort_pointers(smartlist_t *sl);

//统计出现最多次的字符串
char *smartlist_get_most_frequent_string(smartlist_t *sl);

//统计出现最多次的字符串,并返回出现的次数
char *smartlist_get_most_frequent_string_(smartlist_t *sl, int *count_out);


//去除重复出现的字符串
void smartlist_uniq_strings(smartlist_t *sl);

//对于排序的sl,通过key关键字进行二分查找,将找到的元素通过member返回
void *smartlist_bsearch(smartlist_t *sl, const void *key,
                        int (*compare)(const void *key, const void **member));

//对于排序的sl,通过key关键字进行二分查找,返回找到的元素在容器中的位置
int smartlist_bsearch_idx(const smartlist_t *sl, const void *key,
                          int (*compare)(const void *key, const void **member),
                          int *found_out);

//将元素加入到堆中
void smartlist_pqueue_add(smartlist_t *sl,
                          int (*compare)(const void *a, const void *b),
                          int idx_field_offset,
                          void *item);

//移除对顶元素
void *smartlist_pqueue_pop(smartlist_t *sl,
                           int (*compare)(const void *a, const void *b),
                           int idx_field_offset);

//移除堆中指定元素
void smartlist_pqueue_remove(smartlist_t *sl,
                             int (*compare)(const void *a, const void *b),
                             int idx_field_offset,
                             void *item);


#define SPLIT_SKIP_SPACE   0x01
#define SPLIT_IGNORE_BLANK 0x02
#define SPLIT_STRIP_SPACE  0x04


//用sep分割字符串str,将分割产生的字符串数组存入到sl中
int smartlist_split_string(smartlist_t *sl, const char *str, const char *sep,
                           int flags, int max);

//将sl中的字符串数组连接成为一个新的字符串
char * smartlist_join_strings(smartlist_t *sl, const char *join, int terminate,
                              size_t *len_out);

//将sl中的字符串数组连接成为一个新的字符串
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
