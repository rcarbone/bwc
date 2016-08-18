#include <stdlib.h>
#include <string.h>

#include "precomp.h"

/* The implementation */
#define HEADER "khash.h"

#include HEADER

/* Instantiate structs and methods */
KHASH_MAP_INIT_STR(kht, unsigned)


class Khash_HashTableTester : public BaseTester
{
private:

  /* The hashtable */
  khash_t(kht) * ht;

public:

  /* Costructor */
  Khash_HashTableTester (uint size)
  {
    ht = kh_init (kht);
  }


  /* Destructor */
  ~Khash_HashTableTester (void)
  {
    kh_destroy (kht, ht);
  }


  void readWords (WordReader & reader)
  {
    while (const char * word = reader . getWord ())
      {
	int ret;
	khiter_t hi;
	hi = kh_put (kht, ht, word, & ret);
	kh_value (ht, hi) ++;
      }
  }

  void getWords (vector <pair <int, string> > & wordList)
  {
    khiter_t hi;
    wordList . reserve (kh_size (ht));

    for (hi = kh_begin (ht); hi != kh_end (ht); hi)
      wordList . push_back (pair <int, string> (kh_value (ht, hi), kh_key (ht, hi)));
  }
};


BaseTester * Create_Khash_HashTableTester (uint param)
{
  return new Khash_HashTableTester (param);
}

