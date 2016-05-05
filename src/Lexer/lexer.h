#ifndef LEXER_H
#define LEXER_H
#include <string>
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

			private:
				Symbol _currentSymbol;
				LexicalState _state = LexicalState::Initial;
				int _row = 0;
				int _column = 0;
				void UpdatePosition();
		};

		using LexerRef = std::shared_ptr<Lexer>;
	}
}

#endif // LEXER_H
