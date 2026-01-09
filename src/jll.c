
# include "./include/node.h"
# include "./include/llist.h"
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
    const jll_data_t * retdata = nptr->data_ptr;
    free(nptr);
    return retdata;    
}


jll_list_t * jll_allocate_list(int (*data_comp_func)(const jll_data_t *, const jll_data_t *))
{
    jll_list_t * new_list_ptr = (jll_list_t *)malloc(sizeof(jll_list_t));
    new_list_ptr->data_comp_func = data_comp_func;
    new_list_ptr->head_node_ptr = NULL;
    new_list_ptr->tail_node_ptr = NULL;
    new_list_ptr->length = 0;
    return new_list_ptr;
}


void jll_append_head(jll_list_t * lptr, const jll_data_t * dptr)
{
    assert(lptr);
    assert(dptr);

    jll_node_t * new_head_ptr = jll_allocate_node(dptr);

    if (!lptr->head_node_ptr)
    {
        lptr->head_node_ptr = new_head_ptr;
        lptr->tail_node_ptr = new_head_ptr;
    }
    else
    {
        new_head_ptr->next_ptr = lptr->head_node_ptr;
        lptr->head_node_ptr->prev_ptr = new_head_ptr;
        lptr->head_node_ptr = new_head_ptr;
    }

    lptr->length++;
}


void jll_append_tail(jll_list_t * lptr, const jll_data_t * dptr)
{
    assert(lptr);
    assert(dptr);

    jll_node_t* new_tail_ptr = jll_allocate_node(dptr);

    if (!lptr->tail_node_ptr)
    {
        lptr->head_node_ptr = new_tail_ptr;
        lptr->tail_node_ptr = new_tail_ptr; 
    }
    else
    {
        new_tail_ptr->prev_ptr = lptr->tail_node_ptr;
        lptr->tail_node_ptr->next_ptr = new_tail_ptr;
        lptr->tail_node_ptr = new_tail_ptr;
    }
    
    lptr->length++;
}


void jll_insert_before_pos(jll_list_t * lptr, const jll_data_t * dptr, size_t index)
{
    assert(lptr);
    assert(dptr);
    assert((index >= 0) && (index < lptr->length));

    if (index == 0)
        return jll_append_head(lptr, dptr);

    jll_node_t * new_node_ptr = jll_allocate_node(dptr);
    
    jll_node_t * rover_node_ptr;
    size_t k;

    if (lptr->length < 2 * index + 1)
    {
        rover_node_ptr = lptr->tail_node_ptr;
        for (k = lptr->length - 1; k > index; k--)
            rover_node_ptr = rover_node_ptr->prev_ptr;
    }
    else
    {
        rover_node_ptr = lptr->head_node_ptr;
        for (k = 0; k < index; k++)
            rover_node_ptr = rover_node_ptr->next_ptr;
    }

    // Insert BEFORE rover_node_ptr

    new_node_ptr->prev_ptr = rover_node_ptr->prev_ptr;
    new_node_ptr->prev_ptr->next_ptr = new_node_ptr;

    new_node_ptr->next_ptr = rover_node_ptr;
    rover_node_ptr->prev_ptr = new_node_ptr;

    lptr->length++;    
}


void jll_insert_after_pos(jll_list_t * lptr, const jll_data_t * dptr, size_t index)
{
    assert(lptr);
    assert(dptr);
    assert((index >= 0) && (index < lptr->length));

    if (index >= lptr->length)
        return jll_append_tail(lptr, dptr);
    
    jll_node_t * new_node_ptr = jll_allocate_node(dptr);

    jll_node_t * rover_node_ptr;
    size_t k;

    if (lptr->length < 2 * index + 1)
    {
        rover_node_ptr = lptr->tail_node_ptr;
        for (k = lptr->length - 1; k > index; k--)
            rover_node_ptr = rover_node_ptr->prev_ptr;
    }
    else
    {
        rover_node_ptr = lptr->head_node_ptr;
        for (k = 0; k < index; k++)
            rover_node_ptr = rover_node_ptr->next_ptr;
    }

    // Insert AFTER rover_node_ptr

    new_node_ptr->next_ptr = rover_node_ptr->next_ptr;
    new_node_ptr->next_ptr->prev_ptr = new_node_ptr;

    new_node_ptr->prev_ptr = rover_node_ptr;
    rover_node_ptr->next_ptr = new_node_ptr;

    lptr->length++;
}


long jll_search_list(jll_list_t * lptr, const jll_data_t * dptr)
{
    
    assert(lptr);
    assert(dptr);

    jll_node_t * rover_node_ptr = lptr->head_node_ptr;
    long pos = 0;

    while (rover_node_ptr)
    {
        if (lptr->data_comp_func(rover_node_ptr->data_ptr, dptr) == 0)
            return pos;
        
        pos++;
        rover_node_ptr = rover_node_ptr->next_ptr;
    }

    return -1;
}


const jll_data_t * jll_remove_head(jll_list_t * lptr)
{
    assert(lptr);

    if (!lptr->head_node_ptr)
        return NULL;

    const jll_data_t * retdata = lptr->head_node_ptr->data_ptr;
    
    if (lptr->head_node_ptr->next_ptr)
    {
        lptr->head_node_ptr = lptr->head_node_ptr->next_ptr;
        free(lptr->head_node_ptr->prev_ptr);
        lptr->head_node_ptr->prev_ptr = NULL;
    }
    else
    {
        free(lptr->head_node_ptr);
        lptr->head_node_ptr = NULL;
        lptr->tail_node_ptr = NULL;
    }

    lptr->length--;
    return retdata;
}


const jll_data_t * jll_remove_tail(jll_list_t * lptr)
{
    assert(lptr);

    if (!lptr->tail_node_ptr)
        return NULL;

    const jll_data_t * retdata = lptr->tail_node_ptr->data_ptr;

    if (lptr->tail_node_ptr->prev_ptr)
    {
        lptr->tail_node_ptr = lptr->tail_node_ptr->prev_ptr;
        free(lptr->tail_node_ptr->next_ptr);
        lptr->tail_node_ptr->next_ptr = NULL;
    }
    else
    {
        free(lptr->tail_node_ptr);
        lptr->head_node_ptr = NULL;
        lptr->tail_node_ptr = NULL;
    }

    lptr->length--;
    return retdata;
}

const jll_data_t * jll_remove_pos(jll_list_t * lptr, size_t index)
{
    assert(lptr);

    if ((index < 0) || (index >= lptr->length))
        return NULL;
    else if (index == 0)
        return jll_remove_head(lptr);
    else if (index == lptr->length - 1)
        return jll_remove_tail(lptr);

    jll_node_t * rover_node_ptr;
    size_t k;

    if (lptr->length < 2 * index + 1)
    {
        rover_node_ptr = lptr->tail_node_ptr;
        for (k = lptr->length - 1; k > index; k--)
            rover_node_ptr = rover_node_ptr->prev_ptr;
    }
    else
    {
        rover_node_ptr = lptr->head_node_ptr;
        for (k = 0; k < index; k++)
            rover_node_ptr = rover_node_ptr->next_ptr;
    }

    const jll_data_t * retdata = rover_node_ptr->data_ptr;

    // Remove rover_node_ptr

    rover_node_ptr->next_ptr->prev_ptr = rover_node_ptr->prev_ptr;
    rover_node_ptr->prev_ptr->next_ptr = rover_node_ptr->next_ptr;

    free(rover_node_ptr);
    lptr->length--;

    return retdata;
}