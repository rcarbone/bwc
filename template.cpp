#include <stdlib.h>
#include <string.h>

#include "precomp.h"

/* The implementation */
#define HEADER "hashtab.h"

#include HEADER
typedef htab_t ht_t;


class Name_HashTableTester : public BaseTester
{
private:

  /* The hashtable */
  ht_t ht;

public:

  /* Costructor */
  Name_HashTableTester (uint size)
  {
    ht = xxx ();
  }

  /* Destructor */
  ~Name_HashTableTester (void)
  {
    xxx (ht);
  }

  void readWords (WordReader & reader)
  {
    while (const char * word = reader . getWord ())
      {
	xxx_put (ht, word);
      }
  }

  void getWords (vector <pair <int, string> > & wordList)
  {
    wordList . reserve (xxx_count (ht));
    /* iterate */
  }
};


BaseTester * Create_Name_HashTableTester (uint param)
{
  return new Name_HashTableTester (param);
}

