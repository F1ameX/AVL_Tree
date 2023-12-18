#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

char *getStr()
{
    int len = 0, capacity = 1;
    char c = getchar();
    char *str = (char*)malloc(sizeof(char));

    while (c != '\n')
    {
        str[len++] = c;

        if (len >= capacity)
        {
            capacity *= 2;
            str = (char*)realloc(str, capacity * sizeof(char));
        }
        c = getchar();
    }

    str[len] = '\0';
    return str;
}


char* itoa(int number)
{
    int len = 1;
    char symbol, *str = (char*)malloc(sizeof(char) * len);

    while (number)
    {
        str[len - 1] = (char)(number % 10 + '0');
        number /= 10;
        str = (char*)realloc(str, sizeof(char) * ++len);
    }

    str[(len--) - 1] = '\0';

    for (int i = 0, j = len - 1; i < j; i++, j--)
    {
        symbol = str[i];
        str[i] = str[j];
        str[j] = symbol;
    }

    return str;
}


TREE* load_data()
{
    TREE* tree = NULL;

    FILE* data;
    data = fopen("../data.txt", "r");

    if (data != NULL)
    {
        int index;
        char* question = NULL;
        size_t len = 0;

        if (fscanf(data, "%d ", &index) == 1 && getline(&question, &len, data) != -1)
            tree = create(index, strdup(question));

        while (fscanf(data, "%d ", &index) == 1 && getline(&question, &len, data) != -1)
            insert(tree, create(index,strdup(question)));

        free(question);
        fclose(data);
    }
    return tree;
}


void update_data(int node_index, char* new_question, char* left, char* right)
{
    char buffer[BUFFER_SIZE];
    FILE* data = fopen("../data.txt", "r+");
    FILE* temp = fopen("../temp.txt", "w");

    while(fgets(buffer, sizeof(buffer), data) != NULL)
    {
        if (strstr(buffer, strcat(itoa(node_index), " ")))
            fprintf(temp, "%d %s\n", node_index, new_question);
        else
            fputs(buffer, temp);
    }

    fprintf(temp, "%d %s\n", 2 * node_index, left);
    fprintf(temp, "%d %s\n", 2 * node_index + 1, right);

    fclose(temp);
    fclose(data);

    remove("../data.txt");
    rename("../temp.txt", "../data.txt");
}


void game(TREE* tree)
{
    int flag;
    puts("Let`s start! Please wish for a plane and I'll try to guess. Remember: only 'yes' or 'no'!");

    while(tree != NULL)
    {
        char answer[4];
        puts(tree->question);
        scanf("%s", answer);

        if (strcmp(answer, "yes") == 0)
        {
            if (tree->right != NULL)
                tree = tree->right;
            else
            {
                flag = 1;
                break;
            }
        }
        else if (strcmp(answer, "no") == 0)
        {
            if (tree->left != NULL)
                tree = tree->left;
            else
            {
                flag = 0;
                break;
            }

        }
    }

    if (flag)
    {
        puts("It was interesting! Thanks for playing!");
        return;
    }

    else
    {
        char is_this[BUFFER_SIZE] = "Is this ";
        char *new_leaf, *new_question, *new_answer;

        getchar();
        puts("What did you wish for?");
        new_leaf = getStr();

        puts("What question separates these answers?\n");
        new_question = getStr();

        puts("What is the answer to your question for this plane. Yes or no?");
        new_answer = getStr();


        char* final_question = strcat(is_this, new_leaf);
        final_question = strcat(final_question, "?");

        if (strcmp(new_answer, "yes") == 0)
            update_data(tree->index, new_question, tree->question, final_question);
        else
            update_data(tree->index, new_question, final_question, tree->question);

        free(new_leaf);
        free(new_question);
        free(new_answer);
        return;
    }
}


int main() {
    char ready_check[3];
    TREE* tree = NULL;

    puts("Welcome aboard, captain! Ready to guess your favorite plane! Are you ready!? Enter only 'yes' or 'no' please.");

    while (tree == NULL)
    {
        scanf("%s", ready_check);
        if (strcmp(ready_check, "yes") == 0)
        {
            puts("Good luck!");
            tree = load_data();
            game(tree);
        }

        else if (strcmp(ready_check, "no") == 0)
        {
            puts("See you later!");
            return 0;
        }
    }
    return 0;
}