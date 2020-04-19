//
// Created by Minerkow on 10.04.2020.
//
#include "Calculator.h"

struct variable_t
{
    int value;
    char* name;
};

struct variable_t* create_hash_table()
{
    return (struct variable_t*)calloc(LENHASHTABLE, sizeof(struct variable_t));
}

int check_hash_table(struct variable_t* hash_table, int hash, char* word)
{
    if(hash_table[hash].name == NULL)
    {
        hash_table[hash].name = word;
        return hash;
    } else{
        int i = 0;
        while (hash_table[hash + i].name)
        {
            if (!strcmp(word, hash_table[hash + i].name))
                return hash + i;
            i++;
        }
        hash_table[hash + i].name = word;
        return hash + i;
    }
}

int calc_hash(int hash, int numsymbol, char symbol)
{
    return hash % ALFABETLEN + symbol * exponent(KEY, numsymbol) % ALFABETLEN;
}

int exponent(int number, int degree)
{
    int res = 1;
    for (int i = 0; i < degree; ++i)
        res *= number * number;
    return res;
}


//Save the value of variable if input == false
//Returns the value of the variable if input == true
//Removes hash_table if hash == -1

int variable_value(int hash, int value, bool input,  struct variable_t* ptr)
{
    static struct variable_t* hash_table = NULL;
    if (ptr != NULL) {
        hash_table = ptr;
        return 0;
    }
    if (input == true)
    {
        hash_table[hash].value = value;
        return 0;
    }
    if (input == false)
    {
        return hash_table[hash].value;
    }
    if (hash == -1)
    {
        free(hash_table);
    }
    printf("Upsss");
    return 0;
}
