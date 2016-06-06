#include <sstream>
#include <list>
#include "Parser.h"
#include "../Tree/ExpressionNode.h"
#include "../Tree/IntegerNode.h"
#include "../Tree/RealNode.h"
#include "../Tree/StringNode.h"
#include "../Semantic/SemanticException.h"
#include "../Tree/BoolNode.h"
#include "../Tree/CharNode.h"
#include "../Tree/IndexAccessor.h"
#include "../Tree/IdNode.h"
#include "../Tree/MemberAccessor.h"

using namespace WebPascal;
using namespace Syntactic;
using namespace Lexical;

Parser::Parser(WebPascal::Lexical::Lexer analyzer)
		: _analyzer(analyzer), _currentToken(_analyzer.GetNextToken())
{

}

void WebPascal::Syntactic::Parser::Parse()
{
	Program();
}

void WebPascal::Syntactic::Parser::Program()
{
	ConsumeTerminal(TokenClass::HtmlOpenTag);
	BodyHtml();
}

void Parser::BodyHtml()
{
	BlockHtml();
	BodyHtmlPrime();
}

void Parser::BlockHtml()
{
	if (_currentToken.Type == TokenClass::HtmlContent)
	{
		ConsumeTerminal(TokenClass::HtmlContent);
		BlockHtml();
	}
	if (_currentToken.Type == TokenClass::HtmlOpenTag)
	{
		ConsumeTerminal(TokenClass::HtmlOpenTag);
		BlockHtml();
	}
}

void Parser::BodyHtmlPrime()
{
	if (_currentToken.Type == TokenClass::HtmlCloseTag)
	{
		ConsumeTerminal(TokenClass::HtmlCloseTag);
	}
	else
	{
		StatementList();
		BodyHtml();
	}
}

void Parser::StatementList()
{
	Statement();
	if (_currentToken.Type == TokenClass::HtmlContent
		|| _currentToken.Type == TokenClass::HtmlCloseTag
		|| _currentToken.Type == TokenClass::HtmlOpenTag
		|| _currentToken.Type == TokenClass::ReservedEnd
		|| _currentToken.Type == TokenClass::ReservedUntil)
	{
	}
	else
	{
		StatementList();
	}
}

void Parser::Statement()
{
	if (_currentToken.Type == TokenClass::ReservedVar)
	{
		Declaration();
	}
	else if (_currentToken.Type == TokenClass::ReservedIf)
	{
		If();
	}
	else if (_currentToken.Type == TokenClass::ReservedFor)
	{
		ForOrForIn();
	}
	else if (_currentToken.Type == TokenClass::ReservedWhile)
	{
		While();
	}
	else if (_currentToken.Type == TokenClass::ReservedRepeat)
	{
		Repeat();
	}
	else if (_currentToken.Type == TokenClass::ReservedConst)
	{
		Const();
	}
	else if (_currentToken.Type == TokenClass::ReservedCase)
	{
		Case();
	}
	else if (_currentToken.Type == TokenClass::ReservedType)
	{
		DeclareType();
	}
	else if (_currentToken.Type == TokenClass::ReservedFunction)
	{
		FunctionDeclaration();
	}
	else if (_currentToken.Type == TokenClass::ReservedProcedure)
	{
		ProcedureDeclaration();
	}
	else if (_currentToken.Type == TokenClass::Id)
	{
		StartWithId();
	}
}

void Parser::Declaration()
{
	if (_currentToken.Type == TokenClass::ReservedVar)
	{
		ConsumeToken();
	}
	else
	{
		ThrowSyntaxException(R"(Expected "var" )");
	}

	if (_currentToken.Type == TokenClass::Id)
	{
		ConsumeToken();
	}
	else
	{
		ThrowSyntaxException(R"(Expected an id)");
	}

	DeclarationTail();

	if (_currentToken.Type == TokenClass::EndOfStatement)
	{
		ConsumeToken();
	}
	else
	{
		ThrowSyntaxException(R"(Expected ";")");
	}

}

