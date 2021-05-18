//
// Created by Matanel on 12/05/2021.
//

#ifndef _TEST_PAIRS_H_
#define _TEST_PAIRS_H_

#include "pair.h"
/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key);

/**
 * Copies the int value of the pair.
 */
void *int_value_cpy (const_valueT value);

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2);
/**
 * Compares the int value of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2);

/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT* key);

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val);


/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a digit, else - 0
 */
int is_digit (const_keyT elem);

/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem);

#endif //_TEST_PAIRS_H_
