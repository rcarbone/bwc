#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>

#include "precomp.h"

#include <python2.7/Python.h>


struct TestEntry
{
  BaseTester * (* factoryFunc) (uint param);
  uint param;
  const char * name;
  double sumOfTimes;
  double sumOfSquareTimes;
};


/* Forward references */
BaseTester * CreateStdMapTester (uint param);
BaseTester * CreateStdExtHashMapTester (uint param);

BaseTester * CreateStdMapHashTableTester (uint param);
BaseTester * CreateStdExtHashMapHashTableTester (uint param);

BaseTester * CreateGoogleDenseHashMapTester (uint param);
BaseTester * CreateGoogleSparseHashMapTester (uint param);

BaseTester * CreateGCCHashTableTester (uint param);

BaseTester * CreateOpenHashTableTester (uint param);		// Param is % threshold before resizing
BaseTester * CreateChainedHashTableTester (uint param);
BaseTester * CreateStringHashTableTester (uint param);

BaseTester * CreatePythonDictionaryTester (uint param);

BaseTester * CreateJudyLHashTableTester (uint param);
BaseTester * CreateJudySLArrayTester (uint param);


/* The table of implementations under test */
TestEntry TestEntries [] =
{
  { CreateStdMapTester,                 0,      "std::map",                    0, 0 },
  { CreateStdMapHashTableTester,        0,      "std::map-hash-table",         0, 0 },

#if defined(ROCCO)
  { CreateStdExtHashMapTester,          0,      "stdext::hash_map",            0, 0 },
  { CreateStdExtHashMapHashTableTester, 0,      "stdext::hash_map-hash-table", 0, 0 },
#endif /* ROCCO */

  /* Google */
  { CreateGoogleDenseHashMapTester,     32000,  "Google-dense-hash",           0, 0 },
  { CreateGoogleSparseHashMapTester,    32000,  "Google-sparse-hash",          0, 0 },

  /* gcc */
  { CreateGCCHashTableTester,           0,      "GCC-hash-table",              0, 0 },

#if defined(ROCCO)
  { CreateOpenHashTableTester,          33,     "OpenHashTable 33%",           0, 0 },
  { CreateOpenHashTableTester,          50,     "OpenHashTable 50%",           0, 0 },
#endif /* ROCCO */
  { CreateOpenHashTableTester,          66,     "OpenHashTable",               0, 0 },

#if defined(ROCCO)
  { CreateChainedHashTableTester,       4096,   "ChainedHashTable 4096",       0, 0 },
  { CreateChainedHashTableTester,       8192,   "ChainedHashTable 8192",       0, 0 },
  { CreateChainedHashTableTester,       16384,  "ChainedHashTable 16384",      0, 0 },
  { CreateChainedHashTableTester,       32768,  "ChainedHashTable 32768",      0, 0 },
#endif /* ROCCO */

  { CreateChainedHashTableTester,       65536,  "ChainedHashTable",            0, 0 },

#if defined(ROCCO)
  { CreateChainedHashTableTester,       131072, "ChainedHashTable 131072",     0, 0 },
  { CreateChainedHashTableTester,       262144, "ChainedHashTable 262144",     0, 0 },
#endif /* ROCCO */

  { CreateStringHashTableTester,        65536,  "StringHashTable", 0, 0 },

  /* Python */
  { CreatePythonDictionaryTester,       0,      "Python-dictionary",           0, 0 },

  /* Judy */
  { CreateJudyLHashTableTester,         0,      "JudyL-hash-table",            0, 0 },
  { CreateJudySLArrayTester,            0,      "JudySL-Array",                0, 0 },
};


bool ReadWholeFile (const char * filename, vector<char> &buffer)
{
  ifstream inFile (filename);
  if (inFile . fail ())
    return false;

  inFile . seekg (0, ios::end);
  uint fileSize = inFile . tellg ();
  inFile . seekg (0, ios::beg);

  buffer . resize (fileSize);
  inFile . read (& buffer [0], fileSize);
  return true;
}


int main (int argc, const char * argv [])
{
  vector<char> buffer;

  if (argc < 2)
    {
      puts ("Please specify the input filename on the command line.");
      return 1;
    }

  if (! ReadWholeFile (argv [1], buffer))
    {
      printf ("Could not open '%s'.\n", argv [1]);
      return 1;
    }

  /* Set unbuffered stdout */
  setvbuf (stdout, NULL, _IONBF, 0);

  Py_Initialize ();		/* For the Python tests */

  for (int runs = 0; runs < NUMBER_OF_ITERATIONS; runs ++)
    {
#if VALIDATE_TESTS
      vector<pair<int, string> > baseWordList;
#endif

      printf ("Run #%d of %d...\n", runs + 1, NUMBER_OF_ITERATIONS);

      for (uint i = 0; i < sizeof (TestEntries) / sizeof (TestEntries [0]); i ++)
	{
	  TestEntry * entry = TestEntries + i;
	  WordReader reader (buffer);

	  BaseTester * tester = entry -> factoryFunc (entry -> param);

	  struct timespec start, stop, freq;

	  if (clock_gettime (CLOCK_MONOTONIC, & start))
	    abort ();

	  tester -> readWords (reader);

	  if (clock_gettime (CLOCK_MONOTONIC, & stop))
	    abort ();

	  double timeSpent = (double) ((stop . tv_sec * (uint64_t) 1000000000 + stop . tv_nsec) -
				       (start . tv_sec * (uint64_t) 1000000000 + start . tv_nsec));

	  printf ("Completed \"%-20s\" in %7.3f ms\n", entry -> name, timeSpent / 1e6);
	  entry -> sumOfTimes += timeSpent;
	  entry -> sumOfSquareTimes += timeSpent * timeSpent;

#if PRINT_STRINGS
	  {
	    vector<pair<int, string> > wordList;
	    tester -> getWords (wordList);
	    sort (wordList . begin (), wordList . end ());
	    for (uint i = 0; i < 5 && i < wordList.size (); i ++)
	      {
		const pair<int, string> & word = wordList [wordList . size () - 1 - i];
		printf ("%s: %d\n", word . second . c_str(), word . first);
	      }
	  }
#endif

#if VALIDATE_TESTS
	  {
	    vector<pair<int, string> > wordList;
	    tester -> getWords (wordList);
	    sort (wordList . begin (), wordList . end ());
	    if (baseWordList . size () == 0)
	      baseWordList = wordList;
	    else
	      assert (baseWordList == wordList);
	  }
#endif

	  delete tester;
	}
    }

  printf ("\n");
  printf ("Average of %d runs:\n", NUMBER_OF_ITERATIONS);
  printf ("%-20s avg(ms)  stddev(ms)\n", "name");
  for (uint i = 0; i < sizeof (TestEntries) / sizeof (TestEntries [0]); i ++)
    {
      TestEntry * entry = TestEntries + i;
      double average = entry -> sumOfTimes / NUMBER_OF_ITERATIONS;
      double stdDev = sqrt (entry -> sumOfSquareTimes / NUMBER_OF_ITERATIONS - average * average);
      printf ("%-20s %7.3f   %14.2f\n", entry -> name, average / 1e6, stdDev * 1000);
    }

  Py_Finalize ();		/* For the Python tests */

  return 0;
}
