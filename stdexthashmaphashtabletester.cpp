#include "precomp.h"
#include <hash_map>

template <class Key, class Value, class Traits> class StdExtHashMapHashTable
{
public:
	struct Bucket
	{
		Key key;
		Value value;
		Bucket *next;

		Bucket() : key(), value(), next(NULL) {}
	};

	hash_map<uint, Bucket *> m_table;
	uint m_blockSize;
	uint m_blockPos;
	uint m_numKeys;
	vector<Bucket *> m_blocks;

	StdExtHashMapHashTable(uint blockSize = 256)
	{
		m_blockPos = m_blockSize = blockSize;
		m_numKeys = 0;
	}

	~StdExtHashMapHashTable()
	{
		for (uint i = 0; i < m_blocks.size(); i++)
		{
			delete[] m_blocks[i];
		}
	}

	Value &operator[](const Key &key)
	{
		uint hash = Traits::hash(key);
		Bucket **ppFirstBucket = &m_table[hash];
		Bucket *b = *ppFirstBucket;
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
		b->next = *ppFirstBucket;
		*ppFirstBucket = b;
		return b->value;
	}
};

class StdExtHashMapHashTableTester : public BaseTester
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

	StdExtHashMapHashTable<string, int, Traits> m_wordCount;

public:
	StdExtHashMapHashTableTester() {}

	void readWords(WordReader &reader)
	{
		while (const char *word = reader.getWord())
		{
			m_wordCount[word] += 1;
		}
	}

	void getWords(vector<pair<int, string> > &wordList)
	{
		typedef StdExtHashMapHashTable<string, int, Traits>::Bucket Bucket;
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

BaseTester *CreateStdExtHashMapHashTableTester(uint param)
{
	return new StdExtHashMapHashTableTester;
}
