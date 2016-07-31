#include "precomp.h"
#include <google/dense_hash_map>

using namespace google;

class GoogleDenseHashMapTester : public BaseTester
{
private:
	dense_hash_map<string, int> m_wordCount;

public:
	GoogleDenseHashMapTester(uint expectedCount) : m_wordCount(expectedCount)
	{
		m_wordCount.set_empty_key(string());
	}

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
		for (dense_hash_map<string, int>::iterator it = m_wordCount.begin(); it != m_wordCount.end(); it++)
		{
			wordList.push_back(pair<int, string>(it->second, it->first.c_str()));
		}
	}
};

BaseTester *CreateGoogleDenseHashMapTester(uint param)
{
	return new GoogleDenseHashMapTester(param);
}