void Parser::DeclarationTail()
{
	if (_currentToken.Type == TokenClass::PunctuationComma)
	{
		IdOptional();
		ConsumeTerminal(TokenClass::OperatorTypeAssign);
		ConsumeTerminal(TokenClass::Id);
	}
	else if (_currentToken.Type == TokenClass::OperatorTypeAssign)
	{
		ConsumeTerminal(TokenClass::OperatorTypeAssign);
		ConsumeTerminal(TokenClass::Id);
		AssignValue();
	}
	else
	{
		ThrowSyntaxException(R"(Expected ":" or ",")");
	}
}

void Parser::AssignValue()
{
	if (_currentToken.Type == TokenClass::OperatorAssignOrCompare)
	{
		ConsumeToken();
		Expression();
	}
	else
	{
		// Epsilon
	}
}

void Parser::If()
{
	ConsumeTerminal(TokenClass::ReservedIf);
	Expression();
	ConsumeTerminal(TokenClass::ReservedThen);
	Block();
	Else();
}

void Parser::Block()
{
	if (_currentToken.Type == TokenClass::ReservedBegin)
	{
		ConsumeTerminal(TokenClass::ReservedBegin);
		StatementList();
		ConsumeTerminal(TokenClass::ReservedEnd);
		ConsumeTerminal(TokenClass::EndOfStatement);
	}
	else
	{
		Statement();
	}
}

void Parser::Else()
{
	if (_currentToken.Type == TokenClass::ReservedElse)
	{
		ConsumeTerminal(TokenClass::ReservedElse);
		Block();
	}
	else
	{
		//Epsilon
	}
}

void Parser::ForOrForIn()
{
	ConsumeTerminal(TokenClass::ReservedFor);

	ConsumeTerminal(TokenClass::Id);

	if (_currentToken.Type == TokenClass::OperatorAssign)
	{
		For();
	}
	else if (_currentToken.Type == TokenClass::ReservedIn)
	{
		ForIn();
	}
	else
	{
		ThrowSyntaxException("Expected \":=\" or \"in\"");
	}
}


void Parser::For()
{
	ConsumeTerminal(TokenClass::OperatorAssign);
	Expression();
	ConsumeTerminal(TokenClass::ReservedTo);
	Expression();
	ConsumeTerminal(TokenClass::ReservedDo);
	LoopBlock();
}

void Parser::ForIn()
{
	ConsumeTerminal(TokenClass::ReservedIn);
	ConsumeTerminal(TokenClass::Id);
	ConsumeTerminal(TokenClass::ReservedDo);
	LoopBlock();
}

void Parser::LoopBlock()
{
	if (_currentToken.Type == TokenClass::ReservedBegin)
	{
		ConsumeTerminal(TokenClass::ReservedBegin);
		LoopStatementList();
		ConsumeTerminal(TokenClass::ReservedEnd);
		ConsumeTerminal(TokenClass::EndOfStatement);
	}
	else
	{
		LoopStatement();
	}
}

void Parser::LoopStatement()
{
	if (_currentToken.Type == TokenClass::ReservedContinue)
	{
		ConsumeTerminal(TokenClass::ReservedContinue);
		ConsumeTerminal(TokenClass::EndOfStatement);
		LoopStatementList();
	}
	else if (_currentToken.Type == TokenClass::ReservedBreak)
	{
		ConsumeTerminal(TokenClass::ReservedBreak);
		ConsumeTerminal(TokenClass::EndOfStatement);
		LoopStatementList();
	}
	else
	{
		Statement();
		LoopStatementList();
	}
}

void Parser::LoopStatementList()
{
	if (_currentToken.Type != TokenClass::HtmlContent
		&& _currentToken.Type != TokenClass::HtmlCloseTag
		&& _currentToken.Type != TokenClass::HtmlOpenTag
		&& _currentToken.Type != TokenClass::ReservedEnd
		&& _currentToken.Type != TokenClass::ReservedUntil)
	{
		LoopStatement();
	}
}

