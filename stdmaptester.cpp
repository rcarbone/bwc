#include "precomp.h"
#include <map>

class StdMapTester : public BaseTester
{
private:
	map<string, int> m_wordCount;

public:
	void readWords(WordReader &reader)
	{
		while (const char *word = reader.getWord())
		{
			m_wordCount[word] += 1;	// New entries are autoinitialized to 0
		}
	}

	void getWords(vector<pair<int, string > > &wordList)
	{
		wordList.reserve(m_wordCount.size());
		for (map<string, int>::iterator it = m_wordCount.begin(); it != m_wordCount.end(); it++)
		{
			wordList.push_back(pair<int, string>(it->second, it->first.c_str()));
		}
	}
};

BaseTester *CreateStdMapTester(uint param)
{
	return new StdMapTester;
}
