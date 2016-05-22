#ifndef SRC_WEBPASCALPARSER_H
#define SRC_WEBPASCALPARSER_H

#include "lexer.h"

namespace WebPascal{
	namespace Syntactic {
		class Parser {
		public:
			Parser(Lexical::LexerRef analyzer);
			void Parse();

		private:
			Lexical::LexerRef _analyzer;
			Lexical::TokenRef _currentToken;

			void Program();

			void ThrowSyntaxException(std::string what);

			void ConsumeToken();

			void BodyHtml();

			void BlockHtml();

			void BodyHtmlPrime();

			void StatementList();
		};

	}
}



#endif //SRC_WEBPASCALPARSER_H