void Parser::While()
{
	ConsumeTerminal(TokenClass::ReservedWhile);
	Expression();
	ConsumeTerminal(TokenClass::ReservedDo);
	LoopBlock();
}

void Parser::Repeat()
{
	ConsumeTerminal(TokenClass::ReservedRepeat);
	LoopStatementList();
	ConsumeTerminal(TokenClass::ReservedUntil);
	Expression();
	ConsumeTerminal(TokenClass::EndOfStatement);
}

void Parser::Case()
{
	ConsumeTerminal(TokenClass::ReservedCase);
	AccessorOptional();
	ConsumeTerminal(TokenClass::ReservedOf);
	CaseList();
	ConsumeTerminal(TokenClass::ReservedEnd);
	ConsumeTerminal(TokenClass::EndOfStatement);
}

void Parser::CaseList()
{
	if (_currentToken.Type == TokenClass::ReservedElse)
	{
		Else();
	}
	else
	{
		Expression();
		if (_currentToken.Type == TokenClass::PunctuationRangeExclusive)
		{
			ConsumeTerminal(TokenClass::PunctuationRangeExclusive);
			Expression();
			RangeOptional();
		}
		else if (_currentToken.Type == TokenClass::PunctuationComma)
		{
			ConsumeTerminal(TokenClass::PunctuationComma);
			Expression();
			ExpressionListOptional();
		}
		ConsumeTerminal(TokenClass::OperatorTypeAssign);
		Block();
		if (_currentToken.Type == TokenClass::ReservedEnd)
		{
		}
		else
		{
			CaseList();
		}
	}
}

void Parser::IdList()
{
	ConsumeTerminal(TokenClass::Id);
	IdOptional();
}

void Parser::IdOptional()
{
	if (_currentToken.Type == TokenClass::PunctuationComma)
	{
		ConsumeTerminal(TokenClass::PunctuationComma);
		IdList();
	}
}

void Parser::DeclareType()
{
	ConsumeTerminal(TokenClass::ReservedType);
	ConsumeTerminal(TokenClass::Id);
	ConsumeTerminal(TokenClass::OperatorAssignOrCompare);
	TypeDefine();
	ConsumeTerminal(TokenClass::EndOfStatement);
}

void Parser::TypeDefine()
{
	switch (_currentToken.Type)
	{
		case TokenClass::PunctuationLeftParenthesis:
			EnumeratedType();
			break;
		case TokenClass::Id:
			ConsumeTerminal(TokenClass::Id);
			break;
		case TokenClass::ReservedRecord:
			Record();
			break;
		case TokenClass::ReservedArray:
			Array();
			break;
		default:
			ThrowSyntaxException("Expected an Enum,an id, record or array");
			break;
	}
}

bool Parser::ConsumeIfCurrentTokenTypeIs(Lexical::TokenClass tokenType)
{
	if (_currentToken.Type == tokenType)
	{
		this->ConsumeToken();
		return true;
	}
	return false;
}

void Parser::EnumeratedType()
{
	ConsumeTerminal(TokenClass::PunctuationLeftParenthesis);
	IdList();
	ConsumeTerminal(TokenClass::PunctuationRightParenthesis);
}

void Parser::Record()
{
	ConsumeTerminal(TokenClass::ReservedRecord);
	PropertyList();
	ConsumeTerminal(TokenClass::ReservedEnd);
}

void Parser::PropertyList()
{
	if (_currentToken.Type == TokenClass::Id)
	{
		IdList();
		ConsumeTerminal(TokenClass::OperatorTypeAssign);
		TypeDefine();
		ConsumeTerminal(TokenClass::EndOfStatement);
	}
	else
	{
		//Epsilon
	}
	if (_currentToken.Type == TokenClass::ReservedEnd)
	{
	}
	else
	{
		PropertyList();
	}
}

void Parser::Array()
{
	ConsumeTerminal(TokenClass::ReservedArray);
	ConsumeTerminal(TokenClass::PunctuationOpenSquareBracket);
	RangeList();
	ConsumeTerminal(TokenClass::PunctuationCloseSquareBracket);
	ConsumeTerminal(TokenClass::ReservedOf);
	ArrayTypes();
}

