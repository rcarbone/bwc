#include "precomp.h"
#include <Judy.h>

template <class Key, class Value, class Traits> class JudyLHashTable
{
public:
	struct Bucket
	{
		Key key;
		Value value;
		Bucket *next;

		Bucket() : key(), value(), next(NULL) {}
	};

	Pvoid_t m_PJLArray;
	uint m_blockSize;
	uint m_blockPos;
	uint m_numKeys;
	vector<Bucket *> m_blocks;

	JudyLHashTable(uint blockSize = 256)
	{
		m_PJLArray = NULL;
		m_blockPos = m_blockSize = blockSize;
		m_numKeys = 0;
	}

	~JudyLHashTable()
	{
		for (uint i = 0; i < m_blocks.size(); i++)
		{
			delete[] m_blocks[i];
		}
		Word_t rc;
		JLFA(rc, m_PJLArray);
	}

	Value &operator[](const Key &key)
	{
		uint hash = Traits::hash(key);
		Pvoid_t ppFirstBucket;
		JLI(ppFirstBucket, m_PJLArray, hash);
		Bucket *pFirstBucket = *(Bucket**) ppFirstBucket;
		Bucket *b = pFirstBucket;
		while (b)
		{
			if (b->key == key)
				return b->value;	// Found existing bucket
			b = b->next;
		}

		// Add it
		m_numKeys++;
		// Allocate a new bucket
		if (m_blockPos >= m_blockSize)
		{
			Bucket *block = new Bucket[m_blockSize];
			m_blocks.push_back(block);
			m_blockPos = 0;
		}
		b = m_blocks.back() + m_blockPos;
		m_blockPos++;
		b->key = key;
		b->next = pFirstBucket;
		*(Bucket**) ppFirstBucket = b;
		return b->value;
	}
};

class JudyLHashTableTester : public BaseTester
{
private:
	class Traits
	{
	public:
		static uint hash(const string &key)
		{
			uint hash = 2166136261;
			for (const char *s = key.c_str(); *s; s++)
		        hash = (16777619 * hash) ^ (*s);
			return hash;
		};
	};

	JudyLHashTable<string, int, Traits> m_wordCount;

public:
	JudyLHashTableTester() {}

	void readWords(WordReader &reader)
	{
		while (const char *word = reader.getWord())
		{
			m_wordCount[word] += 1;
		}
	}

	void getWords(vector<pair<int, string> > &wordList)
	{
		typedef JudyLHashTable<string, int, Traits>::Bucket Bucket;
		wordList.reserve(m_wordCount.m_numKeys);
		for (uint i = 0; i < m_wordCount.m_blocks.size(); i++)
		{
			Bucket *block = m_wordCount.m_blocks[i];
			for (uint j = 0; j < m_wordCount.m_blockSize; j++)
			{
				if (!block[j].key.empty())
					wordList.push_back(pair<int, string>(block[j].value, block[j].key.c_str()));
			}
		}
	}
};

BaseTester *CreateJudyLHashTableTester(uint param)
{
	return new JudyLHashTableTester;
}
