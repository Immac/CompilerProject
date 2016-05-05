#include <iostream>
#include <memory>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <vector>

#include "src/Lexer/lexer.h"
#include "src/Lexer/sourcestream.h"

using namespace WebPascal::Lexical;

class LexicalAnalysisSteps
{
	public:

		void PrepareContext(std::string content)
		{
			this->_lexer = std::make_shared<Lexer>(content);
		}

		void Tokenize()
		{
			TokenRef currentToken = _lexer->GetNextToken();

			do {
				_tokenList.push_back(*currentToken);
				currentToken = _lexer->GetNextToken();
			} while (currentToken->Type != TokenClass::EndOfFile);
		}

		std::vector<Token> GetTokenList() { return this->_tokenList;}

		void AssertTokenValidity(int index, TokenClass type, std::string lexeme, int row, int column)
		{
			auto token = this->_tokenList.at(index);
			REQUIRE(token.Type == type);
			REQUIRE(token.Lexeme == lexeme);
			REQUIRE(token.Row == row);
			REQUIRE(token.Column == column);
		}

	private:
		LexerRef _lexer;
		std::vector<Token> _tokenList;
};



SCENARIO( "Lexical Analisys", "[vector]" )
{

	GIVEN( "An empty source code" )
	{
		Steps steps;
		steps.PrepareContext("");
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				steps.AssertTokenValidity(0,TokenClass::EndOfFile,"$",0,0);
			}
		}
	}

	GIVEN( "An empty source code" )
	{
		Steps steps;
		steps.PrepareContext("");
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				steps.AssertTokenValidity(0,TokenClass::EndOfFile,"$",0,0);
			}
		}
	}
}