void Parser::ArrayTypes()
{
	switch (_currentToken.Type)
	{
		case TokenClass::Id:
			ConsumeTerminal(TokenClass::Id);
			break;
		case TokenClass::ReservedArray:
			Array();
			break;
		default:
			Range();
	}
}

void Parser::RangeList()
{
	Range();
	RangeOptional();

}

void Parser::Range()
{
	Expression();
	ConsumeTerminal(TokenClass::PunctuationRangeExclusive);
	Expression();
}

void Parser::RangeOptional()
{
	if (ConsumeIfCurrentTokenTypeIs(TokenClass::PunctuationComma))
	{
		RangeList();
	}
	else
	{
		//Epsilon
	}
}

void Parser::FunctionDeclaration()
{
	ConsumeTerminal(TokenClass::ReservedFunction);
	ConsumeTerminal(TokenClass::Id);
	Params();
	ConsumeTerminal(TokenClass::OperatorTypeAssign);
	ConsumeTerminal(TokenClass::Id);
	ConsumeTerminal(TokenClass::EndOfStatement);
	FunctionBlock();
	ConsumeTerminal(TokenClass::EndOfStatement);
}

void Parser::Params()
{
	ConsumeTerminal(TokenClass::PunctuationLeftParenthesis);
	ParameterDeclarationList();
	ConsumeTerminal(TokenClass::PunctuationRightParenthesis);
}

void Parser::ParameterDeclarationList()
{
	if (_currentToken.Type == TokenClass::Id)
	{
		IdList();
		ConsumeTerminal(TokenClass::OperatorTypeAssign);
		ConsumeTerminal(TokenClass::Id);
		ParameterDeclarationOptional();
	}
	else if (_currentToken.Type == TokenClass::ReservedVar)
	{
		ConsumeTerminal(TokenClass::ReservedVar);
		IdList();
		ConsumeTerminal(TokenClass::OperatorTypeAssign);
		ConsumeTerminal(TokenClass::Id);
		ParameterDeclarationOptional();
	}
	else
	{
		//Epsilon
	}
}

void Parser::ParameterDeclarationOptional()
{
	if (_currentToken.Type == TokenClass::EndOfStatement)
	{
		ConsumeTerminal(TokenClass::EndOfStatement);
		ParameterDeclarationList();
	}
	else
	{
		//Epsilon
	}
}

void Parser::FunctionBlock()
{
	ConsumeTerminal(TokenClass::ReservedBegin);
	StatementList();
	ConsumeTerminal(TokenClass::ReservedEnd);
}


void Parser::ProcedureDeclaration()
{
	ConsumeTerminal(TokenClass::ReservedProcedure);
	ConsumeTerminal(TokenClass::Id);
	Params();
	ConsumeTerminal(TokenClass::EndOfStatement);
	FunctionBlock();
	ConsumeTerminal(TokenClass::EndOfStatement);
}

void Parser::StartWithId()
{
	AccessorOptional();
	if (_currentToken.Type == TokenClass::PunctuationLeftParenthesis)
	{
		CallFunction();
		ConsumeTerminal(TokenClass::EndOfStatement);
	}
	else if (_currentToken.Type == TokenClass::OperatorAssign)
	{
		ConsumeTerminal(TokenClass::OperatorAssign);
		Expression();
		ConsumeTerminal(TokenClass::EndOfStatement);
	}
	else
	{
		ThrowSyntaxException("Expected ( or =");
	}
}


Semantic::ExpressionNode &Parser::CallFunction()
{
	ConsumeTerminal(TokenClass::PunctuationLeftParenthesis);
	ExpressionList();
	ConsumeTerminal(TokenClass::PunctuationRightParenthesis);
}

Semantic::ExpressionNode &Parser::Expression()
{
	RelationalExpression();
}

Semantic::ExpressionNode &Parser::ExpressionList()
{
	Expression();
	ExpressionListOptional();
}


