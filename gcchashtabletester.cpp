#include <stdlib.h>
#include <string.h>

#include "precomp.h"
#include <hashtab.h>


class GCCHashTableTester : public BaseTester
{
private:

  struct Node
  {
    char * str;
    uint count;

    Node (const char * s) : count() { str = (char *) malloc (strlen (s) + 1); strcpy (str, s); }
    ~Node () { free (str); }

    static int htab_eq (const void * entry, const void * element) { return strcmp (((Node *) entry) -> str, (const char *) element) == 0; }
    static void htab_del (void * key) { delete (Node *) key; }
  };

  static hashval_t fnv1Hash (const char * key)
  {
    uint hash = 2166136261;
    for (const char * s = key; * s; s ++)
      hash = (16777619 * hash) ^ (* s);
    return hash;
  }

  static hashval_t htab_hash (const void * node)
  {
    return fnv1Hash (((Node *) node) -> str);
  }

  htab_t m_hTab;

public:

  GCCHashTableTester (uint size)
  {
    m_hTab = htab_create_alloc (size, htab_hash, Node::htab_eq, Node::htab_del, calloc, free);
  }

  ~GCCHashTableTester ()
  {
    htab_delete (m_hTab);
  }

  void readWords (WordReader &reader)
  {
    while (const char * word = reader . getWord())
      {
	hashval_t hash = fnv1Hash (word);
	void ** slot = htab_find_slot_with_hash (m_hTab, word, hash, INSERT);
	if (* slot == NULL)
	  {
	    * slot = new Node (word);
	  }
	((Node *) * slot) -> count ++;
      }
  }

  static int htab_trav (void ** slot, void * param)
  {
    Node * node = (Node *) * slot;
    vector<pair<int, string>> & wordList = * (vector<pair<int, string>> *) param;
    wordList . push_back (pair<int, string> (node -> count, node -> str));
    return 1;
  }

  void getWords (vector<pair<int, string>> & wordList)
  {
    wordList . reserve (htab_elements (m_hTab));
    htab_traverse_noresize (m_hTab, htab_trav, & wordList);
  }
};


BaseTester * CreateGCCHashTableTester (uint param)
{
  return new GCCHashTableTester (param);
}
