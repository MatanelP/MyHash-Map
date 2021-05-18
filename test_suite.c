//
// Created by Matanel on 12/05/2021.
//

#include "test_suite.h"
#include "hash_funcs.h"
#include "test_pairs.h"
//#include "pair_char_int.h"

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert (void)
{
  hashmap *new_hashtable = hashmap_alloc (hash_int);
  size_t size = new_hashtable->size;
  assert(size == 0);
  assert(new_hashtable->capacity == 16);
  assert(new_hashtable->hash_func != NULL);
  assert(new_hashtable->buckets != NULL);
  assert(new_hashtable->buckets != NULL);

  pair *pairs[60];
  for (int j = 0; j < 60; ++j)
    {
      char key = (char) (j + 65);

      int value = j+10;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
    }
  int k = 0;
  for (; k < 12; ++k)
    {
      hashmap_insert (new_hashtable, pairs[k]);
      assert(new_hashtable->size == k+1);
      assert(new_hashtable->capacity == 16);
      assert(hashmap_get_load_factor (new_hashtable) == ((k+1)/16.0));
    }

  hashmap_insert (new_hashtable, pairs[k]);

  assert(new_hashtable->size == 13);
  assert(new_hashtable->capacity == 32);
  assert(hashmap_get_load_factor (new_hashtable) == 0.40625);

  for (; k < 24; ++k)
    {
      hashmap_insert (new_hashtable, pairs[k]);
      assert(new_hashtable->size == k+1);
      assert(new_hashtable->capacity == 32);
      assert(hashmap_get_load_factor (new_hashtable) == ((k+1)/32.0));
    }
    char x = 'e';
  const_keyT y = &x;
  valueT z = hashmap_at (new_hashtable, y);
  int a = 5;


//  //apply double_value on values where key is a digit
//  char key_dig = '2', key_letter = 'D';
//  printf ("map['2'] before apply if: %d, map['D'] before apply if: %d\n",
//          *(int *) hashmap_at (map, &key_dig),
//          *(int *) hashmap_at (map, &key_letter));
//  int apply_if_res = hashmap_apply_if (map, is_digit, double_value);
//  printf ("Number of changed values: %d\n", apply_if_res);
//  printf ("map['2'] after apply if: %d, map['D'] after apply if: %d\n",
//          *(int *) hashmap_at (map, &key_dig),
//          *(int *) hashmap_at (map, &key_letter));
//
//  // Free the pairs.
//  for (int k_i = 0; k_i < 8; ++k_i)
//    {
//      pair_free ((void **) &pairs[k_i]);
//    }
//
//  //Free the hash-map.
//  hashmap_free (&map);
//

}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{

}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void)
{

}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor (void)
{

}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if ()
{

}