void Parser::ExpressionListOptional()
{
	if (_currentToken.Type == TokenClass::PunctuationComma)
	{
		ConsumeTerminal(TokenClass::PunctuationComma);
		ExpressionList();
	}
}

void Parser::RelationalExpression()
{
	AdditionExpression();
	RelationalExpressionPrime();
}

void Parser::RelationalExpressionPrime()
{
	RelationalOperator();
}

void Parser::AdditionExpression()
{
	MultiplicationExpression();
	AdditionExpressionPrime();
}

void Parser::AdditionExpressionPrime()
{
	AdditionOperator();
}

void Parser::MultiplicationExpression()
{
	UnaryExpression();
	MultiplicationExpressionPrime();
}

void Parser::MultiplicationExpressionPrime()
{
	MultiplicationOperator();

}

void Parser::UnaryExpression()
{
	if (_currentToken.Type == TokenClass::ReservedNot)
	{
		ConsumeTerminal(TokenClass::ReservedNot);
		Factor();
	}
	else
	{
		Factor();
	}
}

Semantic::ExpressionNode &Parser::Factor()
{
	switch (_currentToken.Type)
	{
		case TokenClass::Id:
		{
			auto output = &Id();
			switch (_currentToken.Type)
			{
				case TokenClass::PunctuationLeftParenthesis:
					//Todo: Alimentar call function con output?
					return CallFunction();
				default:
					return *output;
			}
		}
		case TokenClass::IntegerLiteralDecimal:
		case TokenClass::IntegerLiteralHexadecimal:
		case TokenClass::IntegerLiteralOctal:
			return IntegerLiteral();
		case TokenClass::RealLiteral:
			return RealLiteral();
		case TokenClass::StringLiteralDoubleQuote:
		case TokenClass::StringLiteralSingleQuote:
			return StringLiteral();
		case TokenClass::CharLiteral:
			return CharLiteral();
		case TokenClass::BooleanLiteralTrue:
		case TokenClass::BooleanLiteralFalse:
			return BoolLiteral();
		case TokenClass::PunctuationLeftParenthesis:
		{
			ConsumeTerminal(TokenClass::PunctuationLeftParenthesis);
			auto output = &Expression();
			ConsumeTerminal(TokenClass::PunctuationRightParenthesis);
			return *output;
		}
		default:
			//Epsilon
			break;
	}
}


void Parser::MultiplicationOperator()
{
	switch (_currentToken.Type)
	{
		case TokenClass::OperatorMultiply:
			ConsumeToken();
			MultiplicationExpression();
			break;
		case TokenClass::OperatorDivide:
			ConsumeToken();
			MultiplicationExpression();
			break;
		case TokenClass::ReservedDiv:
			ConsumeToken();
			MultiplicationExpression();
			break;
		case TokenClass::ReservedMod:
			ConsumeToken();
			MultiplicationExpression();
			break;
		case TokenClass::ReservedAnd:
			ConsumeToken();
			MultiplicationExpression();
			break;
	}
}

void Parser::AdditionOperator()
{
	switch (_currentToken.Type)
	{
		case TokenClass::OperatorSum:
			ConsumeToken();
			AdditionExpression();
			break;
		case TokenClass::OperatorSubtract:
			ConsumeToken();
			AdditionExpression();
			break;
		case TokenClass::ReservedOr:
			ConsumeToken();
			AdditionExpression();
			break;
	}
}

void Parser::RelationalOperator()
{
	switch (_currentToken.Type)
	{
		case TokenClass::OperatorGreaterThan:
			ConsumeToken();
			AdditionExpression();
			RelationalExpressionPrime();
			break;
		case TokenClass::OperatorLessThan:
			ConsumeToken();
			AdditionExpression();
			RelationalExpressionPrime();
			break;
		case TokenClass::OperatorGreaterOrEqualTo:
			ConsumeToken();
			AdditionExpression();
			RelationalExpressionPrime();
			break;
		case TokenClass::OperatorLessThanOrEqualTo:
			ConsumeToken();
			AdditionExpression();
			RelationalExpressionPrime();
			break;
		case TokenClass::OperatorDifferentFrom:
			ConsumeToken();
			AdditionExpression();
			RelationalExpressionPrime();
			break;
		case TokenClass::OperatorAssignOrCompare:
			ConsumeToken();
			AdditionExpression();
			RelationalExpressionPrime();
			break;
	}
}

