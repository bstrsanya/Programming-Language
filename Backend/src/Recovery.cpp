#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "Recovery.h"

//TODO: Move to COmmon
void TreeCtor (Tree_t* tree, const char* name_file_input, const char* name_file_output)
{
    assert (tree);
    assert (name_file_input);
    assert (name_file_output);

    printf ("I'm backend! Open file [%s] for read and [%s] for writing\n", name_file_input, name_file_output);

    tree->input = fopen (name_file_input, "rb");

    char** table = (char**) calloc (100, sizeof (char*));
    assert (table);
    for (int i = 0; i < 10; i++)
        table[i] = NULL;

    tree->table_var = table;

    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen (name_file_output, "wb");
}

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right)
{
    Node_t* new_node = (Node_t*) calloc (1, sizeof (Node_t));
    assert (new_node);

    new_node->type = (TypeCommand_t) type;

    if (type == NUM)
        new_node->value.number = value;
    else if (type == VAR)
        new_node->value.var = (int) value;
    else
        new_node->value.com = (ListCommand_t) value;

    new_node->left = left;
    new_node->right = right;

    return new_node;
}

void NodeDtor (Node_t* node)
{
    if (!node) return;
    
    if (node->left) NodeDtor (node->left);
    if (node->right) NodeDtor (node->right);

}

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    for (int i = 0; i < SIZE_ARRAY; i++)
    {
        if (tree->array[i])
            free (tree->array[i]);
    }

    free (tree->array);

    // free (tree->table_var);
    free (tree->read_data);
    free (tree->table_var);

    if (tree->output)
        fclose (tree->output);
}

void ReadDataBase (Tree_t* tree)
{
    size_t size = 0;
    tree->read_data = ReadFile (tree->input, &size);

    // creating tokens from buffer data
    tree->array = CreateArrayTokens ();
    Tokenization (tree);

    int pointer = 0;
    tree->expression = GetP (&pointer, tree->array);
    // for (int i = 0; i < 20; i++)
    // {
    //     if (tree->array[i]->type == NUM)
    //         printf ("[%d]: type [%d], value [%g]\n", i, tree->array[i]->type, tree->array[i]->value.number);
    //     else if (tree->array[i]->type == VAR)
    //         printf ("[%d]: type [%d], value [%d]\n", i, tree->array[i]->type, tree->array[i]->value.var);
    //     else
    //         printf ("[%d]: type [%d], value [%d]\n", i, tree->array[i]->type, tree->array[i]->value.com);
    // }
}

//TODO: MOre asserts
Node_t** CreateArrayTokens ()
{
    Node_t** array = (Node_t**) calloc (SIZE_ARRAY, sizeof (Node_t*));
    assert (array);

    for (int i = 0; i < SIZE_ARRAY; i++)
    {
        array[i] = NodeCtor (0, 0, NULL, NULL);
        array[i]->left = NULL;
        array[i]->right = NULL;
        array[i]->type = INVALID_TYPE;
        array[i]->value.com = F_INVALID;
        array[i]->value.number = NAN;
        array[i]->value.var = -1;
    }

    return array;
}

void Tokenization (Tree_t* tree)
{
    Node_t** array = tree->array;
    int y = 0;

    char* buffer = tree->read_data;
    int position = 0;    

    while (buffer[position] != '\n')
    {
        while (buffer[position] == ' ' || buffer[position] == '\n')
            position++;

        if (('0' <= buffer[position]) && (buffer[position] <= '9'))
        {            
            double d = 0;
            int n = 0;
            sscanf (buffer + position, "%lf%n", &d, &n);
            position += n;
            array[y]->type = NUM;
            array[y]->value.number = d;
            y++;
        }
        else if (buffer[position] == '(')
        {
            array[y]->type = OP;
            array[y]->value.com = F_BRACE_OPEN;
            y++;
            position++;
        }
        else if (buffer[position] == ')')
        {
            array[y]->type = OP;
            array[y]->value.com = F_BRACE_CLOSE;
            y++;
            position++;
        }
        else if (buffer[position] == '_')
        {
            array[y]->type = OP;
            array[y]->value.com = F_UNDERLINING;
            y++;
            position++;
        }  
    }
    array[y]->value.var = '$';
    y++;

    while (buffer[position] != 0)
    {
        while (buffer[position] == ' ' || buffer[position] == '\n')
            position++;
        
        int n = 0;
        sscanf (buffer + position, "%*[a-zA-Z]%n", &n);
        buffer[position + n] = '\0';

        int i = 0;
        while (tree->table_var[i])
            i++;
        
        tree->table_var[i] = buffer + position;

        position += n + 1;
    }
}
//TODO: NAMING NOT RESUCRSIVE DESCENT
Node_t* GetP (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com == F_BRACE_OPEN)
    {
        (*pointer)++;
        Node_t* node = array[*pointer];
        node->type = (TypeCommand_t) array[*pointer]->value.number;
        (*pointer)++;
        if (node->type == NUM)
            node->value.number = array[*pointer]->value.number;
        else if (node->type == VAR)
            node->value.var = (int) array[*pointer]->value.number;
        else if (node->type == OP)
            node->value.com = (ListCommand_t) array[*pointer]->value.number;
        else if (node->type == FUNC)
            node->value.var = (int) array[*pointer]->value.number;
        else if (node->type == BLOCK)
            node->value.com = (ListCommand_t) array[*pointer]->value.number;
        (*pointer)++;

        node->left = GetP (pointer, array);
        node->right = GetP (pointer, array);

        if (array[*pointer]->value.com == F_BRACE_CLOSE)
        {
            (*pointer)++;
        }

        return node;
    }
    else
    {
        (*pointer)++;
        return NULL;
    }
}

