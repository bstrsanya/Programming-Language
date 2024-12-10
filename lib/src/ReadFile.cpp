#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ReadFile.h"
#include "SizeFile.h"

char* ReadFile (FILE* file_input, size_t* file_size)
{
    assert (file_input != NULL);
    assert (file_size != 0);

    *file_size = (size_t) SizeFile (file_input);

    char* all_text = (char*) calloc (*file_size + 1, sizeof (char));
    assert (all_text != NULL);

    size_t n_fread = fread (all_text, sizeof (char), *file_size, file_input);

    if (n_fread != *file_size) printf ("ERROR READ FILE");
    
    return all_text;
}