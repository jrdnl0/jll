
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

# endif