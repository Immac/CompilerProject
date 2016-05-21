#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include <algorithm>
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
					IntegerLiteralHexadecimalSingle,
					IntegerLiteralHexadecimalPair,
					IntegerLiteralHexadecimalImpair,
					IntegerLiteralOctal,
					StringLiteralSingleQuote,
					StringLiteralDoubleQuote,
					StringLiteralClosingDoubleQuote,
					StringLiteralClosingSingleQuote,
					StringLiteralEscapedDoubleQuote,
					StringLiteralEscapedSingleQuote,
					UnanbiguosPunctuation,
					AmbiguousPunctuation,
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

				std::map<std::string,TokenClass> Punctuation
				{
					{"(",TokenClass::PunctuationLeftParenthesis},
					{")",TokenClass::PunctuationRightParenthesis},
					{"(*",TokenClass::CommentOpenBlock},
					{"*)",TokenClass::CommentCloseBlock},
					{":",TokenClass::OperatorTypeAssign},
					{":=",TokenClass::OperatorEnumAssign},
					{"=",TokenClass::OperatorAssignOrCompare},
					{".",TokenClass::OperatorAccessor},
					{"..",TokenClass::PunctuationRangeExclusive},
					{"+",TokenClass::OperatorSum},
					{"-",TokenClass::OperatorSubstract},
					{"*",TokenClass::OperatorMultiply},
					{"<>",TokenClass::OperatorDifferent},
					{">",TokenClass::OperatorGreaterThan},
					{"<",TokenClass::OperatorLessThan},
					{">=",TokenClass::OperatorGreaterOrEqualTo},
					{"<=",TokenClass::OperatorLessThanOrEqualTo},
				};

				std::vector<char> UnambiguousPunctuation //TODO: deduce this from Punctuation
				{
					'=','+','-',')'
				};

				std::vector<char> AmbiguosPunctionStartSymbols //TODO: deduce this from Punctuation
				{
					'(','*',':','=','.','<','>'
				};



				std::vector<char> ValidHexValues
				{
					'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','B','C','D','E','F'
				}; // TODO: make case insensitive, have it accept impair number of hex digits

				std::vector<char> ValidOctValues
				{
					'0','1','2','3','4','5','6','7','8'
				};
			private:
				Symbol _currentSymbol;
				int _row = 0;
				int _column = 0;
				void UpdatePosition();
				void ConsumeSymbol();
				void ThrowLexicalError(std::string what); //TODO: Make a class instead of a function

				template <typename T> bool Contains(std::vector<T> v, T what ) //TODO: Make a class instead of a function
				{
					return std::find(v.begin(),v.end(),what) != v.end();
				}
		};

		using LexerRef = std::shared_ptr<Lexer>;
	}
}

#endif // LEXER_H
