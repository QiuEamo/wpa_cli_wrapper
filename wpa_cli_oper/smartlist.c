
#include "smartlist.h"
#include <stdlib.h>
#include <string.h>
#include "bitarray.h"
#include "compat.h"

#define SMARTLIST_DEFAULT_CAPACITY 16

smartlist_t * smartlist_new(void){
    smartlist_t *sl=(smartlist_t*)malloc(sizeof(smartlist_t));
    sl->num_used=0;
    sl->capacity=SMARTLIST_DEFAULT_CAPACITY;
    sl->list=(void**)malloc(sizeof(void*)*sl->capacity);
    memset(sl->list,0,sizeof(void*)*sl->capacity);
    return sl;
}

void smartlist_free(smartlist_t * sl){
    if(!sl) return;
    free(sl->list);
    free(sl);
}

void smartlist_clear(smartlist_t *sl){
    sl->num_used=0;
}

//内存自动扩张
static INLINE void smartlist_ensure_capacity(smartlist_t *sl,int size){
    if(size>sl->capacity){
        int higher=sl->capacity;
        while(size>higher)
            higher*=2;
        
        sl->capacity=higher;
        sl->list=(void**)realloc(sl->list,sizeof(void*)*(sl->capacity));    
    }    
}

void smartlist_add(smartlist_t *sl,void * element){
    smartlist_ensure_capacity(sl,sl->num_used+1);
    sl->list[sl->num_used++]=element;
}

void smartlist_add_all(smartlist_t *sl,const smartlist_t *s2){
    int new_size=sl->num_used+s2->num_used;
    smartlist_ensure_capacity(sl,new_size);
    memcpy(sl->list+sl->num_used,s2->list,s2->num_used*sizeof(void*));
    sl->num_used=new_size;
}

void smartlist_remove(smartlist_t *sl,const void *element){
    int i;
    if(element==NULL) return;
    for(i=0;i<sl->num_used;++i){
        if(sl->list[i]==element){
            sl->list[i]=sl->list[--sl->num_used];
            i--;
        }
    }
}

void *smartlist_pop_last(smartlist_t *sl){
    if(sl->num_used)
        return sl->list[--sl->num_used];
    else
        return NULL;
}

void smartlist_reverse(smartlist_t *sl){
    int i,j;
    void *tmp;
    for(i=0,j=sl->num_used-1;i<j;++i,--j){
        tmp=sl->list[i];
        sl->list[i]=sl->list[j];
        sl->list[j]=tmp;
    }
}

void smartlist_string_remove(smartlist_t *sl,const char *element){
    int i;
    for(i=0;i<sl->num_used;++i){
        if(!strcmp(element,(const char*)sl->list[i])){
            free(sl->list[i]);
            sl->list[i]=sl->list[--sl->num_used];
            i--;
        }
    }
}

int smartlist_contains(const smartlist_t *sl,const void *element){
    int i;
    if(!sl) return 0;
    for(i=0;i<sl->num_used;i++)
        if(sl->list[i]==element)
            return 1;
    return 0;
}

int smartlist_contains_string(const smartlist_t *sl,const char *element){
    int i;
    if(!sl) return 0;
    for(i=0;i<sl->num_used;i++)
        if(strcmp((const char*)sl->list[i],element)==0)
            return 1;
    return 0;
}

int smartlist_string_pos(const smartlist_t *sl,const char *element){
    int i;
    if(!sl) return -1;
    for(i=0;i<sl->num_used;i++)
        if(strcmp((const char*)sl->list[i],element)==0)
            return i;
    return -1;
}

#ifdef _WIN32
    #define strncasecmp _strnicmp
    #define strcasecmp _stricmp
#endif

int smartlist_contains_string_case(const smartlist_t *sl,const char *element){
    int i;
    if(!sl) return 0;
    for(i=0;i<sl->num_used;i++)
        if(strcasecmp((const char*)sl->list[i],element)==0)
            return 1;
    return 0;
}

int smartlist_strings_eq(const smartlist_t *sl1,const smartlist_t *sl2){
    if(sl1==NULL)
        return sl2==NULL;
    if(sl2==NULL)
        return 0;
    if(smartlist_len(sl1)!=smartlist_len(sl2))
        return 0;
    SMARTLIST_FOREACH(sl1,const char *,cp1,{
        const char *cp2=smartlist_get(sl2,cp1_sl_idx);
        if(strcmp(cp1,cp2))
            return 0;
    });
    return 1;
}

