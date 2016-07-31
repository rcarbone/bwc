#include "precomp.h"
#include <hash_map>

class StdExtHashMapTester : public BaseTester
{
private:
	hash_map<string, int> m_wordCount;

public:
	StdExtHashMapTester() {}

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
		for (hash_map<string, int>::iterator it = m_wordCount.begin(); it != m_wordCount.end(); it++)
		{
			wordList.push_back(pair<int, string>(it->second, it->first.c_str()));
		}
	}
};

BaseTester *CreateStdExtHashMapTester(uint param)
{
	return new StdExtHashMapTester();
}
