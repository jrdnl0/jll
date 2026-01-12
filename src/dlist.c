

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <assert.h>
# include "./include/dlist.h"

/* allocators and deallocators */


jll_dlist_t * jll_alloc_dlist(data_compfunc_t func, bool circflag, bool sortflag, bool perflag)
{
    
    jll_dlist_t * new_dlist = (jll_dlist_t *)malloc(sizeof(jll_dlist_t));

    new_dlist->head = NULL;
    new_dlist->tail = NULL;
    new_dlist->length = 0;

    new_dlist->circular = circflag;
    new_dlist->sorted = sortflag;
    new_dlist->persistent = perflag;

    new_dlist->dlist_comp_func = func;

    return new_dlist;
}


void jll_dealloc_dlist(jll_dlist_t * dlist, void (*data_dealloc_func)(const jll_data_t *))
{
    assert(dlist);
    assert(data_dealloc_func);

    jll_dnode_t * fptr = dlist->head;
    jll_dnode_t * bptr = NULL;

    while (fptr)
    {
        bptr = fptr;
        fptr = fptr->next;

        const jll_data_t * old_data_ptr = jll_dealloc_dnode(bptr);
        data_dealloc_func(old_data_ptr);
    }

    free(dlist);
}

/* insertion functions */

void jll_dlist_append_head(jll_dlist_t * dlist, const jll_data_t * dptr)
{
    assert(dlist);
    assert(dptr);

    jll_dnode_t * newptr = jll_alloc_dnode(dptr);
    
    if (jll_dlist_is_empty(dlist))
    {
        dlist->head = newptr;
        dlist->tail = newptr;
    }
    else
    {
        newptr->next = dlist->head;
        dlist->head->prev = newptr;
        dlist->head = newptr;
    }

    dlist->length++;

    if (dlist->circular)
    {
        dlist->head->prev = dlist->tail;
        dlist->tail->next = dlist->head;
    }
}


void jll_dlist_append_tail(jll_dlist_t * dlist, const jll_data_t * dptr)
{
    assert(dlist);
    assert(dptr);

    jll_dnode_t * newptr = jll_alloc_dnode(dptr);

    if (jll_dlist_is_empty(dlist))
    {
        dlist->head = newptr;
        dlist->tail = newptr;
    }
    else
    {
        dlist->tail->next = newptr;
        newptr->prev = dlist->tail;
        dlist->tail = newptr;
    }

    dlist->length++;

    if (dlist->circular)
    {
        dlist->head->prev = dlist->tail;
        dlist->tail->next = dlist->head;
    }
}

void jll_dlist_insert_sorted(jll_dlist_t * dlist, const jll_data_t * dptr)
{
    assert(dlist);
    assert(dptr);
    assert(dlist->dlist_comp_func);

    if (jll_dlist_is_empty(dlist)) return jll_dlist_append_head(dlist, dptr);

    jll_dnode_t * rover = dlist->head;

    while (rover)
    {
        // Comparison function should return -1 if dptr data belongs *before* the rover node.
        if (dlist->dlist_comp_func(rover->data, dptr) == -1)
        {
            // Insert newnode *before* the rover node.
            if (rover == dlist->head)
            {
                return jll_dlist_append_head(dlist, dptr);
            }
            else
            {
                jll_dnode_t * new_node = jll_alloc_dnode(dptr);
                
                new_node->next = rover;
                new_node->prev = rover->prev;

                if (new_node->prev) new_node->prev->next = new_node;
                
                rover->prev = new_node;
                dlist->length++;
                return;
            }
        }
        else if (!rover->next)
        {
            return jll_dlist_append_tail(dlist, dptr);
        }
        else
        {
            rover = rover->next;
        }
    }
}


/* deletion functions */

const jll_data_t * jll_dlist_remove_index(jll_dlist_t * dlist, size_t index)
{
    assert(dlist);
    if ((index < 0) || (index >= dlist->length)) return NULL;

    if (index == 0) return jll_dlist_remove_head(dlist);
    else if (index == dlist->length - 1) return jll_dlist_remove_tail(dlist);

    jll_dnode_t * rover;
    size_t k;

    if (dlist->length < 2 * index + 1)
    {
        rover = dlist->tail;
        for (k = dlist->length - 1; k > index; k--) rover = rover->prev;
    }
    else
    {
        rover = dlist->head;
        for (k = 0; k < index; k++) rover = rover->next;
    }

    const jll_data_t * retdata = rover->data;

    rover->next->prev = rover->prev;
    rover->prev->next = rover->next;

    free(rover);
    dlist->length--;

    return retdata;
}


