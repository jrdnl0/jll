
# ifndef __JLL_DATA_H__
# define __JLL_DATA_H__

# include <stddef.h>
# include <stdbool.h>

typedef struct jll_data_type
{


} jll_data_t;

typedef int (*data_compfunc_t)(const jll_data_t *, const jll_data_t *);

typedef struct jll_data_payload_type
{
    const jll_data_t ** data;
    size_t length;

} jll_data_payload_t;



# endif