void Parser::ConsumeToken()
{
	_currentToken = _analyzer.GetNextToken();
}

void WebPascal::Syntactic::Parser::ThrowSyntaxException(std::string what)
{
	std::stringstream message;
	std::string errorType = "Syntax Exception occurred: ";
	message << errorType << what << std::endl
	<< "at col: " << _currentToken.Column << " line: " << _currentToken.Row + 1 << std::endl
	<< "Last lexeme was: " << _currentToken.Lexeme << " of Type  "
	<< Token::GetName(_currentToken.Type) << std::endl;
	throw message.str();
}

void Parser::Const()
{
	ConsumeTerminal(TokenClass::ReservedConst);
	ConsumeTerminal(TokenClass::Id);
	switch (_currentToken.Type)
	{
		case TokenClass::OperatorAssignOrCompare:
			ConsumeTerminal(TokenClass::OperatorAssignOrCompare);
			Expression();
			break;
		case TokenClass::OperatorTypeAssign:
			ConsumeTerminal(TokenClass::OperatorTypeAssign);
			ConsumeTerminal(TokenClass::Id);
			ConsumeTerminal(TokenClass::OperatorAssignOrCompare);
			Expression();
			break;
		default:
			//TODO: function or class for this
			std::string what = "Expected \" ";
			what.append(TokenClassNames[static_cast<int>(TokenClass::OperatorAssignOrCompare)]);
			what.append("\" or \"");
			what.append(TokenClassNames[static_cast<int >(TokenClass::OperatorTypeAssign)]);
			ThrowSyntaxException(what);
	}
	ConsumeTerminal(TokenClass::EndOfStatement);
}

Token Parser::ConsumeTerminal(TokenClass type)
{
	if (_currentToken.Type == type)
	{
		auto token = _currentToken;
		this->ConsumeToken();
		return token;
	}
	else
	{
		std::string what = "Expected \" ";
		what.append(Token::GetName(type));
		what.append("\"");
		ThrowSyntaxException(what);
	}
}


Semantic::ExpressionNode &Parser::IntegerLiteral()
{
	switch (_currentToken.Type)
	{
		case TokenClass::IntegerLiteralDecimal:
		{
			auto token = ConsumeTerminal(TokenClass::IntegerLiteralDecimal);
			return *new Semantic::IntegerNode(token.Lexeme);
		}
		case TokenClass::IntegerLiteralOctal:
		{
			auto token = ConsumeTerminal(TokenClass::IntegerLiteralOctal);
			return *new Semantic::IntegerNode(token.Lexeme);
		}
		case TokenClass::IntegerLiteralHexadecimal:
		{
			auto token = ConsumeTerminal(TokenClass::IntegerLiteralHexadecimal);
			return *new Semantic::IntegerNode(token.Lexeme);
		}
		default:
			std::stringstream what;
			what << "Expected " << Token::GetName(TokenClass::IntegerLiteralDecimal) << std::endl
			<< Token::GetName(TokenClass::IntegerLiteralOctal) << std::endl
			<< "or " << Token::GetName(TokenClass::IntegerLiteralHexadecimal) << std::endl;
			throw Semantic::SemanticException(what.str());
	}
}

Semantic::ExpressionNode &Parser::RealLiteral()
{
	auto token = ConsumeTerminal(TokenClass::RealLiteral);
	return *new Semantic::RealNode(token.Lexeme);
}

