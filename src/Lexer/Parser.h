#ifndef SRC_WEBPASCALPARSER_H
#define SRC_WEBPASCALPARSER_H

#include "lexer.h"

namespace WebPascal{
	namespace Syntactic {
		class Parser {
		public:

			Parser(Lexical::Lexer analyzer);

			void Parse();

		private:
			Lexical::Lexer _analyzer;
			Lexical::Token _currentToken;

			void Program();

			void ThrowSyntaxException(std::string what);

			void ConsumeToken();

			void BodyHtml();

			void BlockHtml();

			void BodyHtmlPrime();

			void StatementList();

			void Statement();

			void Declaration();

			void DeclarationTail();

			void AssignValue();

			void If();

			void Block();

			void Else();

			void ForOrForIn();

			void For();

			void ForIn();

			void LoopBlock();

			void LoopStatement();

			void LoopStatementList();

			void While();

			void Repeat();

			void Case();

			void CaseList();

			bool IsCurrentTokenChar();

			void NumberList();

			void CharacterList();

			void CharacterOptional();

			void IdList();

			void IdOptional();

			void DeclareType();

			bool ConsumeIfCurrentTokenTypeIs(Lexical::TokenClass tokenType);

			void EnumeratedType();

			void Record();

			void PropertyList();

			void Array();

			void ArrayTypes();

			void RangeList();

			void Range();

			void RangeOptional();

			void FunctionDeclaration();

			void Params();

			void ParameterDeclarationList();

			void ParameterDeclarationOptional();

			void ProcedureDeclaration();

			void FunctionBlock();

			void StartWithId();

			void Expression();

			void ExpressionList();

			void RelationalExpression();

			void RelationalExpressionPrime();

			void AdditionExpression();

			void AdditionExpressionPrime();

			void MultiplicationExpression();

			void MultiplicationExpressionPrime();

			void UnaryExpression();

			void Factor();

			void CallFunction();

			void MultiplicationOperator();

			void AdditionOperator();

			void RelationalOperator();

			void ExpressionListOptional();

			void Const();

			Lexical::Token ConsumeTerminal(Lexical::TokenClass type);

			void IndexAccess();

			void IntegerLiteral();

			void RealLiteral();

			void StringOrChar();

			void BoolLiteral();

			void IndexAccessOptional();

			void RecordType();

			void TypeDefine();
		};

	}
}



#endif //SRC_WEBPASCALPARSER_H
