#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <unordered_map>
#include "sourcestream.h"
#include "token.h"

namespace WebPascal {
	namespace Lexical {

		class Lexer
		{
			public:
				Lexer(std::string content);
				SourceStream SourceCodeStream;
				TokenRef GetNextToken();

				enum class LexicalState
				{
					Initial,
					Id,
					IntegerLiteralDecimal,
					IntegerLiteralHexadecimal,
					IntegerLiteralOctal,
					StringLiteralSingleQuote,
					StringLiteralDoubleQuote,
					EndOfFile
				};

				std::map<std::string,TokenClass> ReservedWords
				{
					{"and",TokenClass::ReservedAnd},
					{"div",TokenClass::ReservedDiv},
					{"file",TokenClass::ReservedFile},
					{"in",TokenClass::ReservedIn},
					{"of",TokenClass::ReservedOf},
					{"record",TokenClass::ReservedRecord},
					{"type",TokenClass::ReservedType},
					{"array",TokenClass::ReservedArray},
					{"do",TokenClass::ReservedDo},
					{"for",TokenClass::ReservedFor},
					{"label",TokenClass::ReservedLabel},
					{"or",TokenClass::ReservedOr},
					{"repeat",TokenClass::ReservedRepeat},
					{"until",TokenClass::ReservedUntil},
					{"begin",TokenClass::ReservedBegin},
					{"downto",TokenClass::ReservedDownTo},
					{"function",TokenClass::ReservedFunction},
					{"mod",TokenClass::ReservedMod},
					{"packed",TokenClass::ReservedPacked},
					{"set",TokenClass::ReservedSet},
					{"var",TokenClass::ReservedVar},
					{"case",TokenClass::ReservedCase},
					{"else",TokenClass::ReservedElse},
					{"goto",TokenClass::ReservedGoTo},
					{"nil",TokenClass::ReservedNil},
					{"procedure",TokenClass::ReservedProcedure},
					{"then",TokenClass::ReservedThen},
					{"while",TokenClass::ReservedWhile},
					{"const",TokenClass::ReservedConst},
					{"end",TokenClass::ReservedEnd},
					{"if",TokenClass::ReservedIf},
					{"not",TokenClass::ReservedNot},
					{"program",TokenClass::ReservedProgram},
					{"to",TokenClass::ReservedTo},
					{"with",TokenClass::ReservedWith},
				};

			private:
				Symbol _currentSymbol;
				int _row = 0;
				int _column = 0;
				void UpdatePosition();
				void ConsumeSymbol();
		};

		using LexerRef = std::shared_ptr<Lexer>;
	}
}

#endif // LEXER_H
