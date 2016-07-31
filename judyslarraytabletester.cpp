#include "precomp.h"
#include <Judy.h>

class JudySLArrayTester : public BaseTester
{
private:
	Pvoid_t m_PJArray;

public:
	JudySLArrayTester()
	{
		m_PJArray = NULL;
	}

	~JudySLArrayTester()
	{
		int bytes;
		JSLFA(bytes, m_PJArray);
	}

	void readWords(WordReader &reader)
	{
		while (const char *word = reader.getWord())
		{
			Pvoid_t pValue;
			JSLI(pValue, m_PJArray, (const uint8_t *) word);
			*(int *) pValue += 1;
		}
	}

	void getWords(vector<pair<int, string> > &wordList)
	{
		uint8_t Index[1024] = "";			// unsafe, but screw it
		Pvoid_t PValue;
		JSLF(PValue, m_PJArray, Index);       // get first string
		while (PValue != NULL)
		{
			wordList.push_back(pair<int, string>(*(int *) PValue, (char *) Index));
			JSLN(PValue, m_PJArray, Index);   // get next string
		}
	}
};

BaseTester *CreateJudySLArrayTester(uint param)
{
	return new JudySLArrayTester;
}