const jll_data_t * jll_dlist_remove_head(jll_dlist_t * dlist)
{
    assert(dlist);
    if (jll_dlist_is_empty(dlist)) return NULL;


    const jll_data_t * retdata = dlist->head->data;

    if (dlist->head->next)
    {
        dlist->head = dlist->head->next;
        free(dlist->head->prev);

        if (dlist->circular)
        {
            dlist->head->prev = dlist->tail;
            dlist->tail->next = dlist->head;
        }
        else
        {
            dlist->head->prev = NULL;
        }
    }
    else
    {
        free(dlist->head);
        dlist->head = NULL;
        dlist->tail = NULL;
    }

    dlist->length--;
    return retdata;
}


const jll_data_t * jll_dlist_remove_tail(jll_dlist_t * dlist)
{
    assert(dlist);
    if (jll_dlist_is_empty(dlist)) return NULL;


    const jll_data_t * retdata = dlist->tail->data;

    if (dlist->tail->prev)
    {
        dlist->tail = dlist->tail->prev;
        free(dlist->tail->next);
        if (dlist->circular)
        {
            dlist->tail->next = dlist->head;
            dlist->head->prev = dlist->tail;
        }
        else dlist->tail->next = NULL;
    }
    else
    {
        free(dlist->tail);
        dlist->tail = NULL;
        dlist->head = NULL;
    }

    dlist->length--;
    return retdata;
}

const jll_data_t * jll_dlist_remove_cond_first(jll_dlist_t * dlist, bool (*compfunc)(const jll_data_t *))
{
    assert(dlist);
    assert(compfunc);

    jll_dnode_t * rover = dlist->head;
    while (rover)
    {
        if (compfunc(rover->data))
            return rover->data;
        else
            rover = rover->next;
    }

    return NULL;
}

jll_data_payload_t * jll_dlist_remove_cond_first_n(jll_dlist_t * dlist, bool (*compfunc)(const jll_data_t *), size_t n)
{

    assert(dlist);
    assert(compfunc);
    assert((n > 0));

    size_t index = 0;
    size_t found = 0;
    jll_data_t ** vector;

    jll_dnode_t * rover = dlist->head;

    while (rover && (found < n))
    {
        if (compfunc(rover->data))
        {
            found++;
            vector = (jll_data_t **)realloc(vector, found * sizeof(jll_data_t *));
            vector[found - 1] = jll_dlist_remove_index(dlist, index);
        }

        index++;
        rover = rover->next;
    }

    if (found == 0)
        return NULL;

    jll_data_payload_t * new_payload = jll_allocate_data_payload((const jll_data_t **)vector, found);
    return new_payload;
}

jll_data_payload_t * jll_dlist_remove_cond_all(jll_dlist_t * dlist, bool (*compfunc)(const jll_data_t *))
{
    assert(dlist);
    assert(compfunc);

    size_t index = 0;
    size_t found = 0;
    jll_data_t ** vector;


    jll_dnode_t * rover = dlist->head;

    while (rover)
    {
        if (compfunc(rover->data))
        {
            found++;
            vector = (jll_data_t **)realloc(vector, found * sizeof(jll_data_t *));
            vector[found - 1] = jll_dlist_remove_index(dlist, index);
        }

        index++;
        rover = rover->next;
    }

    if (found == 0) return NULL;

    jll_data_payload_t * new_payload = jll_allocate_data_payload((const jll_data_t **)vector, found);
    return new_payload;
}


jll_data_payload_t * jll_dlist_remove_all(jll_dlist_t * dlist)
{
    assert(dlist);
    if (jll_dlist_is_empty(dlist)) return NULL;

    size_t veclen = dlist->length;
    jll_data_t ** vector = (jll_data_t **)malloc(veclen * sizeof(jll_data_t *));

    for (size_t k = 0; k < veclen; k++)
        vector[k] = jll_dlist_remove_index(dlist, k);

    jll_data_payload_t * newpayload = jll_allocate_data_payload((const jll_data_t **)vector, veclen);
    return newpayload;
}





/* access functions */
const jll_data_t * jll_dlist_index_pos(jll_dlist_t * dlist, size_t index)
{
    assert(dlist);

    if ((index < 0) || (index >= dlist->length)) 
        return NULL;
    else if (index == 0) 
        return jll_dlist_index_head(dlist);
    else if (index == dlist->length - 1)
        return jll_dlist_index_tail(dlist);

    jll_dnode_t * rover;
    size_t k;
    if (dlist->length < 2 * index + 1)
    {
        rover = dlist->tail;
        for (k = dlist->length - 1; k > index; k--) rover = rover->prev;
    }
    else
    {
        rover = dlist->head;
        for (k = 0; k < index; k++) rover = rover->next;
    }

    return (rover->data);
}


