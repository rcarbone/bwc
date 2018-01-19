#pragma once

#include <assert.h>
#include <string>
#include <vector>

using namespace std;

#include "wordreader.h"


// Additional config
#define VALIDATE_TESTS		0
#define PRINT_STRINGS		0
#define	NUMBER_OF_ITERATIONS	5

#ifndef _DEBUG
#define _HAS_EXCEPTIONS 0
#endif
#define _HAS_ITERATOR_DEBUGGING 0	// Doesn't affect Release...
#define _SECURE_SCL 0

typedef unsigned int uint;

#if USE_DLMALLOC
extern "C"
{
  void * dlmalloc (size_t);
  void * dlcalloc (size_t, size_t);
  void dlfree (void *);
}

#define malloc dlmalloc
#define calloc dlcalloc
#define free   dlfree

inline void * operator new (size_t cb)    { return dlmalloc (cb); }
inline void * operator new [] (size_t cb) { return dlmalloc (cb); }
inline void operator delete (void * p)    { dlfree (p); }
inline void operator delete [] (void * p) { dlfree (p); }

#endif /* USE_DLMALLOC */


class BaseTester
{
 public:
  virtual ~BaseTester () {}
  virtual void readWords (WordReader & reader) = 0;
  virtual void getWords (vector <pair <int, string> > & wordList) = 0;
};
