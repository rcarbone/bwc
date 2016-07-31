/* System headers */
#include <stdio.h>


/* public header */
#include "hashtab.h"


static hashval_t my_hash (const void * key)
{
  return * (unsigned *) key;
}


static int my_cmp (const void * key1, const void * key2)
{
  return * (unsigned *) key1 == * (unsigned *) key2;
}


int main (int argc, char * argv [])
{
  unsigned key = 11;
  unsigned val = 22;
  void ** item;
  void * hit;

  /* Create a new hash table */
  struct htab * ht = htab_create (1, my_hash, my_cmp, NULL);  /* NULL is for the delete callback */

  /* Add a new entry */
  printf ("Add [%u / %u] ... ", key, val);
  item = htab_find_slot (ht, & key, INSERT);
  if (item && htab_elements (ht) == 1)
    {
      * item = & key;
      printf ("Ok\n");
    }
  else
    printf ("Failed\n");

  /* Get the value */
  printf ("Get [%u] ........ ", key);
  hit = htab_find (ht, & key);
  if (hit)
    printf ("Ok - %u\n", * (unsigned *) hit);
  else
    printf ("Failed\n");

  /* Delete the entry */
  printf ("Del [%u] ........ ", key);
  htab_remove_elt (ht, & key);
  if (htab_elements (ht) == 0)
    printf ("Ok - %u\n", val);
  else
    printf ("Failed\n");

  /* Frees all resources allocated by the hash */
  htab_delete (ht);

  return 0;
}
