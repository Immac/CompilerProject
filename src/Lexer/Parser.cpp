#include <sstream>
#include "Parser.h"
//TODO: fix the iisue with consuming the character when trying to know what to do next
using namespace WebPascal;
using namespace Syntactic;
using namespace Lexical;
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
	if(this->_currentToken->Type != TokenClass::HtmlOpenTag)
	{
		ThrowSyntaxException("Expected HtmlOpenTag");
	}

	BodyHtml();

	if(this->_currentToken->Type != TokenClass::HtmlCloseTag)
	{
		ThrowSyntaxException("Expected HtmlOpenTag");
	}
}

void Parser::ConsumeToken()
{
	_currentToken = _analyzer->GetNextToken();
}

void WebPascal::Syntactic::Parser::ThrowSyntaxException(std::string what)
{
	std::stringstream message;
	std::string errorType = "Syntax Exception: ";
	message << errorType << what
	<< "at col: " << _currentToken->Column
	<< " line: " << _currentToken->Row;

	throw message.str();
}

void Parser::BodyHtml() {
	BlockHtml();
	BodyHtmlPrime();
}

void Parser::BlockHtml() {
	if(_currentToken->Type == TokenClass::HtmlContent)
	{
		this->ConsumeToken();
	}
}

void Parser::BodyHtmlPrime() {
	if(_currentToken->Type == TokenClass::PascalCodeOpen)
	{
		this->ConsumeToken();
		StatementList();
		if(_currentToken->Type != TokenClass::PascalCodeClose)
		{
			ThrowSyntaxException("Expected PascalCodeClose.");
		}
		BodyHtml();
	}
}

void Parser::StatementList() {
	Statement();
	if(_currentToken->Type != TokenClass::PascalCodeClose)
	{
		StatementList();
	}

}

void Parser::Statement() {
	if(_currentToken->Type == TokenClass::ReservedVar)
	{
		this->ConsumeToken();
		Declaration();
	}
	else if (_currentToken->Type == TokenClass::ReservedIf)
	{
		this->ConsumeToken();
		If();
	}
	else if(_currentToken->Type == TokenClass::ReservedFor)
	{
		this->ConsumeToken();
		ForOrForIn();
	}
	else if(_currentToken->Type == TokenClass::ReservedWhile)
	{
		this->ConsumeToken();
		While();
	}
	else if(_currentToken->Type == TokenClass::ReservedRepeat)
	{
		this->ConsumeToken();
		Repeat();
	}
	else if(_currentToken->Type == TokenClass::ReservedCase)
	{
		this->ConsumeToken();
		Case();
	}
	else if(_currentToken->Type == TokenClass::ReservedType)
	{
		this->ConsumeToken();
		DeclareType();
	}
	else if(_currentToken->Type == TokenClass::ReservedFunction)
	{
		this->ConsumeToken();
		FunctionDeclaration();
	}
	else if(_currentToken->Type == TokenClass::ReservedProcedure)
	{
		this->ConsumeToken();
		ProcedureDeclaration();
	}
	else if(_currentToken->Type == TokenClass::Id)
	{
		this->ConsumeToken();
		StartWithId();
	}
}

void Parser::Declaration() {
	if(_currentToken->Type == TokenClass::Id)
	{
		this->ConsumeToken();
		Y();
		if(_currentToken->Type == TokenClass::EndOfStatement)
		{
			ConsumeToken();
		}
		else
		{
			ThrowSyntaxException("Expected EndOfStatement Token");
		}
	}
}

void Parser::Y() {
	if(_currentToken->Type == TokenClass::Id)
	{
		this->ConsumeToken();
		if(_currentToken->Type == TokenClass::OperatorTypeAssign)
		{
			this->ConsumeToken();
			if(_currentToken->Type == TokenClass::Id)
			{
				this->ConsumeToken();
			}
			else
			{
				ThrowSyntaxException("Expected an id");
			}
		}
		else
		{
			ThrowSyntaxException("Expected OperatorTypeAssign");
		}
	}
	else if(_currentToken->Type == TokenClass::OperatorTypeAssign)
	{
		this->ConsumeToken();
		if(_currentToken->Type == TokenClass::Id)
		{
			this->ConsumeToken();
			AssignValue();
		}
		else
		{
			ThrowSyntaxException("Expected an id");
		}
	}
}

