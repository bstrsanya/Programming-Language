#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "General.h"

// void CreateDot (Node_t* node, FILE* file_dot)
// {
//     if (!node) return;

//     if (node->type == NUM)
//         fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#a2ff00\"; color = \"#800000\"; label = \"{type = NUM | value = %g | address = %p | { <f0> left = %p | <f1> right = %p}}\"];\n", node, node->value, node, node->left, node->right);

//     else if (node->type == OP)
//         fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#ffe4c4\"; color = \"#800000\"; label = \"{type = OP | value = %c | address = %p | { <f0> left = %p | <f1> right = %p}}\"];\n", node, (char) node->value, node, node->left, node->right);

//     if (node->left) 
//     {
//         fprintf (file_dot, "node%p: <f0> -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->left);
//         CreateDot (node->left, file_dot);
//     }
//     if (node->right) 
//     {
//         fprintf (file_dot, "node%p: <f1> -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->right);
//         CreateDot (node->right, file_dot);
//     }
// }

void CreateDotUSER (Node_t* node, FILE* file_dot, Tree_t* tree)
{
    if (!node) return;

    if (node->type == NUM)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#a2ff00\"; label = \"%.3g\"];\n", node, node->value.number);

    else if (node->type == OP)
    {
        for (int i = 0; i < NUM_COMMAND; i++)
            if (node->value.com == array_command[i].n_com)
                fprintf (file_dot, "node%p [shape=circle; style = filled; fillcolor = \"#ffe4c4\"; label = \"%s\"];\n", node, array_command[i].name);
    }

    else if (node->type == BLOCK)
    {
        fprintf (file_dot, "node%p [shape=circle; style = filled; fillcolor = \"orange\"; label = \"%s\"];\n", node, ";");
    }

    else if (node->type == FUNC)
    {
        fprintf (file_dot, "node%p [shape=circle; style = filled; fillcolor = \"\"; label = \"%s\"];\n", node, tree->table_var[node->value.var]);
    }

    else if (node->type == VAR)
    {
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = pink; label = \"%s\"];\n", node, tree->table_var[node->value.var]);
    }

    if (node->left) 
    {
        fprintf (file_dot, "node%p -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->left);
        CreateDotUSER (node->left, file_dot, tree);
    }
    if (node->right) 
    {
        fprintf (file_dot, "node%p -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->right);
        CreateDotUSER (node->right, file_dot, tree);
    }
}

void PrintDot (Tree_t* tree, const char* file_input)
{
    FILE* file_dot = fopen ("./aaa.dot", "w");
    assert (file_dot != NULL);
    // fprintf (file_dot, "digraph{\nsplines=\"ortho\";\n");
    fprintf (file_dot, "digraph{\n");
    CreateDotUSER (tree->expression, file_dot, tree);
    fprintf (file_dot, "}");
    fclose (file_dot);

    char main[50] = "";
    sprintf (main, "dot ./aaa.dot -Tpng -o ./%s", file_input);
    system (main);
}