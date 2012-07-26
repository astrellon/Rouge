#include "text_tokeniser.h"

#include "utils.h"

namespace am {
namespace util {

	TextTokeniser::TextTokeniser(const char *str) :
		ctok(str),
		mInFormat(0)
	{
	}
	TextTokeniser::~TextTokeniser() {
	}

	const char *TextTokeniser::currentToken() const 
	{
		if (ctok == NULL || ctok[0] == '\0') 
		{
			return NULL;
		}
		return tokBuff;
	}

	const char *TextTokeniser::nextToken() {
		if (ctok == NULL || ctok[0] == '\0') 
		{
			return NULL;
		}

		char ch = *ctok++;
		int j = 0;
		if (mInFormat == 0)
		{
			while (ch != '<' && ch != '\0')
			{
				tokBuff[j++] = ch;
				ch = *ctok++;
			}
			if (ch == '\0')
			{
				ctok--;
			}
			else if (ch == '<')
			{
				mInFormat = 1;
			}
		}
		else if (mInFormat == 1)
		{
			tokBuff[j++] = '<';
			ctok--;
			mInFormat = 2;
		}
		else if (mInFormat == 2)
		{
			while (IS_WHITE(ch))
			{
				ch = *ctok++;
			}
			if (ch == '/')
			{
				tokBuff[j++] = ch;
			}
			else
			{
				while (ch != '>' && ch != '\0' && !(IS_WHITE(ch)) && 
					   ch != '=' && ch != '\'' && ch != '"')
				{
					tokBuff[j++] = ch;
					ch = *ctok++;
				}
				
				if (ch == '\0')
				{
					ctok--;
				}
				else if (ch == '\'' || ch == '"')
				{
					ch = *ctok++;
					while (ch != '\0' && ch != '\'' && ch != '"')
					{
						tokBuff[j++] = ch;
						ch = *ctok++;
					}
				}
				else if (ch == '>')
				{
					mInFormat = 3;
				}
				else if (ch == '=')
				{
					mInFormat = 4;
					ctok--;
				}
			}
		}
		else if (mInFormat == 4)
		{
			tokBuff[j++] = '=';
			//ctok--;
			mInFormat = 2;
		}
		else
		{
			tokBuff[j++] = '>';
			ctok--;
			mInFormat = 0;
		}
		tokBuff[j++] = '\0';

		if (tokBuff[0] == '\0')
		{
			return nextToken();
		}
		return tokBuff;
	}

}
}
