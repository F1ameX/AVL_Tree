#ifndef AVL_TREE_TREE_H
#define AVL_TREE_TREE_H
#pragma once

#define TREE struct tree

#include <stdio.h>
#include <stdlib.h>


TREE
{
    int index;
    char* question;
    TREE* left;
    TREE* right;
};


TREE* create(int index, char* question)
{
    TREE* result;
    result = (TREE*)malloc(sizeof(TREE));

    result->index = index;
    result->question = question;

    result->left = NULL;
    result->right = NULL;

    return result;
}


void insert(TREE* tree, TREE* new_question)
{
    if (tree != NULL)
    {
        if (2 * tree->index == new_question->index)
            tree->left = new_question;

        else if (2 * tree->index + 1 == new_question->index)
            tree->right = new_question;

        else
        {
            insert(tree->left, new_question);
            insert(tree->right, new_question);
        }
    }
}
#endif //AVL_TREE_TREE_H