void Parser::AssignValue() {
	if(_currentToken->Type == TokenClass::OperatorAssignOrCompare)
	{
		this->ConsumeToken();
		Expression();
	}
	else
	{
		//Epsilon
	}
}

void Parser::If() {
	Expression();
	if(_currentToken->Type == TokenClass::ReservedThen)
	{
		this->ConsumeToken();
		Block();
		Else();
	}
	else
	{
		ThrowSyntaxException("Expected the \"then\" keyword");
	}
}

void Parser::Block() {
	if(_currentToken->Type == TokenClass::ReservedBegin)
	{
		this->ConsumeToken();
		StatementList();
		if(_currentToken->Type == TokenClass::ReservedEnd) {
			this->ConsumeToken();
		}
		else
		{
			ThrowSyntaxException("Expected the \"end\" keyword");
		}
	}
	else
	{
		Statement();
	}
}

void Parser::Else() {
	if(_currentToken->Type == TokenClass::ReservedElse)
	{
		this->ConsumeToken();
		Block();
	}
	else
	{
		//Epsilon
	}
}

void Parser::ForOrForIn() {
	if(_currentToken->Type == TokenClass::OperatorAssign)
	{
		this->ConsumeToken();
		For();
	}
	else if (_currentToken->Type == TokenClass::ReservedIn)
	{
		this->ConsumeToken();
		ForIn();
	}
	else
	{
		ThrowSyntaxException("Expected \":=\" or \"in\"");
	}
}

void Parser::For()
{
	Expression();
	if(_currentToken->Type == TokenClass::ReservedTo)
	{
		this->ConsumeToken();
		Expression();
		if(_currentToken->Type == TokenClass::ReservedDo) {
			this->ConsumeToken();
			LoopBlock();
		}
		else
		{
			ThrowSyntaxException("Expected: \"do\"");
		}
	}
	else
	{
		ThrowSyntaxException("Expected: \"to\"");
	}
}

void Parser::ForIn() {
	if(_currentToken->Type == TokenClass::ReservedIn)
	{
		this->ConsumeToken();
		if(_currentToken->Type == TokenClass::Id)
		{
			this->ConsumeToken();
			if(_currentToken->Type == TokenClass::ReservedDo)
			{
				this->ConsumeToken();
				LoopBlock();
			}
			else
			{
				ThrowSyntaxException("Expected: \"do\"");
			}
		}
		else
		{
			ThrowSyntaxException("Expected: an \"id\"");
		}
	}
	else
	{
		ThrowSyntaxException("Expected: \"in\"");
	}

}

void Parser::LoopBlock() {
	if(_currentToken->Type == TokenClass::ReservedBegin)
	{
		ConsumeToken();
		LoopStatement();
	}
	else
	{
		LoopStatementList();
	}
}

void Parser::LoopStatement() {
	if(_currentToken->Type == TokenClass::ReservedContinue)
	{
		this->ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::ReservedBreak)
	{
		this->ConsumeToken();
	}
	else
	{
		Statement();
	}
}

void Parser::LoopStatementList()
{
	LoopStatement();
	if(_currentToken->Type != TokenClass::EndOfStatement)
	{
		LoopStatementList();
	}
}

void Parser::While() {
	Expression();
	if(_currentToken->Type == TokenClass::ReservedDo)
	{
		this->ConsumeToken();
		LoopBlock();
	}
	else
	{
		ThrowSyntaxException("Expected: an \"do\"");
	}
}

void Parser::Repeat() {
	LoopStatementList();
	if(_currentToken->Type == TokenClass::ReservedUntil)
	{
		this->ConsumeToken();
		Expression();
	}
	else
	{
		ThrowSyntaxException("Expected: an \"until\"");
	}
}

void Parser::Case() {
	if(_currentToken->Type == TokenClass::Id)
	{
		this->ConsumeToken();
		if(_currentToken->Type == TokenClass::ReservedOf)
		{
			this->ConsumeToken();
			CaseList();
		}
		else
		{
			ThrowSyntaxException("Expected: an \"of\"");
		}
	}
	else
	{
		ThrowSyntaxException("Expected: an \"id\"");
	}
}

