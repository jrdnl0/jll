

# ifndef __JLL_DLIST_H__
# define __JLL_DLIST_H__

# include "dnode.h"


typedef struct jll_doubly_list_type
{
    jll_dnode_t * head;
    jll_dnode_t * tail;

    size_t length;

    bool circular;
    bool sorted;
    bool persistent;

    data_compfunc_t dlist_comp_func;

} jll_dlist_t;


/* allocators and deallocators */
jll_dlist_t * jll_alloc_dlist(data_compfunc_t, bool, bool, bool);
void jll_dealloc_dlist(jll_dlist_t *, void (*)(const jll_data_t *));

/* insertion functions */
void jll_dlist_append_head(jll_dlist_t *, const jll_data_t *);
void jll_dlist_append_tail(jll_dlist_t *, const jll_data_t *);
void jll_dlist_insert_sorted(jll_dlist_t *, const jll_data_t *);
void jll_dlist_insert_ranged(jll_dlist_t *, const jll_data_t *, size_t, size_t);
void jll_dlist_insert_from_payload(jll_dlist_t *, const jll_data_payload_t *);
void jll_dlist_insert_from_dlist(jll_dlist_t *, const jll_dlist_t *);

/* deletion functions */
const jll_data_t * jll_dlist_remove_head(jll_dlist_t *);
const jll_data_t * jll_dlist_remove_tail(jll_dlist_t *);
const jll_data_t * jll_dlist_remove_cond_first(jll_dlist_t *, bool (*)(const jll_data_t *));
jll_data_payload_t * jll_dlist_remove_cond_first_n(jll_dlist_t *, bool (*)(const jll_data_t *), size_t *);
jll_data_payload_t * jll_dlist_remove_cond_all(jll_dlist_t *, bool (*)(const jll_data_t *));
jll_data_payload_t * jll_dlist_remove_all(jll_dlist_t *);


/* access functions */
const jll_data_t * jll_dlist_index_pos(jll_dlist_t *, size_t);
const jll_data_t * jll_dlist_index_head(jll_dlist_t *);
const jll_data_t * jll_dlist_index_tail(jll_dlist_t *);
const jll_data_t * jll_dlist_find_first_occurrence(jll_dlist_t *, bool (*)(const jll_data_t *));
const jll_data_t * jll_dlist_nth_occurrence(jll_dlist_t *, bool (*)(const jll_data_t *), size_t);
bool jll_dlist_check_if_sorted(jll_dlist_t *, int (*)(const jll_data_t *, const jll_data_t *));
bool jll_dlist_check_if_contains(jll_dlist_t *, bool (*)(const jll_data_t *));

/* list manipulation */
void jll_dlist_reversal(jll_dlist_t *);
void jll_dlist_rotate_n(jll_dlist_t *, size_t);
void jll_dlist_concat(jll_dlist_t *, jll_dlist_t *);
jll_dlist_t * jll_dlist_split_at_nth(jll_dlist_t *, size_t);


# endif