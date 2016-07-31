#pragma once

class WordReader
{
 private:

  const char * m_rdPos;
  const char * m_endPos;

  static const int MaxWordLen = 255;
  char m_word [MaxWordLen + 1];

 public:

  WordReader (vector<char> & buffer)
    {
      m_rdPos = & buffer [0];
      m_endPos = m_rdPos + buffer . size ();
    }

  const char * getWord ()
    {
      int wordLen = 0;

      while (m_rdPos < m_endPos)
	{
	  char c = * m_rdPos ++;

	  int lower = c | 0x20;	// lowercase
	  if (lower >= 'a' ? lower <= 'z' : (lower >= '0' && lower <= '9'))
	    {
	      if (wordLen < MaxWordLen)
		m_word [wordLen ++] = c;
	    }
	  else
	    {
	      if (wordLen > 0)
		{
		  m_word [wordLen] = 0;
		  return m_word;
		}
	    }
	}
      if (wordLen > 0)
	{
	  m_word [wordLen] = 0;
	  return m_word;
	}
      return NULL;
    }
};
