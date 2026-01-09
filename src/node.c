
# include "./include/node.h"
# include <stdlib.h>


/**
 * @brief Allocates space for a single jll_node_t
 * 
 * @param dptr const jll_data_t reference to become the data_ptr
 * field in the newly constructed node.
 * 
 * @returns Pointer to the newly constructed node.
 */
jll_node_t * jll_allocate_node(const jll_data_t * dptr)
{
    assert(dptr);

    jll_node_t * new_node_ptr = (jll_node_t *)malloc(sizeof(jll_node_t));
    
    new_node_ptr->data_ptr = dptr;
    new_node_ptr->next_ptr = NULL;
    new_node_ptr->prev_ptr = NULL;
    
    return new_node_ptr;
}

/**
 * @brief Deallocates space for a single jll_node_t
 * 
 * @param nptr Pointer to the jll_node_t which is to be
 * deallocated.
 * 
 * @returns A constant jll_data_t pointer which was 
 * held as the data_ptr field of the recently deallocated node.
 * 
 */
const jll_data_t * jll_deallocate_node(jll_node_t * nptr)
{
    assert(nptr);
    
    const jll_data_t * retdata = nptr->data_ptr;
    free(nptr);

    return retdata;    
}


const jll_data_t * jll_nodal_access_data(const jll_node_t * nptr)
{
    assert(nptr);
    return nptr->data_ptr;
}


void jll_nodal_insert_after(jll_node_t * nptr, const jll_data_t * dptr)
{
    assert(nptr);
    assert(dptr);

    jll_node_t * new_node = jll_allocate_node(dptr);

    new_node->next_ptr = nptr->next_ptr;
    if (new_node->next_ptr)
        new_node->next_ptr->prev_ptr = new_node;

    new_node->prev_ptr = nptr;
    nptr->next_ptr = new_node;
}


void jll_nodal_insert_before(jll_node_t * nptr, const jll_data_t * dptr)
{
    assert(nptr);
    assert(dptr);

    jll_node_t * new_node = jll_allocate_node(dptr);

    new_node->prev_ptr = nptr->prev_ptr;
    if (new_node->prev_ptr)
        new_node->prev_ptr->next_ptr = new_node;
    
    new_node->next_ptr = nptr;
    nptr->prev_ptr = new_node;
}


void jll_nodal_insert_between(jll_node_t * lnode, jll_node_t * rnode, const jll_data_t * dptr)
{
    assert(lnode);
    assert(rnode);
    assert(dptr);

    jll_node_t * midnode = jll_allocate_node(dptr);

    midnode->next_ptr = rnode;
    rnode->prev_ptr = midnode;

    midnode->prev_ptr = lnode;
    lnode->next_ptr = midnode;
}


jll_node_t * jll_nodal_recur_traverse(jll_node_t * nptr, size_t n)
{
    if (!nptr)
        return NULL;
    else if (n == 0)
        return nptr;
    else
        return jll_nodal_recur_traverse(nptr->next_ptr, n - 1);
}


const jll_data_t * jll_nodal_recur_index(jll_node_t * nptr, size_t n)
{
    if (!nptr)
        return NULL;
    else if (n == 0)
        return nptr->data_ptr;
    else
        return jll_nodal_recur_index(nptr->next_ptr, n - 1);
}





