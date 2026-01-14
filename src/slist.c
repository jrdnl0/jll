
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <assert.h>
# include "./include/slist.h"


/* allocators and deallocators */

/**
 * @brief Allocate memory for a singly-linked list
 * 
 * @param func The comparison function which the list will use for sorted-based insertions
 * @param circlfag Boolean flag which determines if the list will be circular or not
 * @param sortflag Boolean flag which determines if the list will be sorted or not
 * @param perflag  Boolean flag which determines if the list will be persistent or not
 * 
 * @returns Pointer to the newly created singly-linked list
 */
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

/**
 * @brief Deallocate memory for a singly-linked list.
 * 
 * @param slist Pointer to the singly-linked list to be deallocated
 * @param data_dealloc_func User-specified function which should handle deallocated the jll_data_t pointers referenced
 * by the nodes of the linked list.
 * 
 * @returns None (is void)
 * 
 */
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

/**
 * @brief Appends a new node as the head of a singly-linked list
 * 
 * @param slist Pointer to the singly-linked list
 * @param dptr  Data which is to be referenced by the newly appended head
 *
 * @returns None (is void)
 * 
 */
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


/**
 * @brief Appends a new tail to a singly-linked list
 * 
 * @param slist Pointer to the singly-linked list to be appended to
 * @param dptr  Data to be referenced by the newly appended tail node
 * 
 * @returns None (is void)
 */
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

/**
 * @brief Inserts a node due to specified sorting logic into a singly-linked list
 * 
 * @param slist Pointer to the singly linked list
 * @param dptr  Data to be referenced by the newly inserted node
 * 
 * @returns None (is void)
 * 
 */
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


const jll_data_t * jll_slist_remove_index(jll_slist_t * slist, size_t index)
{
    assert(slist);
    if ((index < 0) || (index >= slist->length))
        return NULL;
    else if (index == 0)
        return jll_slist_remove_head(slist);
    else if (index == slist->length - 1)
        return jll_slist_remove_tail(slist);
    else
    {
        jll_snode_t * backptr = NULL;
        jll_snode_t * frontptr = slist->head;
        size_t k;

        for (k = 0; k < index; k++) 
        {
            backptr = frontptr;
            frontptr = frontptr->next;
        }

        backptr->next = frontptr->next;

        const jll_data_t * retdata = frontptr->data;
        
        free(frontptr);
        slist->length--;
        return retdata;
    }    
}


/**
 * @brief Removes head node from a singly-linked list
 * @param slist List to be edited
 * @returns Constant reference to the data once held by the removed head
 * 
 */
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


/**
 * @brief Removes tail node from a singly-linked list
 * @param slist List to be edited
 * @returns Constant reference to the data once held by the removed tail
 */
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


/**
 * @brief Removes first node in a singly-linked list which matches a user-specified condition
 * @param slist List to to be edited
 * @param compfunc Boolean function which returns true if the data in the argument meets some user-specified criteria.
 * @returns A constant reference to the first data in the list to return true from the parameter compfunc.
 */
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


const jll_data_t * jll_slist_remove_cond_nth(jll_slist_t * slist, bool (*compfunc)(const jll_data_t *), size_t n)
{
    assert(slist);
    assert(compfunc);

    if (n >= slist->length) return NULL;

    size_t counter = 0;
    jll_snode_t * rover = slist->head;

    while (rover)
    {
        if (compfunc(rover->data)) counter++;
        if (counter == n) return rover->data;
        else rover = rover->next;
    }

    return NULL;
}


jll_data_payload_t * jll_slist_remove_cond_first_n(jll_slist_t * slist, bool (*compfunc)(const jll_data_t *), size_t n)
{
    assert(slist);
    assert(compfunc);

    if (n >= slist->length) return NULL;

    size_t index = 0;
    size_t found = 0;
    jll_data_t ** vector;


    jll_snode_t * rover = slist->head;

    
    while (rover && found < n)
    {
        if (compfunc(rover->data))
        {
            found++;
            vector = (jll_data_t **)realloc(vector, found * sizeof(jll_data_t *));
            vector[found - 1] = jll_slist_remove_index(slist, index);
        }

        index++;
        rover = rover->next;
    }


    if (found == 0) return NULL;

    jll_data_payload_t * new_payload = jll_allocate_data_payload((const jll_data_t **)vector, found);
    return new_payload;
}

