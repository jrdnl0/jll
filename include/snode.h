

# ifndef __JLL_SNODE_H__
# define __JLL_SNODE_H__

# include "datatype.h"

/**
 * @brief Singly linked list node type
 */
typedef struct jll_singly_node_type
{
    struct jll_singly_node_type * next;
    const  jll_data_t * data;

} jll_snode_t;

jll_snode_t * jll_alloc_snode(const jll_data_t *);
const jll_data_t * jll_dealloc_snode(jll_snode_t *);
const jll_data_t * jll_access_snode(const jll_snode_t *);


# endif