void Parser::CaseList() {
	this->ConsumeToken();
	if(_currentToken->Type == TokenClass::ReservedElse)
	{
		this->ConsumeToken();
		Else();
	}
	else if(_currentToken->Type == TokenClass::IntegerLiteralDecimal)
	{
		this->ConsumeToken();
		NumberList();
	}
	else if(_currentToken->Type == TokenClass::IntegerLiteralHexadecimal)
	{
		this->ConsumeToken();
		NumberList();
	}
	else if(_currentToken->Type == TokenClass::IntegerLiteralOctal)
	{
		this->ConsumeToken();
		NumberList();
	}
	else if(_currentToken->Type == TokenClass::StringLiteralDoubleQuote)
	{
		if(!IsCurrentTokenChar())
		{
			ThrowSyntaxException("Expected a char literal");
		}
		this->ConsumeToken();
		CharacterList();
	}
	else if(_currentToken->Type == TokenClass::StringLiteralSingleQuote)
	{
		this->ConsumeToken();
		CharacterList();
	}
	else if(_currentToken->Type == TokenClass::CharLiteral)
	{
		this->ConsumeToken();
		CharacterList();
	}
	else if(_currentToken->Type == TokenClass::Id)
	{
		this->ConsumeToken();
		IdList();
	}
}

bool Parser::IsCurrentTokenChar() {
	auto size = _currentToken->Lexeme.size();
	return size == 1
		&& (_currentToken->Type == TokenClass::StringLiteralSingleQuote
	  		|| _currentToken->Type == TokenClass::StringLiteralSingleQuote)
			|| _currentToken->Type == TokenClass::CharLiteral;
}

void Parser::NumberList() {
	NumberOptional();
}

void Parser::NumberOptional() {

	if(_currentToken->Type == TokenClass::PunctuationComma)
	{
		this->ConsumeToken();
		if(_currentToken->Type == TokenClass::IntegerLiteralOctal)
		{
			this->ConsumeToken();

		}
		else if(_currentToken->Type == TokenClass::IntegerLiteralHexadecimal)
		{
			this->ConsumeToken();

		}
		else if(_currentToken->Type == TokenClass::IntegerLiteralDecimal)
		{
			this->ConsumeToken();
		}
		else
		{
			ThrowSyntaxException("Expected a numeric literal");
		}
	}
}

void Parser::CharacterList() {
	CharacterOptional();
}

void Parser::CharacterOptional() {
	if(_currentToken->Type == TokenClass::PunctuationComma)
	{
		this->ConsumeToken();
		if(_currentToken->Type == TokenClass::StringLiteralSingleQuote && IsCurrentTokenChar())
		{
			this->ConsumeToken();
			CharacterList();
		}
		else if(_currentToken->Type == TokenClass::StringLiteralDoubleQuote && IsCurrentTokenChar())
		{
			this->ConsumeToken();
			CharacterList();
		}
		else if(_currentToken->Type == TokenClass::CharLiteral)
		{
			this->ConsumeToken();
			CharacterList();
		}
		else
		{
			ThrowSyntaxException("Expected a char literal");
		}
	}
}

void Parser::IdList() {
	IdOptional();
}

void Parser::IdOptional() {
	if(_currentToken->Type == TokenClass::PunctuationComma)
	{
		this->ConsumeToken();
		if(_currentToken->Type == TokenClass::Id)
		{
			this->ConsumeToken();
			IdList();
		}
		else
		{
			ThrowSyntaxException("Expected an id");
		}
	}
}

void Parser::DeclareType() {
	if(this->ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedType))
	{
		if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
		{
			if(ConsumeIfCurrentTokenTypeIs(TokenClass::OperatorTypeAssign))
			{
				if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationLeftParenthesis))
				{
					EnumeratedType();
				}
				else if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
				{
					// Typedef
				}
				else if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedRecord))
				{
					Record();
				}
				else if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedArray))
				{
					Array();
				}
				else
				{
					ThrowSyntaxException("Expected an Enum,an id, record or array");
				}
			}
			else
			{
				ThrowSyntaxException("Expected id");
			}
		}
		else
		{
			ThrowSyntaxException("Expected id");
		}
	}
	else
	{
		ThrowSyntaxException("Expected type;");
	}
}

bool Parser::ConsumeIfCurrentTokenTypeIs(Lexical::TokenClass tokenType) {
	if(_currentToken->Type == tokenType)
	{
		this->ConsumeToken();
		return true;
	}
	return false;
}

