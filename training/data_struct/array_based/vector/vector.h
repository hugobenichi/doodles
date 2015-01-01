/*
 *  a growable random-access array implementation in C
 *
 *  author: hugo benichi
 *  copyright: copyright 2013 hugo benichi
 *  contact: hugo[dot]benichi[at]m4x[dot]org
 *  status: toy
 */

struct Vector;                  /* forward declaration, cf end for definition */

typedef struct Vector vector;   /* shortcut name */

/*
 *  Allocates memory for a new vector object with at least capacity.
 *  The allocated memory address is written in pointer_to_vector_address.
 *  Returns -1 in case of malloc error, in which case *pointer_to_vector_address
 *  is set to NULL.
 *  The initial capacity is the first pwoer of two and multiple of 8 above the
 *  caller required capacity.
 */
int vector_new(vector** pointer_to_vector_address, int capacity);

/*
 * Frees memory of an allocated vector object.
 */
int vector_free(vector* vec);

/*
 *  Set cell at given index with given pointer value. Returns -1 if index is
 *  out of bound, 0 otherwise. O(1) time.
 */
int vector_set(vector* vec, void* object, int index);

/*
 *  Get cell at given index and writes it at given pointer address. Returns -1
 *  if index is out of bound. O(1) time.
 */
int vector_get(vector* vec, void** pointer_toobject, int index);

/*
 *  Insert element at given index and recopy tails shifted by one index. Grows
 *  vector if necessary. If index is 0 or negative, insertion is done at index
 *  0. Returns -1 if grow operation was unsuccessfull. O(n) expected time where
 *  n is the size of the vector.
 */
int vector_insert(vector* vec, void* object, int index);

/*
 *  Insert elements contained in given array of void pointers, starting from
 *  index. Shift elements to avoid overwritting memory cells and grows the
 *  vector if necessary. If from_index is 0 or negative, insertion is done at 
 *  index 0. Returns -1 if grow operation was unsuccessfull. O(n + l) expected
 *  time where n is the size of the vector and l is the number of elements to
 *  insert.
 */
int vector_insert_many(vector* vec, void** objects, int object_num, int from_index);

/*
 *  Appends element to the end of the vector. Grows vector size if necessary.
 *  Returns -1 if grow operation was unsuccessfull. O(1) amortized time.
 */
int vector_add(vector* vec, void* object);

int vector_rem(vector* vec, void** pointer_to_object); // pop an item from vec



struct Vector {
    void**  data;       /* array of void pointer (arbritrary objects) */
    int     len;        /* length of currently used data */
    int     cap;        /* total valid length of data */
};
