//
// Created by Matanel on 12/05/2021.
//

#include "vector.h"

vector *vector_alloc (vector_elem_cpy elem_copy_func,
                      vector_elem_cmp elem_cmp_func,
                      vector_elem_free elem_free_func)
{
  vector *new_vector = malloc (sizeof *new_vector);
  if (!new_vector)
    {
      return NULL;
    }
  if (!elem_free_func || !elem_copy_func || !elem_cmp_func)
    {
      return NULL;
    }
  new_vector->capacity = VECTOR_INITIAL_CAP;
  new_vector->size = 0;
  new_vector->data = calloc(sizeof (void *) , new_vector->capacity);
  if (!new_vector->data)
    {
      free (new_vector);
      return NULL;
    }
  new_vector->elem_cmp_func = elem_cmp_func;
  new_vector->elem_copy_func = elem_copy_func;
  new_vector->elem_free_func = elem_free_func;
  return new_vector;
}

void vector_free (vector **p_vector)
{
  if (!*p_vector)
    {
      return;
    }
  for (size_t i = 0; i < (*p_vector)->size; ++i)
    {
      (*p_vector)->elem_free_func (&(*p_vector)->data[i]);
    }
  free ((*p_vector)->data);
  free (*p_vector);
  *p_vector = NULL;
}

void *vector_at (const vector *vector, size_t ind)
{
  if (!vector ||  ind >= vector->size) //ind < 0 ||
    {
      return NULL;
    }
  return vector->data[ind];
}

int vector_find (const vector *vector, const void *value)
{
  if (!vector || !value)
    {
      return -1;
    }
  for (size_t i = 0; i < vector->size; ++i)
    {
      if (vector->elem_cmp_func (vector->data[i], value))
        {
          return (int) i;
        }
    }
  return -1;
}

/// increasing the capacity of the vector if needed.
/// \param vector the current vector
/// \return 1 if successful. 0, otherwise.
int vector_increase_capacity (vector *vector)
{
  vector->capacity *= VECTOR_GROWTH_FACTOR;
  // TODO: make sure it should be **tmp or *tmp below:
  void **tmp = realloc (vector->data, sizeof (void *) * vector->capacity);
  if (!tmp)
    {
      vector->capacity /= VECTOR_GROWTH_FACTOR;
      return 0;
    }
  vector->data = tmp;
  return 1;
}

int vector_push_back (vector *vector, const void *value)
{
  if (!vector || !value)
    {
      return -1;
    }

  // TODO: what if elem_copy_func(value) returns null due to alloc failure?

  vector->data[vector->size] = vector->elem_copy_func (value);
  vector->size++;
  if (VECTOR_MAX_LOAD_FACTOR < vector_get_load_factor (vector))
    {
      return vector_increase_capacity (vector);
    }
  return 1;
}

double vector_get_load_factor (const vector *vector)
{
  if (!vector)
    {
      return -1;
    }
  return (double) vector->size / (double) vector->capacity;
}

/// decreasing the capacity of the vector if needed.
/// \param vector the current vector
/// \return 1 if successful. 0, otherwise.
int vector_decrease_capacity (vector *vector)
{
  vector->capacity /= VECTOR_GROWTH_FACTOR;
  // TODO: make sure it should be **tmp or *tmp below:
  void **tmp = realloc (vector->data, sizeof (void *) * vector->capacity);
  if (!tmp)
    {
      vector->capacity *= VECTOR_GROWTH_FACTOR;
      return 0;
    }
  vector->data = tmp;

  return 1;
}

int vector_erase (vector *vector, size_t ind)
{
  if (!vector || ind >= vector->size) //ind < 0 ||
    {
      return 0;
    }
  vector->elem_free_func (&(vector->data[ind]));
  for (size_t i = ind; i < vector->size; ++i)
    {
      vector->data[i] = vector->data[i + 1];
    }
  vector->size--;

  if (VECTOR_MIN_LOAD_FACTOR > vector_get_load_factor (vector))
    {
      return vector_decrease_capacity (vector);
    }
  return 1;
}

void vector_clear (vector *vector)
{
  if (!vector)
    {
      return;
    }
  while (vector->size > 0)
    {
      vector_erase (vector, 0);
    }
}