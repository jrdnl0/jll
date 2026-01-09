
# ifndef __JLL_SLIST_H__
# define __JLL_SLIST_H__


# include "snode.h"


typedef struct jll_singly_list_type
{
    
    jll_snode_t * head;
    jll_snode_t * tail;

    size_t length;
    
    bool circular;
    bool sorted;
    bool persistent;

    data_compfunc_t slist_comp_func;

} jll_slist_t;

/* allocators and deallocators*/
jll_slist_t * jll_alloc_slist(data_compfunc_t, bool, bool, bool);
void jll_dealloc_slist(jll_slist_t *, void (*)(const jll_data_t *));

/*insertion functions*/
void jll_slist_append_head(jll_slist_t *, const jll_data_t *);
void jll_slist_append_tail(jll_slist_t *, const jll_data_t *);
void jll_slist_insert_sorted(jll_slist_t *, const jll_data_t *);
void jll_slist_insert_ranged(jll_slist_t *, const jll_data_t *, size_t, size_t);
void jll_slist_insert_from_payload(jll_slist_t *, const jll_data_payload_t *);
void jll_slist_insert_from_slist(jll_slist_t *, const jll_slist_t *);

/*deletion functions*/
const jll_data_t * jll_slist_remove_head(jll_slist_t *);
const jll_data_t * jll_slist_remove_tail(jll_slist_t *);
const jll_data_t * jll_slist_remove_cond_first(jll_slist_t *, bool (*)(const jll_data_t *));
jll_data_payload_t * jll_slist_remove_cond_first_n(jll_slist_t *, bool (*)(const jll_data_t *), size_t *);
jll_data_payload_t * jll_slist_remove_cond_all(jll_slist_t *, bool (*)(const jll_data_t *));
jll_data_payload_t * jll_slist_remove_all(jll_slist_t *);

/*access functions*/
const jll_data_t * jll_slist_index_pos(jll_slist_t *, size_t);
const jll_data_t * jll_slist_index_head(jll_slist_t *);
const jll_data_t * jll_slist_index_tail(jll_slist_t *);
const jll_data_t * jll_slist_find_first_occurrence(jll_slist_t *, bool (*)(const jll_data_t *));
const jll_data_t * jll_slist_find_nth_occurrence(jll_slist_t *, bool (*)(const jll_data_t *), size_t);
bool jll_slist_check_if_sorted(jll_slist_t *, int (*)(const jll_data_t *, const jll_data_t *));
bool jll_slist_check_if_contains(jll_slist_t *, bool (*)(const jll_data_t *));

/*list manipulation*/
void jll_slist_reversal(jll_slist_t *);
void jll_slist_rotate_n(jll_slist_t *, size_t);
void jll_slist_concat(jll_slist_t *, jll_slist_t *);
jll_slist_t * jll_slist_split_at_nth(jll_slist_t *, size_t);

# endif