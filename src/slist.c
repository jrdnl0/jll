
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


