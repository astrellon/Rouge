#ifndef _ANDROID
#	include "tokeniser.h"
#endif

namespace am {
namespace util {

Tokeniser::Tokeniser(const char *str) :
	ctok(str)
{
}
Tokeniser::~Tokeniser() {
}

const char *Tokeniser::nextToken() {
	if (ctok == NULL || 
		*ctok == '\0') {
		return NULL;
	}
		
	char ch = *ctok;
	while (IS_WHITE(ch)) {
		ch = *(++ctok);
	}
	char prevCh = '\0';
	bool inDQ = false;
	bool inSQ = false;
	bool inMLCmt = false;
	bool inSLCmt = false;
	int j = 0;
	bool escaped = false;
	bool done = false;
	bool first = true;

	while(ch != '\0' && !done) {
		ch = *ctok;

		if (prevCh == '/') {
			if (ch == '/') {
				inSLCmt = true;
			}
			else if(ch == '*') {
				inMLCmt = true;
			}
		}
			
		if (inSLCmt && ch == '\n') {
			inSLCmt = false;
			break;
		} else if (inMLCmt && prevCh == '*' && ch == '/') {
			inMLCmt = false;
			ctok++;
			tokBuff[j++] = '/';
			break;
		}
			
		if (!inSLCmt && !inMLCmt) {
			if (!inDQ && ch == '"') {
				inDQ = true;
				tokBuff[j++] = '"';
				ctok++;
				continue;
			} else if (!inSQ && ch == '\'') {
				inSQ = true;
				tokBuff[j++] = '\'';
				ctok++;
				continue;
			} else if (!inDQ && !inSQ) {
				if (IS_WHITE(ch)) {
					break;
				}
				if (ch == '[' || ch == ']' ||
					ch == '{' || ch == '}' ||
					ch == ':' || ch == ',') {
					if (first) {
						done = true;
					}
					else {
						break;
					}
				}
			}
			if ((inDQ || inSQ) && ch == '\\') {
				escaped = true;
				continue;
			}
			
			if (escaped) {
				switch (ch) {
					default:	break;
					case 'n':	ch = '\n';	break;
					case 'r':	ch = '\r';	break;
					case 't':	ch = '\t';	break;
					case '0':	ch = '\0';	break;
				}
			}
			
			if (inDQ && !escaped && ch == '"') {
				ctok++;
				break;
			} else if (inSQ && !escaped && ch == '\'') {
				ctok++;
				break;
			}
		}
		escaped = false;
		first = false;
			
		prevCh = ch;
		tokBuff[j++] = ch;
		ch = *ctok++;
		}
		tokBuff[j] = '\0';
#ifdef DEBUG
		printf("Found token len %d >%s<\n", j, tokBuff);
#endif
		return tokBuff; 
}

}
}
