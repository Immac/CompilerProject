#include "lexer.h"
#include "sstream"
#include "../Util/utilities.h"


using namespace WebPascal::Lexical;

Lexer::Lexer(std::string content) : SourceCodeStream(content)
{
	_currentSymbol = this->SourceCodeStream.GetNextSymbol();
}

TokenRef Lexer::GetNextToken()
{
	switch (_analysisMode)
	{
		case AnalysisMode::Html:
			return HtmlToken();
		case AnalysisMode ::Pascal:
			return PascalToken();
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
	std::string errorType = "LexicalException: ";
	message << errorType << what
	<< "at col: " << this->_column
	<< " line: " << this->_row;

	throw message.str();
}

TokenRef Lexer::PascalToken() {
	std::string lexeme = "";
	auto state = LexicalState::Initial;
	this->UpdatePosition();
	while(true)
	{
		switch(state)
		{
			case LexicalState::Initial:
				if ( isspace(this->_currentSymbol.Value) )
				{
					state = LexicalState::Initial;
					this->ConsumeSymbol();
				}
				else if(this->_currentSymbol.Value == nullTerminator)
				{
					state = LexicalState::EndOfFile;
					this->UpdatePosition();
					lexeme = "@";
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
				else if (this->_currentSymbol.Value == '%')
				{
					state = LexicalState::SwitchToHtmlAnalysis;
				}
				else if (Contains<char>(this->UnambiguousPunctuation,this->_currentSymbol.Value))
				{
					state = LexicalState::UnambiguousPunctuationState;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (Contains<char>(this->AmbiguousPunctuationStartSymbols,this->_currentSymbol.Value))
				{
					state = LexicalState::AmbiguousPunctuation;
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				break;
			case LexicalState::Id:
				if( isalnum(this->_currentSymbol.Value) )
				{
					state = LexicalState::Id;
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					auto lexemeLowercase = Util::ToLower(lexeme);
					TokenClass type;
					try
					{
						type = this->ReservedWords.at(lexemeLowercase);
					}
					catch (const std::out_of_range& oor)
					{
						type = TokenClass::Id; // TODO: use find instead of catching exception
					}

					return std::make_shared<Token>( lexemeLowercase, type, this->_row, this->_column);
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
					state = LexicalState::IntegerLiteralHexadecimalDigits;
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					ThrowLexicalError("Expected a hexadecimal digit"); // TODO: got <symbol>, lexeme: lexeme
				}
				break;
			case LexicalState::IntegerLiteralHexadecimalDigits:
				if(Util::Contains<char>(ValidHexValues,tolower(this->_currentSymbol.Value)))
				{
					state = LexicalState::IntegerLiteralHexadecimalDigits;
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return std::make_shared<Token>(Util::ToLower(lexeme), TokenClass::IntegerLiteralHexadecimal, this->_row, this->_column);
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
				break;
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
					ThrowLexicalError("Expected a closing single quote."); // Todo: classes for exceptions
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

			case LexicalState::UnambiguousPunctuationState:
				if(lexeme == OpenCommentCurlyBrace)
				{
					state = LexicalState::CurlyBraceOpenedCommentBody;
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if(lexeme == OpenCommentParenthesis)
				{
					state = LexicalState::ParenthesisOpenedCommentBody;
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return std::make_shared<Token>(lexeme,Punctuation.at(lexeme),this->_row,this->_column);
				}
				break;
			case LexicalState ::CurlyBraceOpenedCommentBody:
			{
				auto size = lexeme.length();
				auto lastChar = lexeme.substr(size -1);
				lexeme += this->_currentSymbol.Value;
				if (lastChar == CloseCommentCurlyBrace)
				{
					lexeme = "";
					state = LexicalState::Initial;
				}
				else
				{
					this->ConsumeSymbol();
				}
			}
				break;
			case LexicalState::ParenthesisOpenedCommentBody:
			{
				auto size = lexeme.length();
				auto lastChar = lexeme.substr(size -2);
				lexeme += this->_currentSymbol.Value;
				if (lastChar == CloseCommentParenthesis)
				{
					lexeme = "";
					state = LexicalState::Initial;
				}
				else
				{
					this->ConsumeSymbol();
				}
			}
				break;
			case LexicalState::AmbiguousPunctuation:
			{
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
					state = LexicalState::UnambiguousPunctuationState;
				}
			}
				break;
			case LexicalState::SwitchToHtmlAnalysis:
				this->ConsumeSymbol();
				if(this->_currentSymbol.Value == '>')
				{
					this->SetAnalysisMode(AnalysisMode::Html);
					this->ConsumeSymbol();
					return std::make_shared<Token>
							("", TokenClass::PascalCodeClose, this->_row, this->_column);
				}
				else
				{
					ThrowLexicalError("Expected '>'");
				}
				break;
			case LexicalState::EndOfFile:
				//TODO: Remove Temporary EOF, the actual thing should never end inside the Pascal code
				return std::make_shared<Token>(lexeme, TokenClass::EndOfFile, this->_row, this->_column);
		}
	}
}

TokenRef Lexer::HtmlToken() {
	std::string lexeme = "";
	auto state = LexicalState::Initial;
	this->UpdatePosition();
	while(true)
	{
		size_t lexemeSize = lexeme.size();
		size_t lexemeSizeMinusTwo = lexemeSize - 2;
		std::string lastTwo = lexemeSize >= 2
							  ? lexeme.substr(lexemeSizeMinusTwo)
							  : "";
		switch (state)
		{
			case LexicalState ::Initial:
				if ( isspace(this->_currentSymbol.Value) )
				{
					state = LexicalState::Initial;
					this->ConsumeSymbol();
				}
				else if(lastTwo == "<%")
				{
					this->SetAnalysisMode(AnalysisMode::Pascal);
					return std::make_shared<Token>
							(lexeme.substr(0, lexemeSizeMinusTwo), TokenClass::HtmlContent, this->_row, this->_column);
				}
				else if(this->_currentSymbol.Value == nullTerminator)
				{
					state = LexicalState::EndOfFile;
					this->UpdatePosition();
					lexeme = "@";
				}
				else if (lexemeSize == 0 && (this->_currentSymbol.Value == '<'))
				{
					this->UpdatePosition();
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
					state = LexicalState::TryForHtml;
				}
				else
				{
					this->UpdatePosition();
					state = LexicalState::HtmlContent;
				}
				break;
			case LexicalState::TryForHtml:
			{
				lexeme += this->_currentSymbol.Value;
				bool isClosingTag = false;

				if (tolower(this->_currentSymbol.Value) == '/') {
					this->ConsumeSymbol();
					lexeme += this->_currentSymbol.Value;
					isClosingTag = true;
				}
				if (tolower(this->_currentSymbol.Value) != 'h') {
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += this->_currentSymbol.Value;
				if (tolower(this->_currentSymbol.Value) != 't') {
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += this->_currentSymbol.Value;
				if (tolower(this->_currentSymbol.Value) != 'm') {
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += this->_currentSymbol.Value;
				if (tolower(this->_currentSymbol.Value) != 'l') {
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += this->_currentSymbol.Value;
				if (tolower(this->_currentSymbol.Value) != '>') {
					this->ConsumeSymbol();
					state = LexicalState::Initial;
					break;
				}
				ConsumeSymbol();
				if (isClosingTag)
				{
					return std::make_shared<Token>(Util::ToLower(lexeme), TokenClass::HtmlCloseTag, this->_row, this->_column);
				}
				else
				{
					return std::make_shared<Token>(Util::ToLower(lexeme), TokenClass::HtmlOpenTag, this->_row, this->_column);
				}
			}
			case LexicalState::EndOfFile:
				return std::make_shared<Token>(lexeme, TokenClass::EndOfFile, this->_row, this->_column);
			case LexicalState::HtmlContent:

				if(lastTwo == "<%")
				{
					this->SetAnalysisMode(AnalysisMode::Pascal);
					return std::make_shared<Token>
							(lexeme.substr(0, lexemeSizeMinusTwo), TokenClass::HtmlContent, this->_row, this->_column);
				}
				else if (this->_currentSymbol.Value != '<'
						 && this->_currentSymbol.Value != nullTerminator)
				{
					lexeme += this->_currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return std::make_shared<Token>(lexeme, TokenClass::HtmlContent, this->_row, this->_column);
				}
				break;
		}
	}
}

void Lexer::SetAnalysisMode(AnalysisMode mode) {
	this->_analysisMode = mode;
}








