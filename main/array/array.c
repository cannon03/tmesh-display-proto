#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  void *data;          // Pointer to the data
  size_t element_size; // Size of each element
  size_t size;         // Number of elements
  size_t capacity;     // Total capacity
} dynamic_array;

// Dynamic Array Functions
dynamic_array *dynamic_array_create(size_t element_size,
                                    size_t initial_capacity);
void dynamic_array_free(dynamic_array *arr);
bool dynamic_array_append(dynamic_array *arr, const void *element);
void *dynamic_array_get(dynamic_array *arr, size_t index);
bool dynamic_array_remove(dynamic_array *arr, size_t index);
size_t dynamic_array_size(const dynamic_array *arr);

dynamic_array *dynamic_array_create(size_t element_size,
                                    size_t initial_capacity) {
  dynamic_array *arr = (dynamic_array *)malloc(sizeof(dynamic_array));
  if (!arr)
    return NULL;

  arr->data = malloc(element_size * initial_capacity);
  if (!arr->data) {
    free(arr);
    return NULL;
  }

  arr->element_size = element_size;
  arr->size = 0;
  arr->capacity = initial_capacity;
  return arr;
}

void dynamic_array_free(dynamic_array *arr) {
  if (arr) {
    free(arr->data);
    free(arr);
  }
}

bool dynamic_array_append(dynamic_array *arr, const void *element) {
  if (!arr || !element)
    return false;

  // Resize if necessary
  if (arr->size >= arr->capacity) {
    size_t new_capacity = arr->capacity * 2;
    void *new_data = realloc(arr->data, arr->element_size * new_capacity);
    if (!new_data)
      return false;

    arr->data = new_data;
    arr->capacity = new_capacity;
  }

  // Copy the element to the end of the array
  memcpy((char *)arr->data + (arr->size * arr->element_size), element,
         arr->element_size);
  arr->size++;
  return true;
}

void *dynamic_array_get(dynamic_array *arr, size_t index) {
  if (!arr || index >= arr->size)
    return NULL;
  return (char *)arr->data + (index * arr->element_size);
}

bool dynamic_array_remove(dynamic_array *arr, size_t index) {
  if (!arr || index >= arr->size)
    return false;

  // Move all elements after index one position back
  char *dest = (char *)arr->data + (index * arr->element_size);
  char *src = dest + arr->element_size;
  size_t bytes_to_move = (arr->size - index - 1) * arr->element_size;

  if (bytes_to_move > 0) {
    memmove(dest, src, bytes_to_move);
  }

  arr->size--;
  return true;
}

size_t dynamic_array_size(const dynamic_array *arr) {
  return arr ? arr->size : 0;
}