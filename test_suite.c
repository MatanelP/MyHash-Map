//
// Created by Matanel on 12/05/2021.
//
#include "test_suite.h"
#include "hash_funcs.h"
#include "test_pairs.h"



void test_hash_map_insert_structs (void);

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exitcode 1.
 */
void test_hash_map_insert (void)
{

  hashmap *char_int_hashtable = hashmap_alloc (hash_char);
  assert(char_int_hashtable);

  int return_val = hashmap_insert (char_int_hashtable, NULL);
  assert(return_val == 0);
  // CREATING PAIRS:

  // char:int char_int_pairs:


  pair *char_int_pairs[12];
  pair *tmp;

  for (int j = 0; j < 12; ++j)
    {
      char char_key = (char) (j + 65);
      int int_value = j + 1;
      tmp = pair_alloc (&char_key, &int_value, char_key_cpy, int_value_cpy,
                        char_key_cmp, int_value_cmp, char_key_free,
                        int_value_free);
      assert(tmp);
      char_int_pairs[j] = tmp;

    }

  int k = 0;
  int value;
  size_t ind;
  // inserting char_int_pairs to hashtable:
  for (; k < 12; ++k)
    {
      return_val = hashmap_insert (char_int_hashtable, char_int_pairs[k]);
      assert(return_val == 1);
      ind = char_int_hashtable->hash_func (char_int_pairs[k]->key)
            & (char_int_hashtable->capacity - 1);
      value = *((int *) ((pair *) char_int_hashtable->buckets[ind]
                                                          ->data[0])->value);
      assert(value == k + 1);
      assert(char_int_hashtable->size == (size_t) k + 1);
      assert(char_int_hashtable->capacity == 16);
    }

  // inserting another pair to check capacity increase.
  char key_13 = (char) 65 + 12;
  int value_13 = 13;
  tmp = pair_alloc (&key_13, &value_13, char_key_cpy, int_value_cpy,
                    char_key_cmp, int_value_cmp, char_key_free,
                    int_value_free);
  assert(tmp);
  pair *char_int_pair_13 = tmp;
  return_val = hashmap_insert (NULL, char_int_pair_13);
  assert(return_val == 0);
  return_val = hashmap_insert (char_int_hashtable, char_int_pair_13);
  assert(return_val == 1);
  assert(char_int_hashtable->capacity == 32);
  assert(char_int_hashtable->size == 13);
  ind = char_int_hashtable->hash_func (char_int_pair_13->key)
        & (char_int_hashtable->capacity - 1);
  value = *((int *) ((pair *) char_int_hashtable->buckets[ind]
                                                        ->data[0])->value);
  assert(value == value_13);

  // making sure there is no data lost after capacity changed:
  k = 0;
  for (; k < 12; ++k)
    {
      ind = char_int_hashtable->hash_func (char_int_pairs[k]->key)
            & (char_int_hashtable->capacity - 1);
      value = *((int *) ((pair *) char_int_hashtable->buckets[ind]
                                                          ->data[0])->value);
      assert(value == k + 1);

    }

  // trying to add already existing key, should not work:
  char key_13b = (char) 65 + 12;
  int value_13b = 13;
  tmp = pair_alloc (&key_13b, &value_13b, char_key_cpy, int_value_cpy,
                    char_key_cmp, int_value_cmp, char_key_free,
                    int_value_free);
  assert(tmp);
  pair *char_int_pair_13b = tmp;
  return_val = hashmap_insert (char_int_hashtable, char_int_pair_13b);
  assert(return_val == 0);

  // Free the pairs.
  for (int k_i = 0; k_i < 12; ++k_i)
    {
      pair_free ((void **) &char_int_pairs[k_i]);
    }

  pair_free ((void **) &char_int_pair_13);
  pair_free ((void **) &char_int_pair_13b);

  //Free the hash-map.
  hashmap_free (&char_int_hashtable);
  test_hash_map_insert_structs ();

}


