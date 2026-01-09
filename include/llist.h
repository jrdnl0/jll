
# ifndef __JLL_LLIST_H__
# define __JLL_LLIST_H__

# include "datatype.h"
# include "node.h"

typedef struct jll_list_type
{
    jll_node_t * head_node_ptr;
    jll_node_t * tail_node_ptr;
    size_t length;
    
    /**
     * COMPARES (A, B)
     * if A = B ->  0
     * if A > B ->  1
     * if A < B -> -1 
     */
    int (*data_comp_func)(const jll_data_t *, const jll_data_t *);

} jll_list_t;

jll_list_t * jll_allocate_list(int (*)(const jll_data_t *, const jll_data_t *));
void jll_dealloc_list(jll_list_t *, void (*)(jll_data_t *));

void jll_append_head(jll_list_t *, const jll_data_t *);
void jll_append_tail(jll_list_t *, const jll_data_t *);
void jll_insert_before_pos(jll_list_t *, const jll_data_t *, size_t);
void jll_insert_after_pos(jll_list_t *, const jll_data_t *, size_t);

long jll_search_list(jll_list_t *, const jll_data_t *);

const jll_data_t * jll_remove_head(jll_list_t *);
const jll_data_t * jll_remove_tail(jll_list_t *);
const jll_data_t * jll_remove_pos(jll_list_t *, size_t);



# endif