void CreateAsmFile (Node_t* node, Tree_t* tree)
{
    //TODO: if -> switch
    if (node->type == FUNC)
    {   
        if (!node->left && !node->right)
        {
            fprintf (tree->output, "CALL %s:\n", tree->table_var[node->value.var]);
        }
        else
        {
            fprintf (tree->output, "%s:\n", tree->table_var[node->value.var]);
            if (node->left) CreateAsmFile (node->left, tree);
            if (node->right) CreateAsmFile (node->right, tree);
            fprintf (tree->output, "RET\n");
        }
    }
    if (node->type == NUM)
    {
        fprintf (tree->output, "PUSH %g\n", node->value.number);
    }
    if (node->type == VAR)
    {
        fprintf (tree->output, "PUSH [%d]\n", node->value.var);
    }
    if (node->value.com == F_INTERRUPT)
    {
        if (node->left) CreateAsmFile (node->left, tree);
        if (node->right) CreateAsmFile (node->right, tree);
    }
    if (node->value.com == F_INT || node->value.com == F_DOUBLE)
    {
        CreateAsmFile (node->right, tree);
    }
    if (node->value.com == F_ASSIGNMENT)
    {
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "POP [%d]\n", node->left->value.var);
    }
    if (node->value.com == F_ADD)
    {
        CreateAsmFile (node->left, tree);
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "ADD\n");
    }
    if (node->value.com == F_SUB)
    {
        CreateAsmFile (node->left, tree);
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "SUB\n");
    }
    if (node->value.com == F_MUL)
    {
        CreateAsmFile (node->left, tree);
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "MUL\n");
    }
    if (node->value.com == F_DIV)
    {
        CreateAsmFile (node->left, tree);
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "DIV\n");
    }
    
    if (node->value.com == F_WHILE)
    {
        fprintf (tree->output, "WHILE%p:\n", node);
        CreateAsmFile (node->left->left, tree);
        CreateAsmFile (node->left->right, tree);
        ChangeSign (node, tree);
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "JMP WHILE%p:\nLABEL%p:\n", node, node);                
    }
    
    if (node->value.com == F_IF)
    {
        CreateAsmFile (node->left->left, tree);
        CreateAsmFile (node->left->right, tree);
        ChangeSign (node, tree);
        if (node->right->value.com != F_ELSE)
        {
            CreateAsmFile (node->right, tree);
            fprintf (tree->output, "LABEL%p:\n", node);
        }
        else 
        {
            CreateAsmFile (node->right->left, tree);
            fprintf (tree->output, "JMP LABEL%p:\n", node->right);
            fprintf (tree->output, "LABEL%p:\n", node);
            CreateAsmFile (node->right->right, tree);
            fprintf (tree->output, "LABEL%p:\n", node->right);
        }
    }
    if (node->value.com == F_PRINT)
    {
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "OUT\n");
    }
    if (node->value.com == F_INPUT)
    {
        fprintf (tree->output, "IN\n");
        fprintf (tree->output, "POP [%d]\n", node->right->value.var);
    }
    if (node->value.com == F_SQRT)
    {
        CreateAsmFile (node->right, tree);
        fprintf (tree->output, "SQRT\n");
    }
}

void ChangeSign (Node_t* node, Tree_t* tree)
{
    if (node->left->value.com == F_JBE)
    {
        fprintf (tree->output, "JA LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JE)
    {
        fprintf (tree->output, "JNE LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JAE)
    {
        fprintf (tree->output, "JB LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JB)
    {
        fprintf (tree->output, "JAE LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JA)
    {
        fprintf (tree->output, "JBE LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JNE)
    {
        fprintf (tree->output, "JE LABEL%p:\n", node);
    }
    //TODO: elseif -> switch
}




