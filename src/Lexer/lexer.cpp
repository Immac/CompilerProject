#include "lexer.h"
#include "sstream"

using namespace WebPascal::Lexical;

Lexer::Lexer(std::string content) : SourceCodeStream(content)
{
	_currentSymbol = this->SourceCodeStream.GetNextSymbol();
}

TokenRef Lexer::GetNextToken()
{
	std::string lexeme = "";
	auto state = LexicalState::Initial;
	while(true)
	{
		switch(state)
		{
			case LexicalState::Initial:
				if(this->_currentSymbol.Value == '\0')
				{
					state = LexicalState::EndOfFile;
					this->UpdatePosition();
					lexeme = "$";
				}
				else if ( isspace(this->_currentSymbol.Value) )
				{
					state = LexicalState::Initial;
					this->_currentSymbol = this->SourceCodeStream.GetNextSymbol();
				}
				else if( isalpha(this->_currentSymbol.Value) )
				{
					state = LexicalState::Id;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->_currentSymbol = this->SourceCodeStream.GetNextSymbol();
				}
				break;
		case LexicalState::EndOfFile:
			return std::make_shared<Token>(lexeme, TokenClass::EndOfFile, this->_row, this->_column);
		case LexicalState::Id:
			if( isalnum(this->_currentSymbol.Value) )
			{
				state = LexicalState::Id;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				TokenClass type;
				try
				{
					type = this->ReservedWords.at(lexeme);
				}
				catch (const std::out_of_range& oor)
				{
					 //std::cerr << "Out of Range error: " << oor.what() << '\n';
					 type = TokenClass::Id;
				}
				return std::make_shared<Token>(lexeme, type, this->_row, this->_column);
			}
			break;
		}
	}
}

void Lexer::UpdatePosition()
{
	this->_column = this->_currentSymbol.Column;
	this->_row = this->_currentSymbol.Row;
}

void Lexer::ConsumeSymbol()
{
	this->_currentSymbol = this->SourceCodeStream.GetNextSymbol();
}


//std::stringstream message;
//message << "Lexical analysis error at col: "
//		  << this->_column
//		  << " line: " << this->_row;
//throw message.str()