jll_data_payload_t * jll_slist_remove_cond_all(jll_slist_t * slist, bool (*compfunc)(const jll_data_t *))
{
    assert(slist);
    assert(compfunc);

    size_t index = 0;
    size_t found = 0;
    jll_data_t ** vector;


    jll_snode_t * rover = slist->head;

    while (rover)
    {
        if (compfunc(rover->data))
        {
            found++;
            vector = (jll_data_t **)realloc(vector, found * sizeof(jll_data_t *));
            vector[found - 1] = jll_slist_remove_index(slist, index);
        }
        index++;
        rover = rover->next;
    }

    if (found == 0) return NULL;

    jll_data_payload_t * new_payload = jll_allocate_data_payload((const jll_data_t **)vector, found);
    return new_payload;
}


jll_data_payload_t * jll_slist_remove_all(jll_slist_t * slist)
{
    assert(slist);
    if (jll_slist_is_empty(slist)) return NULL;

    size_t index  = 0;
    size_t veclen = slist->length;
    jll_data_t ** vector = (jll_data_t **)malloc(veclen * sizeof(jll_data_t *));

    while (!jll_slist_is_empty(slist))
    {
        vector[index] = jll_slist_remove_head(slist);
        index++;
    }

    jll_data_payload_t * new_payload = jll_allocate_data_payload((const jll_data_t **)vector, veclen);
    return new_payload;
}

/* access functions */

const jll_data_t * jll_slist_index_pos(jll_slist_t * slist, size_t index)
{
    assert(slist);
    if ((index < 0) || (index >= slist->length)) return NULL;
    else if (index == 0) return slist->head->data;
    else if (index == slist->length - 1) return slist->tail->data;

    jll_snode_t * rover = slist->head;
    for (size_t k = 0; k < index; k++) rover = rover->next;
    return rover->data;
}

const jll_data_t * jll_slist_index_head(jll_slist_t * slist)
{
    assert(slist);
    if (jll_slist_is_empty(slist)) return NULL;
    else return slist->head->data;
}

const jll_data_t * jll_slist_index_tail(jll_slist_t * slist)
{
    assert(slist);
    if (jll_slist_is_empty(slist)) return NULL;
    else return slist->tail->data;
}

const jll_data_t * jll_slist_find_first_occurrence(jll_slist_t * slist, bool (*compfunc)(const jll_data_t *))
{
    assert(slist);
    assert(compfunc);

    jll_snode_t * rover = slist->head;

    while (rover)
    {
        if (compfunc(rover->data)) return rover->data;
        else rover = rover->next;
    }

    return NULL;
}

const jll_data_t * jll_slist_find_nth_occurrence(jll_slist_t * slist, bool (*compfunc)(const jll_data_t *), size_t n)
{
    assert(slist);
    assert(compfunc);
    if (n >= slist->length) return NULL;

    size_t found = 0;
    jll_snode_t * rover = slist->head;

    while (rover)
    {
        if (compfunc(rover->data)) found++;
        if (found == n) return rover->data;
        else rover = rover->next;
    }

    return NULL;
}


bool jll_slist_check_if_sorted(jll_slist_t * slist)
{
    assert(slist);
    if (!slist->slist_comp_func) return false;
    else if (jll_slist_is_empty(slist)) return false;

    jll_snode_t * rover = slist->head;
    
    while (rover->next)
    {
        if (slist->slist_comp_func(rover->data, rover->next->data) == -1) return false;
        else rover = rover->next;
    }

    return true;
}


bool jll_slist_check_if_contains(jll_slist_t * slist, bool (*compfunc)(const jll_data_t *))
{
    assert(slist);
    assert(compfunc);

    jll_snode_t * rover = slist->head;
    while (rover) if (compfunc(rover->data)) return true;
    else rover = rover->next;

    return false;
}


