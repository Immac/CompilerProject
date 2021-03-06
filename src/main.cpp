#include <iostream>
#include <memory>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>
#include "Lexer/lexer.h"
#include "Lexer/Parser.h"
#include "Util/utilities.h"

using namespace WebPascal::Lexical;

class LexicalAnalysisSteps
{
private:
	Lexer _lexer = Lexer("");
	std::vector<Token> _tokenList;
public:

	void HtmlContext(std::string content)
	{
		this->_lexer = Lexer(content);
	}

	void PascalContext(std::string content)
	{
		this->_lexer = Lexer(content);
		this->_lexer.SetAnalysisMode(Lexer::AnalysisMode::Pascal);
	}

	void Tokenize()
	{
		Token currentToken = _lexer.GetNextToken();

		do
		{
			_tokenList.push_back(currentToken);
			currentToken = _lexer.GetNextToken();
		} while (currentToken.Type != TokenClass::EndOfFile);
		_tokenList.push_back(currentToken);
	}

	std::vector<Token> GetTokenList()
	{ return this->_tokenList; }

	void AssertTokenValidity(int index, TokenClass type, std::string lexeme, int row, int column)
	{
		auto token = this->_tokenList.at(index);
		REQUIRE(token.Type == type);
		REQUIRE(token.Lexeme == lexeme);
		REQUIRE(token.Row == row);
		REQUIRE(token.Column == column);
	}

	void Parse()
	{
		WebPascal::Syntactic::Parser parser(_lexer);
		parser.Parse();
	};


};




