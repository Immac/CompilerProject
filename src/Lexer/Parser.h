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

			void Statement();

			void Declaration();

			void Y();

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

			void NumberOptional();

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

			void X();

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
		};

	}
}



#endif //SRC_WEBPASCALPARSER_H
