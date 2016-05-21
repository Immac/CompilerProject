#include "lexer.h"
#include "sstream"
#include "../Util/stringextension.h"


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
		const int nullTerminator = '\0';
		switch(state)
		{
			case LexicalState::Initial:
				if(this->_currentSymbol.Value == nullTerminator)
				{
					state = LexicalState::EndOfFile;
					this->UpdatePosition();
					lexeme = "@";
				}
				else if ( isspace(this->_currentSymbol.Value) )
				{
					state = LexicalState::Initial;
					this->ConsumeSymbol();
				}
				else if( isalpha(this->_currentSymbol.Value) )
				{
					state = LexicalState::Id;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if ( isdigit(this->_currentSymbol.Value))
				{
					state = LexicalState::IntegerLiteralDecimal;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if(this->_currentSymbol.Value == '&')
				{
					state = LexicalState::IntegerLiteralOctal;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if ( this->_currentSymbol.Value == '$')
				{
					state = LexicalState::IntegerLiteralHexadecimalSingle;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (this->_currentSymbol.Value == '\"') {
					state = LexicalState::StringLiteralDoubleQuote;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (this->_currentSymbol.Value == '\'') {
					state = LexicalState::StringLiteralSingleQuote;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (Contains<char>(this->UnambiguousPunctuation,this->_currentSymbol.Value))
				{
					state = LexicalState::UnanbiguosPunctuation;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (Contains<char>(this->AmbiguosPunctuationStartSymbols,this->_currentSymbol.Value))
				{
					state = LexicalState::AmbiguousPunctuation;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
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
					auto hola = Util::ToLower(lexeme);
					type = this->ReservedWords.at(hola);
				}
				catch (const std::out_of_range& oor)
				{
					 type = TokenClass::Id;
				}

				return std::make_shared<Token>( lexeme, type, this->_row, this->_column);
			}
			break;
		case LexicalState::IntegerLiteralDecimal:
			if( isdigit(this->_currentSymbol.Value) )
			{
				state = LexicalState::IntegerLiteralDecimal;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				return std::make_shared<Token>(lexeme, TokenClass::IntegerLiteralDecimal, this->_row, this->_column);
			}
			break;
		case LexicalState::IntegerLiteralHexadecimalSingle:
			if(std::find(ValidHexValues.begin(),ValidHexValues.end(),this->_currentSymbol.Value) != ValidHexValues.end())
			{
				state = LexicalState::IntegerLiteralHexadecimalPair;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				ThrowLexicalError("Expected a hexadecimal digit"); // TODO: got <symbol>, lexeme: lexeme
			}
			break;
		case LexicalState::IntegerLiteralHexadecimalPair:
			if(std::find(ValidHexValues.begin(),ValidHexValues.end(),this->_currentSymbol.Value) != ValidHexValues.end())
			{
				state = LexicalState::IntegerLiteralHexadecimalImpair;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else if (lexeme.length() == unsigned(2))
			{
				return std::make_shared<Token>(lexeme, TokenClass::IntegerLiteralHexadecimal, this->_row, this->_column);
			}
			else
			{
				ThrowLexicalError("Expected a hexadecimal digit"); // TODO: got <symbol>, lexeme: lexeme
			}
			break;
		case LexicalState::IntegerLiteralHexadecimalImpair:
			if(std::find(ValidHexValues.begin(),ValidHexValues.end(),this->_currentSymbol.Value) != ValidHexValues.end())
			{
				state = LexicalState::IntegerLiteralHexadecimalPair;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				return std::make_shared<Token>(lexeme, TokenClass::IntegerLiteralHexadecimal, this->_row, this->_column);
			}
		break;
		case LexicalState::IntegerLiteralOctal:
			if(std::find(ValidOctValues.begin(),ValidOctValues.end(),this->_currentSymbol.Value) != ValidOctValues.end())
			{
				state = LexicalState::IntegerLiteralOctal;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else if (lexeme.length() > 1)
			{
				return std::make_shared<Token>(lexeme, TokenClass::IntegerLiteralOctal, this->_row, this->_column);
			}
			else
			{
				ThrowLexicalError("Expected a hexadecimal digit"); // TODO: got <symbol>, lexeme: lexeme
			}
			break;
		case LexicalState::StringLiteralDoubleQuote:
			if(this->_currentSymbol.Value == '\"' )
				{
					state = LexicalState::StringLiteralClosingDoubleQuote;
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
			}
			else if(std::isprint(this->_currentSymbol.Value))
			{
				state = LexicalState::StringLiteralDoubleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				ThrowLexicalError("Expected a closing double quote."); // Todo: classes for exceptions
			}

			break;
		case LexicalState::StringLiteralClosingDoubleQuote:
			if(this->_currentSymbol.Value == '\"')
			{
				state = LexicalState::StringLiteralEscapedDoubleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else if(std::isprint(this->_currentSymbol.Value))
			{
				state = LexicalState::StringLiteralDoubleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				return std::make_shared<Token>(lexeme, TokenClass::StringLiteralDoubleQuote, this->_row, this->_column);
			}
			break;
		case LexicalState::StringLiteralEscapedDoubleQuote:
			if(this->_currentSymbol.Value == '\"')
			{
				state = LexicalState::StringLiteralClosingDoubleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else if(std::isprint(this->_currentSymbol.Value))
			{
				state = LexicalState::StringLiteralDoubleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				ThrowLexicalError("Expected a closing double quote."); // Todo: classes for exceptions
			}

		case LexicalState::StringLiteralSingleQuote:
			if(this->_currentSymbol.Value == '\'' )
			{
				state = LexicalState::StringLiteralClosingSingleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else if(std::isprint(this->_currentSymbol.Value))
			{
				state = LexicalState::StringLiteralSingleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				ThrowLexicalError("Expected a closing Single quote."); // Todo: classes for exceptions
			}

			break;
		case LexicalState::StringLiteralClosingSingleQuote:
			if(this->_currentSymbol.Value == '\'')
			{
				state = LexicalState::StringLiteralEscapedSingleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else if(std::isprint(this->_currentSymbol.Value))
			{
				state = LexicalState::StringLiteralSingleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				return std::make_shared<Token>(lexeme, TokenClass::StringLiteralSingleQuote, this->_row, this->_column);
			}
			break;
		case LexicalState::StringLiteralEscapedSingleQuote:
			if(this->_currentSymbol.Value == '\'')
			{
				state = LexicalState::StringLiteralClosingSingleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else if(std::isprint(this->_currentSymbol.Value))
			{
				state = LexicalState::StringLiteralSingleQuote;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
				ThrowLexicalError("Expected a closing Single quote."); // Todo: classes for exceptions
			}
				
		case LexicalState::UnanbiguosPunctuation:
			return std::make_shared<Token>(lexeme,Punctuation.at(lexeme),this->_row,this->_column);
		case LexicalState::AmbiguousPunctuation:
			auto nextSymbol = this->_currentSymbol;
			auto nextLexeme = lexeme;
			auto isEndOfFile = nextSymbol.Value == nullTerminator;
			if(!isEndOfFile)
			{
				nextLexeme += nextSymbol.Value;
			}

			auto nextLexemeIsContained = this->Punctuation.find(nextLexeme) != this->Punctuation.end();
			if (!isEndOfFile && nextLexemeIsContained)
			{
				state = LexicalState::AmbiguousPunctuation;
				lexeme += this->_currentSymbol.Value;
				this->ConsumeSymbol();
			}
			else
			{
					state = LexicalState::UnanbiguosPunctuation;
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

void Lexer::ThrowLexicalError(std::string what)
{
	std::stringstream message;
	message << what
			  << "at col: " << this->_column
			  << " line: " << this->_row;

	throw message.str();
}
