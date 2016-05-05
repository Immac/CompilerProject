#include "lexer.h"
using namespace WebPascal::Lexical;

Lexer::Lexer(std::string content) : Content(content)
{
	_currentSymbol = this->Content.GetNextSymbol();
}

TokenRef Lexer::GetNextToken()
{
	std::string lexeme;

	while(true)
	{
		switch(_state)
		{
			case LexicalState::Initial:
				if(this->_currentSymbol.Value == '\0')
				{
					_state = LexicalState::EndOfFile;
					this->UpdatePosition();
					lexeme = "$";
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

