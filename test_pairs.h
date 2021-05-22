//
// Created by Matanel on 12/05/2021.
//

#ifndef _TEST_PAIRS_H_
#define _TEST_PAIRS_H_

#include "pair.h"
/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Copies the int value of the pair.
 */
void *int_value_cpy (const_valueT value)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) value);
  return new_int;
}

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

/**
 * Compares the int value of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}

/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT* key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}


/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents in int is even digit, else - 0
 */
int is_in_even_index (const_keyT elem)
{
  char c = *((char *) elem);
  if (c%2 == 0)
  {
      return 1;
  }
  return 0;
}

/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a digit, else - 0
 */
int is_digit (const_keyT elem)
{
  char c = *((char *) elem);
  return (c > 47 && c < 58);
}

/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
  *((int *) elem) *= 2;
}

#endif //_TEST_PAIRS_H_
