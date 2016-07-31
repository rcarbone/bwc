#include "precomp.h"

template <class Key, class Value, class Traits> class OpenHashTable
{
public:
    struct Slot
    {
		uint hash;
        Key key;
        Value value;
        Slot() : hash(), key(), value() {}
    };

    uint m_numSlots;
    uint m_numKeys;
	uint m_limitKeys;
    Slot *m_slots;
	float m_threshold;

    OpenHashTable(uint numSlots, float threshold)
    {
        assert((numSlots & (numSlots - 1)) == 0);	// Must be a power of two.
        m_numSlots = numSlots;
        m_numKeys = 0;
		m_limitKeys = (uint) (m_numSlots * threshold);
        m_slots = new Slot[m_numSlots];
		m_threshold = threshold;
    }

    ~OpenHashTable()
    {
		delete[] m_slots;
    }

    Value &operator[](const Key &key)
    {
		uint hash = Traits::hash(key);
		for (;;)
		{
			uint mask = m_numSlots - 1;
	        uint index = hash & mask;
			Slot *slot;
			for (;;)
			{
				slot = m_slots + index;
				if (slot->hash == hash && slot->key == key)
					return slot->value;
				if (Traits::isEmpty(slot->key))
					break;
				index = (index + 1) & mask;
			}
			slot->hash = hash;
			slot->key = key;
			m_numKeys++;
			if (m_numKeys < m_limitKeys)
				return slot->value;
			Resize(m_numSlots * 2);
			// Now that everything has relocated, loop, find and return the new slot
		}
	}

	void Resize(uint newSize)
	{
		uint newMask = newSize - 1;
		assert((newSize & newMask) == 0);	// must be power of 2
		Slot *newSlots = new Slot[newSize];
		Slot *oldSlot = m_slots;
		for (uint i = 0; i < m_numSlots; i++, oldSlot++)
		{
			uint newIndex = oldSlot->hash & newMask;
			Slot *newSlot;
			for (;;)
			{
				newSlot = newSlots + newIndex;
				if (Traits::isEmpty(newSlot->key))
					break;
				newIndex = (newIndex + 1) & newMask;
			}
			*newSlot = *oldSlot;
		}
		m_numSlots = newSize;
		delete[] m_slots;
		m_slots = newSlots;
		m_limitKeys = (uint) (newSize * m_threshold);
		assert(m_numKeys < m_limitKeys);
	}
};

class OpenHashTableTester : public BaseTester
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

		static bool isEmpty(const string &key)
		{
			return key.empty();
		}
	};

	OpenHashTable<string, int, Traits> m_wordCount;

public:
	OpenHashTableTester(float threshold) : m_wordCount(65536, threshold)
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
		wordList.reserve(m_wordCount.m_numKeys);
		OpenHashTable<string, int, Traits>::Slot *slot = m_wordCount.m_slots;
		for (uint i = 0; i < m_wordCount.m_numSlots; i++, slot++)
		{
			if (!slot->key.empty())
				wordList.push_back(pair<int, string>(slot->value, slot->key.c_str()));
		}
	}
};

BaseTester *CreateOpenHashTableTester(uint param)		// Param is % threshold before resizing
{
	return new OpenHashTableTester(param / 100.f);
}