int smartlist_ints_eq(const smartlist_t *sl1,const smartlist_t *sl2){
    if(sl1==NULL)
        return sl2==NULL;
    if(sl2==NULL)
        return 0;
        
    if(smartlist_len(sl1)!=smartlist_len(sl2))
        return 0;
        
    SMARTLIST_FOREACH(sl1,int * ,cp1,{
        int *cp2=smartlist_get(sl2,cp1_sl_idx);
        if(*cp1!=*cp2)
            return 0;
    });
    return 1;
}

int smartlist_contains_buf(const smartlist_t *sl, const char *element,int len){
    int i;
    if(!sl) return 0;
    for(i=0;i<sl->num_used;i++)
        if(memcmp(sl->list[i],element,len)==0)
            return 1;
    return 0;
}

//判断sl1和sl2中是否有相同的部分
int smartlist_overlap(const smartlist_t *sl1,const smartlist_t *sl2){
    int i;
    for(i=0;i<sl2->num_used;i++)
        if(smartlist_contains(sl1,sl2->list[i]))
            return 1;
    return 0;
}

//移除sl1中的不在sl2中的元素,保留下来的都能在sl2中存在
void smartlist_intersect(smartlist_t *sl1,const smartlist_t *sl2){
    int i;
    for(i=0;i<sl1->num_used;i++){
        if(!smartlist_contains(sl2,sl1->list[i])){
            sl1->list[i]=sl1->list[--sl1->num_used];
            i--;
        }
    }
}

//移除sl1中在sl2中也存在的元素,保留下来的都不在sl2中
void smartlist_subtract(smartlist_t *sl1,const smartlist_t *sl2){
    int i;
    for(i=0;i<sl2->num_used;i++)
        smartlist_remove(sl1,sl2->list[i]);
}

void smartlist_del(smartlist_t *sl, int idx){
    sl->list[idx]=sl->list[--sl->num_used];
}

void smartlist_del_keeporder(smartlist_t *sl, int idx){
    --sl->num_used;
    if(idx<sl->num_used)
        memmove(sl->list+idx,sl->list+idx+1,sizeof(void*)*(sl->num_used-idx));
}

void smartlist_insert(smartlist_t *sl, int idx, void *val){
    if(idx==sl->num_used){
        smartlist_add(sl,val);
    }else{
        smartlist_ensure_capacity(sl,sl->num_used+1);
        if(idx<sl->num_used)
            memmove(sl->list+idx+1,sl->list+idx,sizeof(void*)*(sl->num_used-idx));
        sl->num_used++;
        sl->list[idx]=val;
    }    
}

void smartlist_sort(smartlist_t *sl,
                    int (*compare)(const void **a, const void **b)){
    if(!sl->num_used) return;
    qsort(sl->list,sl->num_used,sizeof(void*),(int (*)(const void *,const void*))compare);
}

void *smartlist_get_most_frequent_(const smartlist_t *sl,
                    int (*compare)(const void **a, const void **b),
                    int *count_out){
    const void * most_frequent=NULL;
    int most_frequent_count=0;
    
    const void *cur=NULL;
    int i,count=0;
    
    if(!sl->num_used){
        if(count_out)
            *count_out=0;
        return NULL;
    }
    
    for(i=0;i<sl->num_used;++i){
        const void *item=sl->list[i];
        if(cur && 0==compare(&cur,&item)){
            ++count;
        }else{
            if(cur && count>=most_frequent_count){
                most_frequent=cur;
                most_frequent_count=count;
            }
            cur=item;
            count=1;
        }
    }
    
    if(cur && count>most_frequent_count){
        most_frequent=cur;
        most_frequent_count=count;
    }
    if(count_out)
        *count_out=most_frequent_count;
    return (void*)most_frequent;
}

