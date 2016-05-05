#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <memory>
#include <map>

namespace WebPascal {
	namespace Lexical {

		enum class TokenClass
		{
			EndOfFile
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
