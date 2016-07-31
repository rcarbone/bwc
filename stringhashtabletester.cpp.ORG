#include "precomp.h"

class StringHashTable
{
public:
    struct Bucket
    {
        char *key;
        uint value;
        Bucket *next;

        Bucket() : key(), value(), next() {}
		void setKey(const char *k) { key = (char *) malloc(strlen(k) + 1); strcpy(key, k); }
		~Bucket() { free(key); }
    };

    uint m_tableSize;
    Bucket *m_table;
    uint m_blockSize;
    uint m_blockPos;
    uint m_numKeys;
    vector<Bucket *> m_blocks;

    StringHashTable(uint tableSize, uint blockSize = 256)
    {
        assert((tableSize & (tableSize - 1)) == 0);	// Must be a power of two.
        m_table = new Bucket[tableSize];
        m_tableSize = tableSize;
        m_blockPos = m_blockSize = blockSize;
        m_numKeys = 0;
    }

    ~StringHashTable()
    {
        delete[] m_table;
        for (uint i = 0; i < m_blocks.size(); i++)
        {
            delete[] m_blocks[i];
        }
    }

	static uint fnv1Hash(const char *key)
	{
		uint hash = 2166136261;
		for (const char *s = key; *s; s++)
	        hash = (16777619 * hash) ^ (*s);
		return hash;
	};

    uint &operator[](const char *key)
    {
        uint hash = fnv1Hash(key) & (m_tableSize - 1);
        Bucket *firstBucket = m_table + hash;
        Bucket *b = firstBucket;
		if (b->key)
		{
			do
			{
				if (strcmp(b->key, key) == 0)
					return b->value;	// Found existing bucket
				b = b->next;
			}
			while (b);
		}
        // Add it
        m_numKeys++;
        if (!firstBucket->key)
        {
            firstBucket->setKey(key);	// Use bucket in table
            return firstBucket->value;
        }
        // Allocate a new bucket
        if (m_blockPos >= m_blockSize)
        {
            Bucket *block = new Bucket[m_blockSize];
            m_blocks.push_back(block);
            m_blockPos = 0;
        }
        b = m_blocks.back() + m_blockPos;
        m_blockPos++;
        b->setKey(key);
        b->next = firstBucket->next;
        firstBucket->next = b;
        return b->value;
    }
};

class StringHashTableTester : public BaseTester
{
private:
	StringHashTable m_wordCount;

public:
	StringHashTableTester(uint tableSize) : m_wordCount(tableSize)
	{
	}

	void readWords(WordReader &reader)
	{
		while (const char *word = reader.getWord())
		{
			m_wordCount[word] += 1;
		}
	}

	void getWords(vector<pair<int, string> > &wordList)
	{
		typedef StringHashTable::Bucket Bucket;
		wordList.reserve(m_wordCount.m_numKeys);
		uint i;
		for (i = 0; i < m_wordCount.m_tableSize; i++)
		{
			Bucket *b = m_wordCount.m_table + i;
			if (b->key)
				wordList.push_back(pair<int, string>(b->value, b->key));
		}
		for (i = 0; i < m_wordCount.m_blocks.size(); i++)
		{
			Bucket *block = m_wordCount.m_blocks[i];
			for (uint j = 0; j < m_wordCount.m_blockSize; j++)
			{
				if (block[j].key)
					wordList.push_back(pair<int, string>(block[j].value, block[j].key));
			}
		}
	}
};

BaseTester *CreateStringHashTableTester(uint param)
{
	return new StringHashTableTester(param);
}