//去除掉sl中相等的,通过compare函数判断是否相等
void smartlist_uniq(smartlist_t *sl,
                    int (*compare)(const void **a, const void **b),
                    void (*free_fn)(void *elt)){
    int i;
    for(i=1;i<sl->num_used;++i){
        if(compare((const void **)&(sl->list[i-1]),
                   (const void **)&(sl->list[i]))==0){
            if(free_fn)
                free_fn(sl->list[i]);
            smartlist_del_keeporder(sl,i--);
        }
    }
}

void *smartlist_bsearch(smartlist_t *sl, const void *key,
                  int (*compare)(const void *key, const void **member)){
  int found, idx;
  idx = smartlist_bsearch_idx(sl, key, compare, &found);
  return found ? smartlist_get(sl, idx) : NULL;
}

int smartlist_bsearch_idx(const smartlist_t *sl, const void *key,
                      int (*compare)(const void *key, const void **member),
                      int *found_out){
  int hi, lo, cmp, mid, len, diff;
  len = smartlist_len(sl);

  if (len == 0) {
    *found_out = 0;
    return 0;
  }

  lo = 0;
  hi = len - 1;

  while (lo <= hi) {
    diff = hi - lo;
   
    mid = lo + (diff / 2);
    cmp = compare(key, (const void**) &(sl->list[mid]));
    if (cmp == 0) {
      *found_out = 1;
      return mid;
    } else if (cmp > 0) {
      lo = mid + 1;
    } else {
      if (mid > 0) {
        hi = mid - 1;
      } else {
        *found_out = 0;
        return 0;
      }
    }
  }

  if (lo < len) {
    cmp = compare(key, (const void **) &(sl->list[lo]));
  } else {
    cmp = compare(key, (const void **) &(sl->list[len-1]));
  }

  *found_out = 0;
  return lo;
}

static int compare_string_ptrs_(const void **_a, const void **_b){
  return strcmp((const char*)*_a, (const char*)*_b);
}

void smartlist_sort_strings(smartlist_t *sl){
  smartlist_sort(sl, compare_string_ptrs_);
}

char *smartlist_get_most_frequent_string(smartlist_t *sl){
  return (char*)smartlist_get_most_frequent(sl, compare_string_ptrs_);
}

char *smartlist_get_most_frequent_string_(smartlist_t *sl, int *count_out){
  return (char*)smartlist_get_most_frequent_(sl, compare_string_ptrs_, count_out);
}

void smartlist_uniq_strings(smartlist_t *sl){
  smartlist_uniq(sl, compare_string_ptrs_, free);
}

static int compare_ptrs_(const void **_a, const void **_b){
  const void *a = *_a, *b = *_b;
  if (a<b)
    return -1;
  else if (a==b)
    return 0;
  else
    return 1;
}

void smartlist_sort_pointers(smartlist_t *sl){
  smartlist_sort(sl, compare_ptrs_);
}

//容器存放构造小根堆
#define LEFT_CHILD(i)  ( 2*(i) + 1 )
#define RIGHT_CHILD(i) ( 2*(i) + 2 )
#define PARENT(i)      ( ((i)-1) / 2 )

#define IDXP(p) ((int*)STRUCT_VAR_P(p, idx_field_offset))

#define UPDATE_IDX(i)  do {                            \
    void *updated = sl->list[i];                       \
    *IDXP(updated) = i;                                \
  } while (0)

#define IDX_OF_ITEM(p) (*IDXP(p))

//调整堆
static INLINE void smartlist_heapify(smartlist_t *sl,
                  int (*compare)(const void *a, const void *b),
                  int idx_field_offset,
                  int idx){
  while (1) {
    int left_idx = LEFT_CHILD(idx);
    int best_idx;

    if (left_idx >= sl->num_used)
      return;
    if (compare(sl->list[idx],sl->list[left_idx]) < 0)
      best_idx = idx;
    else
      best_idx = left_idx;
    if (left_idx+1 < sl->num_used &&
        compare(sl->list[left_idx+1],sl->list[best_idx]) < 0)
      best_idx = left_idx + 1;

    if (best_idx == idx) {
      return;
    } else {
      void *tmp = sl->list[idx];
      sl->list[idx] = sl->list[best_idx];
      sl->list[best_idx] = tmp;
      UPDATE_IDX(idx);
      UPDATE_IDX(best_idx);

      idx = best_idx;
    }
  }
}

