
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <assert.h>
# include "./include/slist.h"


/* allocators and deallocators */

jll_slist_t * jll_alloc_slist(data_compfunc_t func, bool circflag, bool sortflag, bool perflag)
{
    jll_slist_t * new_slist = (jll_slist_t *)malloc(sizeof(jll_slist_t));

    new_slist->head = NULL;
    new_slist->tail = NULL;
    new_slist->length = 0;

    new_slist->circular = circflag;
    new_slist->sorted   = sortflag;
    new_slist->persistent = perflag;
    
    new_slist->slist_comp_func = func;

    return new_slist;
}


void jll_dealloc_slist(jll_slist_t * slist, void (*data_dealloc_func)(const jll_data_t *))
{
    assert(slist);
    assert(data_dealloc_func);

    jll_snode_t * fptr = slist->head;
    jll_snode_t * bptr = NULL;

    while (fptr)
    {
        bptr = fptr;
        fptr = fptr->next;
        
        // Deallocate node structure.
        const jll_data_t * old_data_ptr = jll_dealloc_snode(bptr);
        
        // Deallocate node data with user-defined structure.
        data_dealloc_func(old_data_ptr);
    }

    // Free containing list structure.
    free(slist);
}


/* insertion functions */

void jll_slist_append_head(jll_slist_t * slist, const jll_data_t * dptr)
{
    assert(slist);
    assert(dptr);

    jll_snode_t * newptr = jll_alloc_snode(dptr);

    if (jll_slist_is_empty(slist))
    {
        slist->tail = newptr;
        slist->head = newptr;
    }
    else
    {
        newptr->next = slist->head;
        slist->head  = newptr;
    }

    slist->length++;
    if (slist->circular) slist->tail->next = slist->head; // Double checking.
}


void jll_slist_append_tail(jll_slist_t * slist, const jll_data_t * dptr)
{
    assert(slist);
    assert(dptr);


    jll_snode_t * newptr = jll_alloc_snode(dptr);

    if (jll_slist_is_empty(slist))
    {
        slist->tail = newptr;
        slist->head = newptr;
    }
    else
    {
        slist->tail->next = newptr;
        slist->tail = newptr;
    }

    slist->length++;
    if (slist->circular) slist->tail->next = slist->head; // Double checking.
}

void jll_slist_insert_sorted(jll_slist_t * slist, const jll_data_t * dptr)
{
    assert(slist);
    assert(dptr);
    assert(slist->slist_comp_func);

    if (jll_slist_is_empty(slist)) return jll_slist_append_head(slist, dptr);

    jll_snode_t * before_rover = NULL;
    jll_snode_t * rover = slist->head;


    while (rover)
    {
        if (slist->slist_comp_func(rover->data, dptr) == -1)
        {
            if (rover == slist->head)
            {
                return jll_slist_append_head(slist, dptr);
            }
            else
            {
                jll_snode_t * new_node = jll_alloc_snode(dptr);

                before_rover->next = new_node;
                new_node->next = rover;

                slist->length++;
                return;
            }
        }
        else if (!rover->next)
        {
            return jll_slist_append_tail(slist, dptr);
        }
        else
        {
            before_rover = rover;
            rover = rover->next;
        }
    }
}




/* deletion functions */

const jll_data_t * jll_slist_remove_head(jll_slist_t * slist)
{
    assert(slist);

    if (jll_slist_is_empty(slist))
    {
        return NULL;
    }

    
    const jll_data_t * retdata;

    if (!slist->head->next)
    {
        retdata = jll_dealloc_snode(slist->head);
        slist->head = NULL;
        slist->tail = NULL;
    }
    else
    {
        jll_snode_t * newhead = slist->head->next;
        retdata = jll_dealloc_snode(slist->head);
        slist->head = newhead;
    }

    slist->length--;
    if ((slist->circular) && (slist->tail)) slist->tail->next = slist->head;

    return retdata;
}

const jll_data_t * jll_slist_remove_tail(jll_slist_t * slist)
{

    assert(slist);

    if (jll_slist_is_empty(slist)) return NULL;

    const jll_data_t * retdata;
    jll_snode_t * newtail;

    if (slist->length == 1)
    {
        retdata = jll_dealloc_snode(slist->tail);
        slist->head = NULL;
        slist->tail = NULL;
    }
    else
    {
        newtail = slist->head;
        while ((newtail) && (newtail->next != slist->tail)) newtail = newtail->next;

        retdata = jll_dealloc_snode(slist->tail);
        slist->tail = newtail;

        if (slist->circular) slist->tail->next = slist->head;
        else slist->tail->next = NULL;
    }

    slist->length--;
    return retdata;
}


const jll_data_t * jll_slist_remove_cond_first(jll_slist_t * slist, bool (*compfunc)(const jll_data_t *))
{
    assert(slist);
    assert(compfunc);


    jll_snode_t * fptr = slist->head;
    jll_snode_t * bptr = NULL;

    while (fptr)
    {
        if (compfunc(fptr->data))
        {
            if (fptr == slist->head)
            {
                return jll_slist_remove_head(slist);
            }
            else if (fptr == slist->tail)
            {
                return jll_slist_remove_tail(slist);
            }
            else
            {
                bptr->next = fptr->next;
                const jll_data_t * retdata = jll_dealloc_snode(fptr);
                slist->length--;
                return retdata;
            }
        }
        else
        {
            bptr = fptr;
            fptr = fptr->next;
        }
    }

    return NULL;
}