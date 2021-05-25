//
// Created by Matanel on 12/05/2021.
//
#include "hashmap.h"

hashmap *hashmap_alloc (hash_func func)
{
  hashmap *new_hashmap = malloc (sizeof *new_hashmap);
  vector **vectors = calloc (HASH_MAP_INITIAL_CAP, sizeof (vector *));
  if (!new_hashmap || !vectors)
    {
      return NULL;
    }
  new_hashmap->size = 0;
  new_hashmap->capacity = HASH_MAP_INITIAL_CAP;
  new_hashmap->buckets = vectors;
  new_hashmap->hash_func = func;
  return new_hashmap;
}

/**
 * freeing buckets, array of vectors.
 * @param capacity the size of the buckets
 * @param buckets pointer for the array
 */
void free_buckets (size_t capacity, vector **buckets)
{
  for (size_t i = 0; i < capacity; ++i)
    {
      vector_free (&(buckets[i]));
    }
  free (buckets);
}

void hashmap_free (hashmap **p_hash_map)
{
  if (!p_hash_map)
    {
      return;
    }
  free_buckets ((*p_hash_map)->capacity, (*p_hash_map)->buckets);
  free (*p_hash_map);
  *p_hash_map = NULL;
}


/**
 * rehashing the map. going through the old buckets one pair at a time and
 * inserting it correctly to a new buckets array.
 * updating the map buckets to be the new buckets.
 * @param p_hashmap pointer for the current map.
 * @param new_buckets pointer to a new array of vectors to be filled.
 * @param new_capacity the size of the new array.
 */
void rehash_after_capacity_change (hashmap *p_hashmap, vector **new_buckets,
                                   size_t new_capacity)
{
  size_t ind, fail = 0 ;
  pair *cur_pair;
  for (size_t i = 0; i < p_hashmap->capacity; ++i)
    {
      for (size_t j = 0; p_hashmap->buckets[i] &&
                                        j < p_hashmap->buckets[i]->size; ++j)
        {
          cur_pair = vector_at (p_hashmap->buckets[i], j);
          ind = p_hashmap->hash_func (cur_pair->key) & (new_capacity - 1);
          if (!(new_buckets[ind]))
            {
              vector *tmp = vector_alloc (pair_copy, pair_cmp, pair_free);
              if (!tmp)
                {
                  fail = 1;
                  break;
                }
              new_buckets[ind] = tmp;
            }
          vector_push_back (new_buckets[ind], cur_pair);
        }
    }
    if (!fail)
      {
        free_buckets (p_hashmap->capacity, p_hashmap->buckets);
        p_hashmap->buckets = new_buckets;
      }
}


/**
 * changing the map capacity according to direction (up/down)
 * @param hash_map the current map
 * @param direction 1 = up, otherwise = down.
 * @return 0 on fail, 1 on success.
 */
int hashmap_change_capacity (hashmap *hash_map, int direction)
{
  size_t new_capacity;
  if (direction == 1)
    {
      new_capacity = hash_map->capacity * HASH_MAP_GROWTH_FACTOR;
    }
  else
    {
      new_capacity = hash_map->capacity / HASH_MAP_GROWTH_FACTOR;
    }
  vector **new_buckets = calloc (new_capacity, sizeof (vector *));
  if (!new_buckets)
    {
      return 0;
    }
  rehash_after_capacity_change (hash_map, new_buckets, new_capacity);
  hash_map->capacity = new_capacity;
  return 1;
}

int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (!hash_map || !in_pair || hashmap_at (hash_map, in_pair->key))
    {
      return 0;
    }
  size_t ind = hash_map->hash_func (in_pair->key) & (hash_map->capacity - 1);
  if (!(hash_map->buckets[ind]))
    {
      vector *tmp = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (!tmp)
        {
          return 0;
        }
      hash_map->buckets[ind] = tmp;
    }
  vector_push_back (hash_map->buckets[ind], in_pair);
  hash_map->size++;
  if (HASH_MAP_MAX_LOAD_FACTOR < hashmap_get_load_factor (hash_map))
    {
      return hashmap_change_capacity (hash_map, 1);
    }
  return 1;
}

valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (!hash_map || !key)
    {
      return NULL;
    }
  pair *cur_pair;
  size_t ind = hash_map->hash_func (key) & (hash_map->capacity - 1);
  for (size_t j = 0; hash_map->buckets[ind] &&
                                        j < hash_map->buckets[ind]->size; ++j)
    {
      cur_pair = vector_at (hash_map->buckets[ind], j);
      if (cur_pair->key_cmp (cur_pair->key, key))
        {
          return cur_pair->value;
        }
    }
  return NULL;
}

/**
 * deleting a pair from a vector at index 'ind'
 * in buckets in position j in the vector.
 * @param hash_map the current map
 * @param ind the index in bucket to get the correct vector.
 * @param j the index inside the vector for delete the pair in it.
 * @return 0 on fail, 1 on success.
 */
int delete_pair_in_index (hashmap *hash_map, size_t ind, size_t j)
{
  vector_erase (hash_map->buckets[ind], j);
  hash_map->size--;
  if (HASH_MAP_MIN_LOAD_FACTOR > hashmap_get_load_factor (hash_map))
    {
      return hashmap_change_capacity (hash_map, -1);
    }
  return 1;
}

int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (!hash_map || !key)
    {
      return 0;
    }
  pair *cur_pair;
  size_t ind = hash_map->hash_func (key) & (hash_map->capacity - 1);
  for (size_t j = 0; j < hash_map->buckets[ind]->size; ++j)
    {
      cur_pair = vector_at (hash_map->buckets[ind], j);
      if (cur_pair->key_cmp (cur_pair->key, key))
        {
          return delete_pair_in_index (hash_map, ind, j);
        }
    }
  //if got here, key not in map
  return 0;

}

double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (!hash_map)
    {
      return -1;
    }
  return (double) hash_map->size / (double) hash_map->capacity;
}

int hashmap_apply_if (const hashmap *hash_map,
                      keyT_func keyT_func, valueT_func valT_func)
{
  int values_changed = 0;
  if (!hash_map || !keyT_func || !valT_func)
    {
      return -1;
    }
  for (size_t i = 0; i < hash_map->capacity; ++i)
    {
      for (size_t j = 0; hash_map->buckets[i]
                                      && j <  hash_map->buckets[i]->size; ++j)
        {
          pair *cur_pair = vector_at (hash_map->buckets[i], j);
          if (keyT_func (cur_pair->key))
            {
              valT_func (cur_pair->value);
              values_changed++;
            }
        }
    }
  return values_changed;
}