
# ifndef __JLL_NODE_H__
# define __JLL_NODE_H__

# include "datatype.h"
# include <stddef.h>

typedef struct jll_list_node_type
{
    struct jll_list_node_type * next_ptr;
    struct jll_list_node_type * prev_ptr;
    const jll_data_t * data_ptr;

} jll_node_t;

jll_node_t * jll_allocate_node(const jll_data_t *);
const jll_data_t * jll_deallocate_node(jll_node_t *);

const jll_data_t * jll_nodal_access_data(const jll_node_t *);

void jll_nodal_insert_after(jll_node_t *, const jll_data_t *);
void jll_nodal_insert_before(jll_node_t *, const jll_data_t *);
void jll_nodal_insert_between(jll_node_t *, jll_node_t *, const jll_data_t *);

jll_node_t * jll_nodal_recur_traverse(jll_node_t *, size_t);
const jll_data_t * jll_nodal_recur_index(jll_node_t *, size_t);


# endif