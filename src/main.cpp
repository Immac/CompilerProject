#include <iostream>
#include <memory>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include "Lexer/lexer.h"

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

// TODO: rewrite scenarios taking in account each scenario creates a new "steps"
SCENARIO( "Lexical Analisys", "[vector]" )
{
	LexicalAnalysisSteps steps;
	GIVEN( "An empty source code" )
	{
		std::string sourceCode = "";

		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a single whitespace" )
	{
		std::string sourceCode = " ";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 1;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a reserved word" )
	{
		std::string sourceCode = "array";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::ReservedArray,"array",row,column);
				index = 1; row = 0; column = 5;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a reserved word in different case" )
	{
		std::string sourceCode = "ArRay";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::ReservedArray,"array",row,column);
				index = 1; row = 0; column = 5;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "an id" )
	{
		std::string sourceCode = "kanako";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::Id,"kanako",row,column);
				index = 1; row = 0; column = 6;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a decimal integer literal" )
	{
		std::string sourceCode = "12";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::IntegerLiteralDecimal,"12",row,column);
				index = 1; row = 0; column = 2;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a hexadecimal integer literal $9" )
	{
		std::string sourceCode = "$9";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::IntegerLiteralHexadecimal,"$9",row,column);
				index = 1; row = 0; column = 2;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a hexadecimal integer literal $9A6B" )
	{
		std::string sourceCode = "$9A6B";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::IntegerLiteralHexadecimal,"$9A6B",row,column);
				index = 1; row = 0; column = 5;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "an octal integer literal &30" )
	{
		std::string sourceCode = "&30";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::IntegerLiteralOctal,"&30",row,column);
				index = 1; row = 0; column = 3;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a double quoted string literal \"hola\"" )
	{
		std::string sourceCode = "\"hola\"";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::StringLiteralDoubleQuote,"\"hola\"",row,column);
				index = 1; row = 0; column = 6;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a double quoted string literal \"hola\"\"\"" )
	{
		std::string sourceCode = "\"hola\"\"\"";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::StringLiteralDoubleQuote,"\"hola\"\"\"",row,column);
				index = 1; row = 0; column = 8;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a one character token +" )
	{
		std::string sourceCode = "+";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::OperatorSum,"+",row,column);
				index = 1; row = 0; column = 1;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a one character token ." )
	{
		std::string sourceCode = ".";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::OperatorAccessor,".",row,column);
				index = 1; row = 0; column = 1;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN( "a two character token .." )
	{
		std::string sourceCode = "..";
		steps.PrepareContext(sourceCode);
		WHEN( "we tokenize" )
		{
			steps.Tokenize();
			THEN( "the result should be" )
			{
				int index = 0; int row = 0; int column = 0;
				steps.AssertTokenValidity(index,TokenClass::PunctuationRangeExclusive,"..",row,column);
				index = 1; row = 0; column = 2;
				steps.AssertTokenValidity(index,TokenClass::EndOfFile,"@",row,column);
			}
		}
	}

	GIVEN("")
	{
		system("pause");
	}

}
