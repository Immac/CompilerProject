#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <memory>
#include <map>

namespace WebPascal {
	namespace Lexical {

		enum class TokenClass
		{
			EndOfFile,
			Id,
			ReservedAnd,
			ReservedDiv,
			ReservedFile,
			ReservedIn,
			ReservedOf,
			ReservedRecord,
			ReservedType,
			ReservedArray,
			ReservedDo,
			ReservedFor,
			ReservedLabel,
			ReservedOr,
			ReservedRepeat,
			ReservedUntil,
			ReservedBegin,
			ReservedDownTo,
			ReservedFunction,
			ReservedMod,
			ReservedPacked,
			ReservedSet,
			ReservedVar,
			ReservedCase,
			ReservedElse,
			ReservedGoTo,
			ReservedNil,
			ReservedProcedure,
			ReservedThen,
			ReservedWhile,
			ReservedConst,
			ReservedEnd,
			ReservedIf,
			ReservedNot,
			ReservedProgram,
			ReservedTo,
			ReservedWith,
			OperatorAssignOrCompare,
			OperatorSum,
			OperatorSubstract,
			OperatorMultiply,
			OperatorDifferent,
			OperatorGreaterThan,
			OperatorLessThan,
			OperatorGreaterOrEqualTo,
			OperatorLessThanOrEqualTo,
			OperatorTypeDeclaration,
			OperatorVariableAssign,
			PunctuationLeftParenthesis,
			PunctuationRightParenthesis,
			PunctuationEndOfSentence,
			PunctuationRangeInclusive,
			PunctuationRangeExclusive,
			IntegerLiteralDecimal,
			IntegerLiteralHexadecimal,
			IntegerLiteralOctal,
			StringLiteralDoubleQuote,
			StringLiteralSingleQuote,
			CommentOpenBlock,
			CommentCloseBlock,
			OperatorTypeAssign,
			OperatorEnumAssign,
			OperatorAccessor,
			HtmlOpenTag,
			HtmlCloseTag,
			HtmlContent,
			PascalCodeOpen,
			PascalCodeClose
		};

		class Token
		{
			public:
				Token(std::string lexeme,TokenClass type, int row, int column)
					: Lexeme(lexeme), Type(type), Row(row), Column(column) {}
				std::string Lexeme;
				TokenClass Type;
				int Row;
				int Column;
		};

		using TokenRef = std::shared_ptr<Token>;
	}
}

#endif // TOKEN_H