//将元素加入到堆中
void smartlist_pqueue_add(smartlist_t *sl,
                     int (*compare)(const void *a, const void *b),
                     int idx_field_offset,
                     void *item){
  int idx;
  smartlist_add(sl,item);
  UPDATE_IDX(sl->num_used-1);

  for (idx = sl->num_used - 1; idx; ) {
    int parent = PARENT(idx);
    if (compare(sl->list[idx], sl->list[parent]) < 0) {
      void *tmp = sl->list[parent];
      sl->list[parent] = sl->list[idx];
      sl->list[idx] = tmp;
      UPDATE_IDX(parent);
      UPDATE_IDX(idx);
      idx = parent;
    } else {
      return;
    }
  }
}

//移除堆顶元素
void *smartlist_pqueue_pop(smartlist_t *sl,
                     int (*compare)(const void *a, const void *b),
                     int idx_field_offset){
  void *top;
  top = sl->list[0];
  *IDXP(top)=-1;
  if (--sl->num_used) {
    sl->list[0] = sl->list[sl->num_used];
    UPDATE_IDX(0);
    smartlist_heapify(sl, compare, idx_field_offset, 0);
  }
  return top;
}

//移除堆中的指定元素
void smartlist_pqueue_remove(smartlist_t *sl,
                        int (*compare)(const void *a, const void *b),
                        int idx_field_offset,
                        void *item){
  int idx = IDX_OF_ITEM(item);
  --sl->num_used;
  *IDXP(item) = -1;
  if (idx == sl->num_used) {
    return;
  } else {
    sl->list[idx] = sl->list[sl->num_used];
    UPDATE_IDX(idx);
    smartlist_heapify(sl, compare, idx_field_offset, idx);
  }
}

//用sep分割字符串str,将分割产生的字符串数组存入到sl中
int smartlist_split_string(smartlist_t *sl, const char *str, const char *sep,
                           int flags, int max){
    const char *cp,*end,*next;
    int n=0;
    cp=str;
    while(1){
        if(flags&SPLIT_SKIP_SPACE){
            while(SMP_ISSPACE(*cp)) ++cp;
        }
        
        if(max>0 && n==max-1){
            end=strchr(cp,'\0');
        }else if(sep){
            end=strstr(cp,sep);
            if(!end)
                end=strchr(cp,'\0');
        }else{
            for(end=cp;*end && *end!='\t' && *end!=' ';++end);
        }
        
        if(!*end){
            next=NULL;
        }else if(sep){
            next=end+strlen(sep);
        }else{
            next=end+1;
            while(*next == '\t' || *next == ' ')
                ++next;
        }
        
        if(flags & SPLIT_SKIP_SPACE){
            while(end > cp && SMP_ISSPACE(*(end-1)))
                --end;
        }
        
        if(end!=cp || !(flags & SPLIT_IGNORE_BLANK)){
            char *string=smp_strndup(cp,end-cp);
            if(flags & SPLIT_SKIP_SPACE)
                smp_strstrip(string," ");
            smartlist_add(sl,string);
            ++n;
        }
        
        if(!next) break;
        cp=next;
    }
                           
    return n;               
}
char *smartlist_join_strings(smartlist_t *sl, const char *join, int terminate,
                             size_t *len_out) {
    return smartlist_join_strings2(sl,join,strlen(join),terminate,len_out);
}

char *smartlist_join_strings2(smartlist_t *sl, const char *join,
                              size_t join_len, int terminate, size_t *len_out) {
    int i;
    size_t n=0;
    char *r=NULL,*dst,*src;
    
    if(terminate)
        n=join_len;
    
    for(i=0;i<sl->num_used;++i){
        n+=strlen((const char*)sl->list[i]);
        if(i+1<sl->num_used)
            n+=join_len;
    }
    
    dst=r=(char*)malloc(n+1);
    for(i=0;i<sl->num_used;){
        for(src=(char*)sl->list[i];*src;)
            *dst++=*src++;
        if(++i<sl->num_used){
            memcpy(dst,join,join_len);
            dst+=join_len;
        }        
    }
    
    if(terminate){
        memcpy(dst,join,join_len);
        dst+=join_len;
    }
    *dst='\0';
    if(len_out)
        *len_out=dst-r;
    return r;                  
}

                              
