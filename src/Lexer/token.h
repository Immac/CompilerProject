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
			ReservedWith
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

				std::map<char,TokenClass> TypeMap =
				{
					std::make_pair<char,TokenClass>('\n',TokenClass::EndOfFile)
				};
		};

		using TokenRef = std::shared_ptr<Token>;
	}
}

#endif // TOKEN_H
