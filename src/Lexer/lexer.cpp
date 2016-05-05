#include "lexer.h"
using namespace WebPascal::Lexical;

Lexer::Lexer(std::string content) : SourceCodeStream(content)
{
	_currentSymbol = this->SourceCodeStream.GetNextSymbol();
}

TokenRef Lexer::GetNextToken()
{
	std::string lexeme;

	while(true)
	{
		switch(this->_state)
		{
			case LexicalState::Initial:
				if(this->_currentSymbol.Value == '\0')
				{
					this->_state = LexicalState::EndOfFile;
					this->UpdatePosition();
					lexeme = "$";
				}
				else if ( isspace(this->_currentSymbol.Value) )
				{
					this->_state = LexicalState::Initial;
					this->_currentSymbol = this->SourceCodeStream.GetNextSymbol();
				}
				break;
		case LexicalState::EndOfFile:
			auto output
					= std::make_shared<Token>(lexeme, TokenClass::EndOfFile, this->_row, this->_column);
			return output;
		}
	}
}

void Lexer::UpdatePosition()
{
	this->_column = this->_currentSymbol.Column;
	this->_row = this->_currentSymbol.Row;
}