void test_hash_map_insert_structs (void)
{
  hashmap *double_point_hashtable = hashmap_alloc (hash_double);
  assert(double_point_hashtable);

  // CREATING PAIRS:

  // char:int char_int_pairs:


  pair *double_point_pairs[12];
  pair *tmp;

  for (int j = 0; j < 12; ++j)
    {
      Point* point_value = malloc (sizeof (Point));
      assert(point_value);
      point_value->_x = j;
      point_value->_y = (26-j);
      double double_key = ((point_value->_x)*(point_value->_x)
                               + (point_value->_y)*(point_value->_y));

      tmp = pair_alloc (&double_key, point_value,
                        double_key_cpy, point_value_cpy,
                        double_key_cmp, point_value_cmp,
                        double_key_free, point_value_free);
      assert(tmp);
      double_point_pairs[j] = tmp;
      free(point_value);
    }
  int k = 0;
  int value;
  size_t ind;
  int return_val;
  // inserting char_int_pairs to hashtable:
  for (; k < 12; ++k)
    {
      return_val = hashmap_insert (double_point_hashtable,
                                   double_point_pairs[k]);
      assert(return_val == 1);
      ind = double_point_hashtable->hash_func (double_point_pairs[k]->key)
            & (double_point_hashtable->capacity - 1);
      value = 0;
      for (size_t j = 0; j < double_point_hashtable->buckets[ind]->size; ++j)
        {
          double v = (double) k;
          tmp = vector_at (double_point_hashtable->buckets[ind], j);
          double key = ((v)*(v) + (26-v)*(26-v));
          keyT key_p = &key;
          if (tmp->key_cmp (tmp->key, key_p))
            {
              value = 1;
              break;
            }
        }
      assert(value == 1);
      assert(double_point_hashtable->size == (size_t) k + 1);
      assert(double_point_hashtable->capacity == 16);
    }

  // inserting another pair to check capacity increase.

  Point* point_value_13 = malloc (sizeof (Point));
  assert(point_value_13);
  point_value_13->_x = -1;
  point_value_13->_y = (27);
  double double_key = ((point_value_13->_x)*(point_value_13->_x)
                           + (point_value_13->_y)*(point_value_13->_y));

  tmp = pair_alloc (&double_key, point_value_13,
                    double_key_cpy, point_value_cpy,
                    double_key_cmp, point_value_cmp,
                    double_key_free, point_value_free);
  assert(tmp);
  pair *double_point_pair_13 = tmp;
  return_val = hashmap_insert (double_point_hashtable, double_point_pair_13);
  assert(return_val == 1);
  assert(double_point_hashtable->capacity == 32);
  assert(double_point_hashtable->size == 13);
  ind = double_point_hashtable->hash_func (double_point_pair_13->key)
        & (double_point_hashtable->capacity - 1);

  value = 0;
  for (size_t j = 0; j < double_point_hashtable->buckets[ind]->size; ++j)
    {
      tmp = vector_at (double_point_hashtable->buckets[ind], j);
      double key = ((-1)*(-1) + (27)*(27));
      keyT key_p = &key;
      if (tmp->key_cmp (tmp->key, key_p))
        {
          value = 1;
          break;
        }
    }
  assert(value == 1);
  // making sure there is no data lost after capacity changed:
  k = 0;
  for (; k < 12; ++k)
    {
      ind = double_point_hashtable->hash_func (double_point_pairs[k]->key)
            & (double_point_hashtable->capacity - 1);
      value = 0;
      for (size_t j = 0;  double_point_hashtable->buckets[ind] &&
                          j < double_point_hashtable->buckets[ind]->size; ++j)
        {
          double v = (double) k;
          tmp = vector_at (double_point_hashtable->buckets[ind], j);
          double key = ((v)*(v) + (26-v)*(26-v));
          keyT key_p = &key;
          if (tmp->key_cmp (tmp->key, key_p))
            {
              value = 1;
              break;
            }
        }
      assert(value == 1);
    }

  for (int k_i = 0; k_i < 12; ++k_i)
    {
      pair_free ((void **) &double_point_pairs[k_i]);
    }
  pair_free ((void **) &double_point_pair_13);
  free(point_value_13);
  hashmap_free (&double_point_hashtable);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{
  hashmap *char_int_hashtable = hashmap_alloc (hash_char);
  assert(char_int_hashtable);

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
      assert(tmp);
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
  assert(char_int_hashtable);

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
      assert(tmp);
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
      value = *((int *) ((pair *) char_int_hashtable->buckets[ind]
                                                            ->data[0])->value);
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
 * This function checks the hashmap_get_load_factor function of
 * the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits
 * with exit code 1.
 */
void test_hash_map_get_load_factor (void)
{
  hashmap *char_int_hashtable = hashmap_alloc (hash_char);
  assert(char_int_hashtable);

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
      assert(tmp);
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
  assert(tmp);
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
 * If HashMapGetApplyIf fails at some points, the functions exits with exit
 * code 1.
 */
void test_hash_map_apply_if ()
{
  hashmap *char_int_hashtable = hashmap_alloc (hash_char);
  assert(char_int_hashtable);

  int return_val = hashmap_apply_if (NULL, is_in_even_index,
                                                               double_value);
  assert(return_val == -1);
  return_val = hashmap_apply_if (char_int_hashtable, NULL,
                                                               double_value);
  assert(return_val == -1);
  return_val = hashmap_apply_if (char_int_hashtable, is_in_even_index,
                                                              NULL);
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
      assert(tmp);
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
  return_val = hashmap_apply_if (char_int_hashtable, is_in_even_index,
                                                                double_value);
  assert(return_val==6);

  // making sure the wanted values actually changed:
  k = 0;
  for (; k < 12; ++k)
    {
      ind = char_int_hashtable->hash_func (pairs[k]->key)
            & (char_int_hashtable->capacity - 1);
      if (char_int_hashtable->buckets[ind] && k%2 != 0)
        {
          value = *((int *) ((pair *) char_int_hashtable->buckets[ind]->
                                                            data[0])->value);
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