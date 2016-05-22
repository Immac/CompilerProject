#include <sstream>
#include "Parser.h"
using namespace WebPascal;
using namespace Syntactic;

Parser::Parser(WebPascal::Lexical::LexerRef analyzer)
		: _analyzer(analyzer)
{
	_currentToken = _analyzer->GetNextToken();
}

void WebPascal::Syntactic::Parser::Parse()
{
	Program();
}

void WebPascal::Syntactic::Parser::Program() {
	// <HTML>
	ConsumeToken();
	if(this->_currentToken->Type != Lexical::TokenClass::HtmlOpenTag)
	{
		ThrowSyntaxException("Expected HtmlOpenTag"); //TODO: HTML token parsing missing
	}

	BodyHtml();

	if(this->_currentToken->Type != Lexical::TokenClass::HtmlCloseTag)
	{
		ThrowSyntaxException("Expected HtmlOpenTag"); //TODO: HTML token parsing missing
	}
}

void Parser::ConsumeToken()
{
	this->_currentToken = this->_analyzer->GetNextToken();
}

void WebPascal::Syntactic::Parser::ThrowSyntaxException(std::string what)
{
		std::stringstream message;
		std::string errorType = "Syntax Exception: ";
		message << errorType << what
		<< "at col: " << this->_currentToken->Column
		<< " line: " << this->_currentToken->Row;

		throw message.str();
}

void Parser::BodyHtml() {
	BlockHtml();
	BodyHtmlPrime();
}

void Parser::BlockHtml() {
	if(this->_currentToken->Type == Lexical::TokenClass::HtmlContent)
	{
		this->ConsumeToken();
	}
}

void Parser::BodyHtmlPrime() {
	if(this->_currentToken->Type == Lexical::TokenClass::PascalCodeOpen)
	{
		this->ConsumeToken();
		StatementList();
		if(this->_currentToken->Type != Lexical::TokenClass::PascalCodeClose)
		{
			ThrowSyntaxException("Expected PascalCodeClose.");
		}
		BodyHtml();
	}
}

void Parser::StatementList() {
	//TODO
}















