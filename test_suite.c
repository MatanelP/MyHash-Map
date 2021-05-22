//
// Created by Matanel on 12/05/2021.
//

#include "test_suite.h"
#include "hash_funcs.h"
#include "test_pairs.h"
#include <stdio.h>

void print_vector (vector *p_vector)
{
  for (size_t i = 0; i < p_vector->capacity; ++i)
    {
      pair *cur = vector_at (p_vector, i);
      if (cur == NULL)
        {
          printf ("  X  |");
        }
      else
        {
          printf (" %c:%i |", *(char *) cur->key, *(int *) cur->value);
        }
    }
}
void print_hashmap (hashmap *hash_map)
{
  for (size_t i = 0; i < hash_map->capacity; ++i)
    {
      printf ("buckets[%zu]: \n\t\t\t[", i);
      print_vector (hash_map->buckets[i]);
      printf ("*]\n");
    }
  printf ("\n\n");

}

void test_hash_map_init ()
{
  hashmap *new_hashtable = hashmap_alloc (hash_int);
  assert(new_hashtable->size == 0);
  assert(new_hashtable->capacity == 16);
  assert(new_hashtable->hash_func != NULL);
  assert(new_hashtable->buckets != NULL);
  hashmap_free (&new_hashtable);
  assert(new_hashtable == NULL);
}
/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert (void)
{
  test_hash_map_init ();

  hashmap *char_int_hashtable = hashmap_alloc (hash_char);

  int return_val = hashmap_insert (char_int_hashtable, NULL);
  assert(return_val == 0);
  // CREATING PAIRS:

  // char:int pairs:


  pair *pairs[12];
  pair *tmp;

  for (int j = 0; j < 12; ++j)
    {
      char key = (char) (j + 65);
      int value = j + 1;
      tmp = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                        char_key_cmp, int_value_cmp, char_key_free,
                        int_value_free);
      if (!tmp)
        {
          break;
        }
      pairs[j] = tmp;
    }
  int k = 0;
  int value;
  size_t ind;
  double load_factor;
  // inserting pairs to hashtable:
  for (; k < 12; ++k)
    {
      return_val = hashmap_insert (char_int_hashtable, pairs[k]);
      assert(return_val == 1);
      ind = char_int_hashtable->hash_func (pairs[k]->key)
            & (char_int_hashtable->capacity - 1);
      value = *((int *) ((pair *) char_int_hashtable->buckets[ind]->data[0])->value);
      assert(value == k + 1);
      assert(char_int_hashtable->size == (size_t) k + 1);
      assert(char_int_hashtable->capacity == 16);
      load_factor = hashmap_get_load_factor (char_int_hashtable);
      assert(load_factor == ((k + 1) / 16.0));
    }

  // inserting another pair to check capacity increase.
  char key_13 = (char) 65 + 12;
  int value_13 = 13;
  tmp = pair_alloc (&key_13, &value_13, char_key_cpy, int_value_cpy,
                    char_key_cmp, int_value_cmp, char_key_free,
                    int_value_free);
  pair *pair_13 = tmp;
  return_val = hashmap_insert (NULL, pair_13);
  assert(return_val == 0);
  return_val = hashmap_insert (char_int_hashtable, pair_13);
  assert(return_val == 1);
  assert(char_int_hashtable->capacity == 32);
  assert(char_int_hashtable->size == 13);
  load_factor = hashmap_get_load_factor (char_int_hashtable);
  assert(load_factor == 0.40625);
  ind = char_int_hashtable->hash_func (pair_13->key)
        & (char_int_hashtable->capacity - 1);
  value = *((int *) ((pair *) char_int_hashtable->buckets[ind]->data[0])->value);
  assert(value == value_13);

  // making sure there is no data lost after capacity changed:
  k = 0;
  for (; k < 12; ++k)
    {
      ind = char_int_hashtable->hash_func (pairs[k]->key)
            & (char_int_hashtable->capacity - 1);
      value = *((int *) ((pair *) char_int_hashtable->buckets[ind]->data[0])->value);
      assert(value == k + 1);
    }

  // trying to add already existing key, should not work:
  char key_13b = (char) 65 + 12;
  int value_13b = 13;
  tmp = pair_alloc (&key_13b, &value_13b, char_key_cpy, int_value_cpy,
                    char_key_cmp, int_value_cmp, char_key_free,
                    int_value_free);
  pair *pair_13b = tmp;
  return_val = hashmap_insert (char_int_hashtable, pair_13b);
  assert(return_val == 0);

  // Free the pairs.
  for (int k_i = 0; k_i < 12; ++k_i)
    {
      pair_free ((void **) &pairs[k_i]);
    }
  pair_free ((void **) &pair_13);
  pair_free ((void **) &pair_13b);

  //Free the hash-map.
  hashmap_free (&char_int_hashtable);

}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{
  hashmap *char_int_hashtable = hashmap_alloc (hash_char);

  void *return_val = hashmap_at (char_int_hashtable, NULL);
  assert(return_val == NULL);

  // CREATING PAIRS:

  // char:int pairs:

  pair *pairs[12];
  pair *tmp;
  for (int j = 0; j < 12; ++j)
    {
      char key = (char) (j + 65);
      int value = j + 1;
      tmp = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                        char_key_cmp, int_value_cmp, char_key_free,
                        int_value_free);
      if (!tmp)
        {
          break;
        }
      pairs[j] = tmp;
    }
  int k = 0;
  void *value;

  // try to get a value for a non-existing key (before any insertion):
  value = hashmap_at (char_int_hashtable, pairs[k]->key);
  assert(value == NULL);

  // inserting pairs to hashtable:
  for (; k < 12; ++k)
    {
      hashmap_insert (char_int_hashtable, pairs[k]);
      value = hashmap_at (char_int_hashtable, pairs[k]->key);
      assert(*(int *) value == k + 1);
    }
  // Free the pairs.
  for (int k_i = 0; k_i < 12; ++k_i)
    {
      pair_free ((void **) &pairs[k_i]);
    }

  //Free the hash-map.
  hashmap_free (&char_int_hashtable);

}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void)
{

  hashmap *char_int_hashtable = hashmap_alloc (hash_char);

  int return_val = hashmap_erase (char_int_hashtable, NULL);
  assert(return_val == 0);
  // CREATING PAIRS:

  // char:int pairs:


  pair *pairs[5];
  pair *tmp;

  for (int j = 0; j < 5; ++j)
    {
      char key = (char) (j + 65);
      int value = j + 1;
      tmp = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                        char_key_cmp, int_value_cmp, char_key_free,
                        int_value_free);
      if (!tmp)
        {
          break;
        }
      pairs[j] = tmp;
    }
  int k = 0;
  int value;
  size_t ind;
  // inserting pairs to hashtable:
  for (; k < 5; ++k)
    {
      hashmap_insert (char_int_hashtable, pairs[k]);
    }
  return_val = hashmap_erase (char_int_hashtable, pairs[k - 1]->key);
  assert(return_val == 1);
  assert(char_int_hashtable->capacity == 16);
  assert(char_int_hashtable->size == 4);
  ind = char_int_hashtable->hash_func (pairs[k - 1]->key)
        & (char_int_hashtable->capacity - 1);
  assert(char_int_hashtable->buckets[ind]->data[0] == NULL);


  //if key not in map:
  return_val = hashmap_erase (char_int_hashtable, pairs[k - 1]->key);
  assert(return_val == 0);

  // delete another pair to check capacity decrease.
  hashmap_erase (char_int_hashtable, pairs[k - 2]->key);
  assert(char_int_hashtable->capacity == 8);
  assert(char_int_hashtable->size == 3);



  // making sure there is no data lost after capacity changed:
  k = 0;
  for (; k < 3; ++k)
    {
      ind = char_int_hashtable->hash_func (pairs[k]->key)
            & (char_int_hashtable->capacity - 1);
      value = *((int *) ((pair *) char_int_hashtable->buckets[ind]->data[0])->value);
      assert(value == k + 1);
    }

  hashmap_erase (char_int_hashtable, pairs[k - 1]->key);
  hashmap_erase (char_int_hashtable, pairs[k - 2]->key);
  assert(char_int_hashtable->capacity == 4);
  assert(char_int_hashtable->size == 1);
  hashmap_erase (char_int_hashtable, pairs[k - 3]->key);
  assert(char_int_hashtable->capacity == 2);
  assert(char_int_hashtable->size == 0);




  // Free the pairs.
  for (int k_i = 0; k_i < 5; ++k_i)
    {
      pair_free ((void **) &pairs[k_i]);
    }

  //Free the hash-map.
  hashmap_free (&char_int_hashtable);

}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor (void)
{
  hashmap *char_int_hashtable = hashmap_alloc (hash_char);

  double return_val = hashmap_get_load_factor (NULL);
  assert(return_val == -1);
  // CREATING PAIRS:

  // char:int pairs:


  pair *pairs[12];
  pair *tmp;

  for (int j = 0; j < 12; ++j)
    {
      char key = (char) (j + 65);
      int value = j + 1;
      tmp = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                        char_key_cmp, int_value_cmp, char_key_free,
                        int_value_free);
      if (!tmp)
        {
          break;
        }
      pairs[j] = tmp;
    }
  int k = 0;
  double load_factor;
  // inserting pairs to hashtable:
  for (; k < 12; ++k)
    {
      hashmap_insert (char_int_hashtable, pairs[k]);
      load_factor = hashmap_get_load_factor (char_int_hashtable);
      assert(load_factor == ((k + 1) / 16.0));
    }

  // inserting another pair to increase capacity.
  char key_13 = (char) 65 + 12;
  int value_13 = 13;
  tmp = pair_alloc (&key_13, &value_13, char_key_cpy, int_value_cpy,
                    char_key_cmp, int_value_cmp, char_key_free,
                    int_value_free);
  pair *pair_13 = tmp;
  hashmap_insert (char_int_hashtable, pair_13);
  load_factor = hashmap_get_load_factor (char_int_hashtable);
  assert(load_factor == 0.40625);

  // Free the pairs.
  for (int k_i = 0; k_i < 12; ++k_i)
    {
      pair_free ((void **) &pairs[k_i]);
    }
  pair_free ((void **) &pair_13);

  //Free the hash-map.
  hashmap_free (&char_int_hashtable);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if ()
{
  hashmap *char_int_hashtable = hashmap_alloc (hash_char);

  int return_val = hashmap_apply_if (NULL, is_in_even_index, double_value);
  assert(return_val == 0);
  return_val = hashmap_apply_if (char_int_hashtable, NULL, double_value);
  assert(return_val == 0);
  return_val = hashmap_apply_if (char_int_hashtable, is_in_even_index, NULL);
  assert(return_val == 0);
  // CREATING PAIRS:

  // char:int pairs:


  pair *pairs[12];
  pair *tmp;

  for (int j = 0; j < 12; ++j)
    {
      char key = (char) (j + 65);
      int value = j + 1;
      tmp = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                        char_key_cmp, int_value_cmp, char_key_free,
                        int_value_free);
      if (!tmp)
        {
          break;
        }
      pairs[j] = tmp;
    }
  int k = 0;
  int value;
  size_t ind;
  // inserting pairs to hashtable:
  for (; k < 12; ++k)
    {
      hashmap_insert (char_int_hashtable, pairs[k]);
    }
  return_val = hashmap_apply_if (char_int_hashtable, is_in_even_index, double_value);
  assert(return_val==6);

  // making sure the wanted values actually changed:
  k = 0;
  for (; k < 12; ++k)
    {
      ind = char_int_hashtable->hash_func (pairs[k]->key)
            & (char_int_hashtable->capacity - 1);
      if (k%2 != 0)
        {
          value = *((int *) ((pair *) char_int_hashtable->buckets[ind]->data[0])->value);
          assert(value == 2 * (k + 1));
        }
    }

  // Free the pairs.
  for (int k_i = 0; k_i < 12; ++k_i)
    {
      pair_free ((void **) &pairs[k_i]);
    }

  //Free the hash-map.
  hashmap_free (&char_int_hashtable);

}