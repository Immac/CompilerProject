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
			_tokenList.push_back(*currentToken);
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
		std::string sourceCode = "";
		LexicalAnalysisSteps steps;
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"$",row,column);
			}
		}
	}

	GIVEN( "a single whitespace" )
	{
		std::string sourceCode = " ";
		LexicalAnalysisSteps steps;
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 1;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"$",row,column);
			}
		}
	}

	GIVEN( "a reserved word" )
	{
		std::string sourceCode = "array";
		LexicalAnalysisSteps steps;
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::ReservedArray,"array",row,column);
				index = 1; row = 0; column = 5;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"$",row,column);
			}
		}
	}

	GIVEN( "an id" )
	{
		std::string sourceCode = "kanako";
		LexicalAnalysisSteps steps;
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::Id,"kanako",row,column);
				index = 1; row = 0; column = 6;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"$",row,column);
			}
		}
	}
}
