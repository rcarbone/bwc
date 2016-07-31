#include "precomp.h"
#include <google/sparse_hash_map>

using namespace google;

class GoogleSparseHashMapTester : public BaseTester
{
private:
	sparse_hash_map<string, int> m_wordCount;

public:
	GoogleSparseHashMapTester(uint expectedCount) : m_wordCount(expectedCount) {}

	void readWords(WordReader &reader)
	{
		while (const char *word = reader.getWord())
		{
			m_wordCount[word] += 1;	// New entries are autoinitialized to 0
		}
	}

	void getWords(vector<pair<int, string> > &wordList)
	{
		wordList.reserve(m_wordCount.size());
		for (sparse_hash_map<string, int>::iterator it = m_wordCount.begin(); it != m_wordCount.end(); it++)
		{
			wordList.push_back(pair<int, string>(it->second, it->first.c_str()));
		}
	}
};

BaseTester *CreateGoogleSparseHashMapTester(uint param)
{
	return new GoogleSparseHashMapTester(param);
}