Semantic::ExpressionNode &Parser::StringLiteral()
{
	switch (_currentToken.Type)
	{
		case TokenClass::StringLiteralDoubleQuote:
		{
			auto token = ConsumeTerminal(TokenClass::StringLiteralDoubleQuote);
			return *new Semantic::StringNode(token.Lexeme);
		}
		case TokenClass::StringLiteralSingleQuote:
		{
			auto token = ConsumeTerminal(TokenClass::StringLiteralSingleQuote);
			return *new Semantic::StringNode(token.Lexeme);
		}
		default:
			std::stringstream what;
			what << "Expected " << TokenClassNames[(int) TokenClass::StringLiteralSingleQuote] << std::endl
			<< "or " << TokenClassNames[(int) TokenClass::StringLiteralDoubleQuote] << std::endl;
			throw Semantic::SemanticException(what.str());
	}

}

Semantic::ExpressionNode &Parser::BoolLiteral()
{
	switch (_currentToken.Type)
	{
		case TokenClass::BooleanLiteralFalse:
		{
			auto token = ConsumeTerminal(TokenClass::BooleanLiteralFalse);
			return *new Semantic::BoolNode(token.Lexeme);
		}
		case TokenClass::BooleanLiteralTrue:
		{
			auto token = ConsumeTerminal(TokenClass::BooleanLiteralTrue);
			return *new Semantic::BoolNode(token.Lexeme);
		}
		default:
			std::stringstream what;
			what << "Expected " << Token::GetName(TokenClass::BooleanLiteralTrue) << std::endl
			<< "or " << Token::GetName(TokenClass::BooleanLiteralFalse) << std::endl;
			Semantic::SemanticException(what.str()).Throw();
	}
}

Semantic::ExpressionNode &Parser::CharLiteral()
{
	auto token = ConsumeTerminal(TokenClass::CharLiteral);
	return *new Semantic::CharNode(token.Lexeme);
}

Semantic::ExpressionNode &Parser::Id()
{
	auto token = ConsumeTerminal(TokenClass::Id);
	std::string id = token.Lexeme;
	auto accessorList = Accessor();
	return *new Semantic::IdNode(id, accessorList);
}

std::list<Semantic::Accessor *> &Parser::Accessor()
{
	switch (_currentToken.Type)
	{
		case TokenClass::PunctuationOpenSquareBracket:
		{
			return IndexAccessor();
		}
		case TokenClass::OperatorAccessor:
		{
			std::list<Semantic::Accessor *> &accessorList = *(new std::list<Semantic::Accessor *>());
			accessorList.push_back(&MemberAccessorA());
			return accessorList;
		}
		default:
			return *new std::list<Semantic::Accessor *>(); // TODO: ????
	}
}

std::list<Semantic::Accessor *> &Parser::AccessorOptional()
{
	/*if (_currentToken.Type == TokenClass::Id)
	{
		ConsumeTerminal(TokenClass::Id);
		return Accessor();
	}
	else */if (_currentToken.Type == TokenClass::PunctuationOpenSquareBracket)
	{
		return Accessor();
	}
	else if (_currentToken.Type == TokenClass::OperatorAccessor)
	{
		return Accessor();
	}
	return *new std::list<Semantic::Accessor *>();
}

std::list<Semantic::Accessor *> &Parser::IndexAccessor()
{
	ConsumeTerminal(TokenClass::PunctuationOpenSquareBracket);
	Semantic::ExpressionNode &expression = ExpressionList(); // TODO: In this case Expressions should return something ???
	ConsumeTerminal(TokenClass::PunctuationCloseSquareBracket);

	auto accessor = *new Semantic::IndexAccessor(expression);
	std::list<Semantic::Accessor *> &accessorList = AccessorOptional();

	accessorList.insert(accessorList.begin(), &accessor);
	return accessorList;
}

Semantic::Accessor &Parser::MemberAccessorA()
{
	ConsumeTerminal(TokenClass::OperatorAccessor);
	auto token = ConsumeTerminal(TokenClass::Id);
	auto accessorList = Accessor();
	auto idNode = new Semantic::IdNode(token.Lexeme, accessorList);
	return *new Semantic::MemberAccessor(idNode);
}









