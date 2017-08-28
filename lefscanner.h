#ifndef LEFSCANNER_H
#define LEFSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer lefFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include "lef.yy.h"

namespace lef {
	class LEFata;
	class LEFScanner : public lefFlexLexer
	{
	public:
		LEFScanner();
		int leflex(lef::LEFParser::semantic_type*, lef::LEFData*);
	};
}

int lefFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

#endif // LEFSCANNER_H