void Parser::EnumeratedType() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id)) {
		IdList();
	}
	else
	{
		ThrowSyntaxException("Expected id");
	}
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationRightParenthesis))
	{
		// Empty on purpose
	}
	else
	{
		ThrowSyntaxException("Expected )");
	}
}

void Parser::Record() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
	{
		PropertyList();
	}
	else
	{
		ThrowSyntaxException("Expected an id");
	}

	if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedEnd))
	{

	}
	else
	{
		ThrowSyntaxException("Expected end");
	}

	if(ConsumeIfCurrentTokenTypeIs(TokenClass::EndOfStatement))
	{

	}
	else
	{
		ThrowSyntaxException("Expected ;");
	}

}

void Parser::PropertyList() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::OperatorTypeAssign))
	{
		if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationLeftParenthesis))
		{
			EnumeratedType();
		}
		else if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
		{
			// Typedef
		}
		else if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedRecord))
		{
			Record();
		}
		else
		{
			ThrowSyntaxException("Expected (, an id, or record");
		}
	}
	else
	{
		ThrowSyntaxException("Expected a type assign operator");
	}
}

void Parser::Array() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationOpenSquareBracket))
	{
	}
	else
	{
		ThrowSyntaxException("Expected [");
	}
	RangeList();
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationCloseSquareBracket))
	{
	}
	else
	{
		ThrowSyntaxException("Expected ]");
	}
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedOf))
	{
	}
	else
	{
		ThrowSyntaxException("Expected of");
	}
	ArrayTypes();
}

void Parser::ArrayTypes() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
	{
	}
	else if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedArray))
	{
		Array();
	}
	else
	{
		Range();
	}
}

void Parser::RangeList() {
	Range();
	RangeOptional();

}

void Parser::Range() {
	Expression();
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationRangeExclusive))
	{
	}
	else
	{
		ThrowSyntaxException("Expected ..");
	}
	Expression();
}

void Parser::RangeOptional() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationComma))
	{
		RangeList();
	}
	else
	{
		//Epsilon
	}
}

void Parser::FunctionDeclaration() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
	{
	}
	else
	{
		ThrowSyntaxException("Expected id");
	}
	Params();
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::EndOfStatement))
	{
	}
	else
	{
		ThrowSyntaxException("Expected ;");
	}
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::OperatorTypeAssign))
	{
	}
	else
	{
		ThrowSyntaxException("Expected :");
	}
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
	{
	}
	else
	{
		ThrowSyntaxException("Expected id");
	}
	FunctionBlock();
}

void Parser::Params() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationLeftParenthesis))
	{
	}
	else
	{
		ThrowSyntaxException("Expected (");
	}
	ParameterDeclarationList();
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationRightParenthesis))
	{
	}
	else
	{
		ThrowSyntaxException("Expected )");
	}
}

void Parser::ParameterDeclarationList () {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
	{
		if(ConsumeIfCurrentTokenTypeIs(TokenClass::OperatorTypeAssign))
		{

		}
		else
		{
			ThrowSyntaxException("Expected :");
		}
		if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
		{

		}
		else
		{
			ThrowSyntaxException("Expected id");
		}
		ParameterDeclarationOptional();

	}
	else if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedVar))
	{
		if(_currentToken->Type == TokenClass::Id) {
			this->ConsumeToken();
			IdList();
		}
		else
		{
			ThrowSyntaxException("Expected id or var");
		}
		if(ConsumeIfCurrentTokenTypeIs(TokenClass::OperatorTypeAssign))
		{

		}
		else
		{
			ThrowSyntaxException("Expected :");
		}
		ParameterDeclarationOptional();
	}
	else
	{
		//Epsilon
	}


}

void Parser::ParameterDeclarationOptional()
{
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationComma))
	{
		ParameterDeclarationList();
	}
}

void Parser::FunctionBlock() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedBegin))
	{
	}
	else
	{
		ThrowSyntaxException("Expected begin");
	}
	StatementList();
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::ReservedEnd))
	{
	}
	else
	{
		ThrowSyntaxException("Expected end");
	}
}



