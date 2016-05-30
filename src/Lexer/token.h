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
			OperatorSubtract,
			OperatorMultiply,
			OperatorDifferentFrom,
			OperatorGreaterThan,
			OperatorLessThan,
			OperatorGreaterOrEqualTo,
			OperatorLessThanOrEqualTo,
			PunctuationLeftParenthesis,
			PunctuationRightParenthesis,
			PunctuationRangeExclusive,
			IntegerLiteralDecimal,
			IntegerLiteralHexadecimal,
			IntegerLiteralOctal,
			StringLiteralDoubleQuote,
			StringLiteralSingleQuote,
			CommentOpenBlock,
			CommentCloseBlock,
			OperatorTypeAssign,
			OperatorAssign,
			OperatorAccessor,
			HtmlOpenTag,
			HtmlCloseTag,
			HtmlContent,
			EndOfStatement,
			ReservedContinue,
			ReservedBreak,
			PunctuationComma,
			PunctuationOpenSquareBracket,
			PunctuationCloseSquareBracket,
			BooleanLiteralTrue,
			BooleanLiteralFalse,
			OperatorDivide,
			CharLiteral,
			RealLiteral
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

			static std::array<std::string,73> TokenClassNames {{
																	   "EndOfFile",
																	   "Id",
																	   "ReservedAnd",
																	   "ReservedDiv",
																	   "ReservedFile",
																	   "ReservedIn",
																	   "ReservedOf",
																	   "ReservedRecord",
																	   "ReservedType",
																	   "ReservedArray",
																	   "ReservedDo",
																	   "ReservedFor",
																	   "ReservedLabel",
																	   "ReservedOr",
																	   "ReservedRepeat",
																	   "ReservedUntil",
																	   "ReservedBegin",
																	   "ReservedDownTo",
																	   "ReservedFunction",
																	   "ReservedMod",
																	   "ReservedPacked",
																	   "ReservedSet",
																	   "ReservedVar",
																	   "ReservedCase",
																	   "ReservedElse",
																	   "ReservedGoTo",
																	   "ReservedNil",
																	   "ReservedProcedure",
																	   "ReservedThen",
																	   "ReservedWhile",
																	   "ReservedConst",
																	   "ReservedEnd",
																	   "ReservedIf",
																	   "ReservedNot",
																	   "ReservedProgram",
																	   "ReservedTo",
																	   "ReservedWith",
																	   "OperatorAssignOrCompare",
																	   "OperatorSum",
																	   "OperatorSubtract",
																	   "OperatorMultiply",
																	   "OperatorDifferentFrom",
																	   "OperatorGreaterThan",
																	   "OperatorLessThan",
																	   "OperatorGreaterOrEqualTo",
																	   "OperatorLessThanOrEqualTo",
																	   "PunctuationLeftParenthesis",
																	   "PunctuationRightParenthesis",
																	   "PunctuationRangeExclusive",
																	   "IntegerLiteralDecimal",
																	   "IntegerLiteralHexadecimal",
																	   "IntegerLiteralOctal",
																	   "StringLiteralDoubleQuote",
																	   "StringLiteralSingleQuote",
																	   "CommentOpenBlock",
																	   "CommentCloseBlock",
																	   "OperatorTypeAssign",
																	   "OperatorAssign",
																	   "OperatorAccessor",
																	   "HtmlOpenTag",
																	   "HtmlCloseTag",
																	   "HtmlContent",
																	   "EndOfStatement",
																	   "ReservedContinue",
																	   "ReservedBreak",
																	   "PunctuationComma",
																	   "PunctuationOpenSquareBracket",
																	   "PunctuationCloseSquareBracket",
																	   "BooleanLiteralTrue",
																	   "BooleanLiteralFalse",
																	   "OperatorDivide",
																	   "CharLiteral",
																	   "RealLiteral"
															   }};

	}
}

#endif // TOKEN_H