// TODO: rewrite scenarios taking in account each scenario creates a new "steps"
SCENARIO("Lexical Analysis", "[vector]")
{
	LexicalAnalysisSteps steps;
	GIVEN("An empty source code")
	{
		std::string sourceCode = "";

		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a single whitespace")
	{
		std::string sourceCode = " ";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 1;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a an html opening tag ")
	{
		std::string sourceCode = "<HTML>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlOpenTag, "<html>", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a an html closing tag ")
	{
		std::string sourceCode = "</HTmL>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlCloseTag, "</html>", row, column);
				index = 1;
				row = 0;
				column = 7;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("html body text ")
	{
		std::string sourceCode = "</hola";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlContent, "</hola", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a  pascal code opening tag")
	{
		std::string sourceCode = "<%";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlContent, "", row, column);
				index = 1;
				row = 0;
				column = 2;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a  pascal code closing tag")
	{
		std::string sourceCode = "%>";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 2;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a real literal")
	{
		std::string sourceCode = "1.22";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::RealLiteral, "1.22", row, column);
			}
		}
	}

	GIVEN("a  pascal code opening tag and a pascal closing tab")
	{
		std::string sourceCode = "<%%>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlContent, "", row, column);
				index = 1;
				row = 0;
				column = 4;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a an html tag with different letter casing")
	{
		std::string sourceCode = "</HTmL>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlCloseTag, "</html>", row, column);
				index = 1;
				row = 0;
				column = 7;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a change in analysis type and reserved word")
	{
		std::string sourceCode = "<%array";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlContent, "", row, column);
				index = 1;
				row = 0;
				column = 2;
				steps.AssertTokenValidity(index, TokenClass::ReservedArray, "array", row, column);
				index = 2;
				row = 0;
				column = 7;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a reserved word")
	{
		std::string sourceCode = "array";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::ReservedArray, "array", row, column);
				index = 1;
				row = 0;
				column = 5;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a reserved word in different case")
	{
		std::string sourceCode = "ArRay";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::ReservedArray, "array", row, column);
				index = 1;
				row = 0;
				column = 5;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an id")
	{
		std::string sourceCode = "kanako";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::Id, "kanako", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an id")
	{
		std::string sourceCode = "KaNako";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::Id, "kanako", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a decimal integer literal")
	{
		std::string sourceCode = "12";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::IntegerLiteralDecimal, "12", row, column);
				index = 1;
				row = 0;
				column = 2;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a hexadecimal integer literal $9")
	{
		std::string sourceCode = "$9";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::IntegerLiteralHexadecimal, "$9", row, column);
				index = 1;
				row = 0;
				column = 2;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a hexadecimal integer literal $9A6B")
	{
		std::string sourceCode = "$9A6B";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::IntegerLiteralHexadecimal, "$9a6b", row, column);
				index = 1;
				row = 0;
				column = 5;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a hexadecimal integer literal $9cd")
	{
		std::string sourceCode = "$9cd";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::IntegerLiteralHexadecimal, "$9cd", row, column);
				index = 1;
				row = 0;
				column = 4;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an octal integer literal &30")
	{
		std::string sourceCode = "&30";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::IntegerLiteralOctal, "&30", row, column);
				index = 1;
				row = 0;
				column = 3;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an char literal #20")
	{
		std::string sourceCode = "#20";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::CharLiteral, "#20", row, column);
				index = 1;
				row = 0;
				column = 3;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a double quoted string literal \"hola\"")
	{
		std::string sourceCode = "\"hola\"";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::StringLiteralDoubleQuote, "\"hola\"", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a double quoted string literal \"hola\"\"\"")
	{
		std::string sourceCode = "\"hola\"\"\"";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::StringLiteralDoubleQuote, "\"hola\"\"\"", row, column);
				index = 1;
				row = 0;
				column = 8;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}


	GIVEN("a double single string literal 'abcd'")
	{
		std::string sourceCode = "'abcd'";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::StringLiteralSingleQuote, "'abcd'", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a one character token +")
	{
		std::string sourceCode = "+";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::OperatorSum, "+", row, column);
				index = 1;
				row = 0;
				column = 1;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a one character token .")
	{
		std::string sourceCode = ".";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::OperatorAccessor, ".", row, column);
				index = 1;
				row = 0;
				column = 1;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a two character token ..")
	{
		std::string sourceCode = "..";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::PunctuationRangeExclusive, "..", row, column);
				index = 1;
				row = 0;
				column = 2;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("a two character token .. with comments before")
	{
		std::string sourceCode = "{Hola}..";
		steps.PascalContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 6;
				steps.AssertTokenValidity(index, TokenClass::PunctuationRangeExclusive, "..", row, column);
				index = 1;
				row = 0;
				column = 8;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an open html tag and a closing html tag")
	{
		std::string sourceCode = "<html></html>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlOpenTag, "<html>", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::HtmlCloseTag, "</html>", row, column);
				index = 2;
				row = 0;
				column = 13;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an open html tag and a closing html tag with empty pascal code")
	{
		std::string sourceCode = "<html><%%></html>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlOpenTag, "<html>", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::HtmlContent, "", row, column);
				index = 2;
				row = 0;
				column = 10;
				steps.AssertTokenValidity(index, TokenClass::HtmlCloseTag, "</html>", row, column);
				index = 3;
				row = 0;
				column = 17;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an open html tag and a closing html tag with array pascal code")
	{
		std::string sourceCode = "<html><%array%></html>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlOpenTag, "<html>", row, column);
				index = 1;
				row = 0;
				column = 6;
				steps.AssertTokenValidity(index, TokenClass::HtmlContent, "", row, column);
				index = 2;
				row = 0;
				column = 8;
				steps.AssertTokenValidity(index, TokenClass::ReservedArray, "array", row, column);
				index = 3;
				row = 0;
				column = 15;
				steps.AssertTokenValidity(index, TokenClass::HtmlCloseTag, "</html>", row, column);
				index = 4;
				row = 0;
				column = 22;
				steps.AssertTokenValidity(index, TokenClass::EndOfFile, "@", row, column);
			}
		}
	}

	GIVEN("an simple program")
	{
		std::string sourceCode = "<html><%Write('Hello World!');Readln();%></html>";
		steps.HtmlContext(sourceCode);
		WHEN("we tokenize")
		{
			steps.Tokenize();
			THEN("the result should be")
			{
				int index = 0;
				int row = 0;
				int column = 0;
				steps.AssertTokenValidity(index, TokenClass::HtmlOpenTag, "<html>", row, column);
				//TODO: Asserts, for now it's ok that it passes
			}
		}
	}
}

SCENARIO("Syntactic Analysis", "")
{
	LexicalAnalysisSteps steps;
	GIVEN("other simple program")
	{
		std::string sourceCode = "<html><%Write('Hello World!');Readln();%></html>";
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("yet another simple program 2")
	{
		std::string sourceCode = Util::File::LoadTextFile("simple-test-2.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("a simple program 1")
	{
		std::string sourceCode = Util::File::LoadTextFile("simple-test-1.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("a simple program 3")
	{
		std::string sourceCode = Util::File::LoadTextFile("simple-test-3.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("a simple program 4")
	{
		std::string sourceCode = Util::File::LoadTextFile("simple-test-4.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("a simple program with while statements")
	{
		std::string sourceCode = Util::File::LoadTextFile("while-test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("a simple program with repeat statements")
	{
		std::string sourceCode = Util::File::LoadTextFile("repeat-test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("a simple program with const declarations")
	{
		std::string sourceCode = Util::File::LoadTextFile("const-declaration.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("provided the file accessors.pas")
	{
		std::string sourceCode = Util::File::LoadTextFile("accessors.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("provided the file case-test.pas ")
	{
		std::string sourceCode = Util::File::LoadTextFile("case-test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("provided the file declare-method-test.pas ")
	{
		std::string sourceCode = Util::File::LoadTextFile("declare-method-test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("provided the file declare-function-test.pas ")
	{
		std::string sourceCode = Util::File::LoadTextFile("declare-function-test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}


	GIVEN("yet another simple program")
	{
		std::string sourceCode = Util::File::LoadTextFile("simple-test-3.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("yet another simple program call-function-test.pas")
	{
		std::string sourceCode = Util::File::LoadTextFile("call-function-test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("yet another simple program typedef-test.pas")
	{
		std::string sourceCode = Util::File::LoadTextFile("typedef-test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("yet another simple program record.pas")
	{
		std::string sourceCode = Util::File::LoadTextFile("record.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}

	GIVEN("a complex program test.pas")
	{
		std::string sourceCode = Util::File::LoadTextFile("test.pas");
		steps.HtmlContext(sourceCode);
		WHEN("we parse")
		{
			steps.Parse();
			THEN("it should pass")
			{
				//TODO: Asserts, for now it's ok if it passes
			}
		}
	}
}
