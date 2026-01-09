

# ifndef __JLL_DNODE_H__
# define __JLL_DNODE_H__

# include "datatype.h"

/**
 * @brief Doubly linked list node type
 */
typedef struct jll_doubly_node_type
{
    struct jll_doubly_node_type * next;
    struct jll_doubly_node_type * prev;
    const  jll_data_t * data;

} jll_dnode_t;

jll_dnode_t * jll_alloc_dnode(const jll_data_t *);
const jll_data_t * jll_dealloc_dnode(jll_dnode_t *);
const jll_data_t * jll_access_dnode(const jll_dnode_t *);



# endif