const jll_data_t * jll_dlist_index_head(jll_dlist_t * dlist)
{
    assert(dlist);
    if (!dlist->head)
        return NULL;
    else
        return dlist->head->data;
}


const jll_data_t * jll_dlist_index_tail(jll_dlist_t * dlist)
{
    assert(dlist);
    if (!dlist->tail)
        return NULL;
    else
        return dlist->tail->data;
}


const jll_data_t * jll_dlist_find_first_occurrence(jll_dlist_t * dlist, bool (*compfunc)(const jll_data_t *))
{
    assert(dlist);
    assert(compfunc);

    jll_dnode_t * rover = dlist->head;

    while (rover)
    {
        if (compfunc(rover->data))
            return rover->data;
        else
            rover = rover->next;
    }
    
    return NULL;
}


const jll_data_t * jll_dlist_find_nth_occurrence(jll_dlist_t * dlist, bool (*compfunc)(const jll_data_t *), size_t n)
{
    assert(dlist);
    assert(compfunc);

    if ((n <= 0) || (n >= dlist->length)) return NULL;

    size_t counter = 0;
    jll_dnode_t * rover = dlist->head;

    while (rover)
    {
        if (compfunc(rover->data)) counter++;
        if (counter == n) return rover->data;
        rover = rover->next;
    }

    return NULL;
}

bool jll_dlist_check_if_sorted(jll_dlist_t * dlist)
{
    assert(dlist);
    if (!dlist->dlist_comp_func)
        return false;
    if (jll_dlist_is_empty(dlist))
        return false;

    jll_dnode_t * rover = dlist->head;

    while (rover->next)
    {
        if (dlist->dlist_comp_func(rover->data, rover->next->data) == -1)
            return false;
        else
            rover = rover->next;
    }

    return true;
}

bool jll_dlist_check_if_contains(jll_dlist_t * dlist, bool (*compfunc)(const jll_data_t *))
{
    assert(dlist);
    assert(compfunc);

    jll_dnode_t * rover = dlist->head;
    while (rover)
    {  
        if (compfunc(rover->data))
            return true;
        else
            rover = rover->next;
    }
    return false;
}

bool jll_dlist_is_empty(jll_dlist_t * dlist)
{
    if (!dlist->head)
        return true;
    else if (!dlist->tail)
        return true;
    else if (dlist->length == 0)
        return true;
    else
        return false;
}

bool jll_dlist_is_circular(jll_dlist_t * dlist)
{
    assert(dlist);

    if (jll_dlist_is_empty(dlist))
        return false;
    else if (dlist->tail->next != dlist->head)
        return false;
    else if (dlist->head->prev != dlist->tail)
        return false;
    else
        return true;
}

bool jll_dlist_is_circular(jll_dlist_t * dlist)
{
    assert(dlist);
    
    if (jll_dlist_is_empty(dlist))
        return false;
    else if (dlist->tail->next != dlist->head)
        return false;
    else if (dlist->head->prev != dlist->tail)
        return false;
    else
        return true;
}


/* list manipulation */

static void __jll_dlist_swap_node_data(jll_dnode_t * nodeone, jll_dnode_t * nodetwo)
{
    const jll_data_t * temp = nodeone->data;
    nodeone->data = nodetwo->data;
    nodetwo->data = temp;
}

void jll_dlist_reversal(jll_dlist_t * dlist)
{
    assert(dlist);
    if (jll_dlist_is_empty(dlist)) return;
    else if (dlist->length == 1) return;
    
    jll_dnode_t * front_rover = dlist->head;
    jll_dnode_t * back_rover  = dlist->tail;

    __jll_dlist_swap_node_data(front_rover, back_rover);

    front_rover = front_rover->next;
    back_rover  = back_rover->prev;

    while (front_rover->prev != back_rover)
    {
        __jll_dlist_swap_node_data(front_rover, back_rover);
        front_rover = front_rover->next;
        back_rover = back_rover->prev;
    }
}


void jll_dlist_concat(jll_dlist_t * lone, jll_dlist_t * ltwo)
{
    assert(lone);
    assert(ltwo);

    // lone + ltwo

    lone->tail->next = ltwo->head;
    ltwo->head->prev = lone->tail;
    lone->tail = ltwo->tail;
    
    if (lone->circular || ltwo->circular)
    {
        lone->tail->prev = lone->head;
        lone->head->next = lone->tail;        
    }

    lone->length += ltwo->length;
    
    free(ltwo); // Full list is now stored in lone.
    ltwo = NULL;
}