void Parser::ProcedureDeclaration()
{
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::Id))
	{
	}
	else
	{
		ThrowSyntaxException("Expected id");
	}
	Params();
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::EndOfStatement))
	{
	}
	else
	{
		ThrowSyntaxException("Expected ;");
	}
	FunctionBlock();
}

void Parser::StartWithId() {
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationLeftParenthesis))
	{
		CallFunction();
	}
	else if (ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationLeftParenthesis))
	{
		AssignValue();
	}
	else
	{
		ThrowSyntaxException("Expected ( or =");
	}

}

void Parser::CallFunction() {
	ExpressionList();
	if(ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationRightParenthesis))
		{
		}
		else
		{
			ThrowSyntaxException("Expected )");
		}
}

void Parser::Expression() {
	RelationalExpression();
}

void Parser::ExpressionList() {

}

void Parser::RelationalExpression() {
	AdditionExpression();
	RelationalExpressionPrime();
}

void Parser::RelationalExpressionPrime() {
	RelationalOperator();
	AdditionExpression();
	RelationalExpressionPrime();
}

void Parser::AdditionExpression() {
	MultiplicationExpression();
	AdditionExpressionPrime();
}

void Parser::AdditionExpressionPrime() {
	AdditionOperator();
	AdditionExpressionPrime();
}

void Parser::MultiplicationExpression() {
	UnaryExpression();
	MultiplicationExpressionPrime();
}

void Parser::MultiplicationExpressionPrime() {
	MultiplicationOperator();
	MultiplicationExpressionPrime();
}

void Parser::UnaryExpression() {
	if(_currentToken->Type == TokenClass::ReservedNot)
	{
		ConsumeToken();
		Factor();
	}
	else
	{
		Factor();
	}
}

void Parser::Factor() {
	if(_currentToken->Type == TokenClass::Id)
	{
		ConsumeToken();
		if(_currentToken->Type == TokenClass::PunctuationLeftParenthesis)
		{
			CallFunction();
		}
	}
	else if(_currentToken->Type == TokenClass::IntegerLiteralDecimal)
	{
		ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::IntegerLiteralOctal)
	{
		ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::IntegerLiteralHexadecimal)
	{
		ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::StringLiteralSingleQuote)
	{
		ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::StringLiteralDoubleQuote)
	{
		ConsumeToken();
	}
	else if(IsCurrentTokenChar()) // Todo: temporary quick fix
	{
		ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::BooleanLiteralTrue)
	{
		ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::BooleanLiteralFalse)
	{
		ConsumeToken();
	}
	else if(_currentToken->Type == TokenClass::PunctuationLeftParenthesis)
	{
		ConsumeToken();
	}
	else
	{
		Expression();
	}
}

void Parser::MultiplicationOperator() {
	switch (_currentToken->Type )
	{
		case TokenClass::OperatorMultiply:
			ConsumeToken();
			break;
		case TokenClass::OperatorDivide:
			ConsumeToken();
			break;
		case TokenClass::ReservedDiv:
			ConsumeToken();
			break;
		case TokenClass::ReservedMod:
			ConsumeToken();
			break;
		case TokenClass::ReservedAnd:
			ConsumeToken();
			break;
		default:
			ThrowSyntaxException("expected a multiplication level operator");
	}
}

void Parser::AdditionOperator() {
	switch (_currentToken->Type)
	{
		case TokenClass::OperatorSum:
			ConsumeToken();
			break;
		case TokenClass::OperatorSubtract:
			ConsumeToken();
			break;
		case TokenClass::ReservedOr:
			ConsumeToken();
			break;
		default:
			ThrowSyntaxException("expected a multiplication level operator");
	}
}

void Parser::RelationalOperator() {
	switch (_currentToken->Type )
	{
		case TokenClass::OperatorGreaterThan:
			ConsumeToken();
			break;
		case TokenClass::OperatorLessThan:
			ConsumeToken();
			break;
		case TokenClass::OperatorGreaterOrEqualTo:
			ConsumeToken();
			break;
		case TokenClass::OperatorLessThanOrEqualTo:
			ConsumeToken();
			break;
		case TokenClass::OperatorDifferentFrom:
			ConsumeToken();
			break;
		case TokenClass::OperatorAssignOrCompare:
			ConsumeToken();
			break;
		default:
			ThrowSyntaxException("expected a multiplication level operator");
	}
}

