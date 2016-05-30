#include "lexer.h"
#include "sstream"
#include "../Util/utilities.h"


using namespace WebPascal::Lexical;

Lexer::Lexer(std::string content) : SourceCodeStream(content)
{
	_currentSymbol = this->SourceCodeStream.GetNextSymbol();
}

Token Lexer::GetNextToken()
{
	switch (_analysisMode)
	{
		case AnalysisMode::Html:
			return GetHtmlToken();
		case AnalysisMode::Pascal:
			return GetPascalToken();
	}
}

void Lexer::UpdatePosition()
{
	_column = _currentSymbol.Column;
	_row = _currentSymbol.Row;
}

void Lexer::ConsumeSymbol()
{
	_currentSymbol = this->SourceCodeStream.GetNextSymbol();
}

Token Lexer::GetPascalToken()
{
	std::string lexeme = "";
	auto state = LexicalState::Initial;
	this->UpdatePosition();
	while (true)
	{
		switch (state)
		{
			case LexicalState::Initial:
			{
				if (isspace(_currentSymbol.Value))
				{
					state = LexicalState::Initial;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == nullTerminator)
				{
					state = LexicalState::EndOfFile;
					lexeme = "@";
					this->UpdatePosition();
				}
				else if (isalpha(_currentSymbol.Value))
				{
					state = LexicalState::Id;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (isdigit(_currentSymbol.Value))
				{
					state = LexicalState::IntegerLiteralDecimal;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == '&')
				{
					state = LexicalState::IntegerLiteralOctal;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == '$')
				{
					state = LexicalState::IntegerLiteralHexadecimalSingle;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == '#')
				{
					state = LexicalState::CharLiteral;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == '\"')
				{
					state = LexicalState::StringLiteralDoubleQuote;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == '\'')
				{
					state = LexicalState::StringLiteralSingleQuote;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == '%')
				{
					state = LexicalState::SwitchToHtmlAnalysis;
				}
				else if (Util::Vector::Contains<char>(this->UnambiguousPunctuation, _currentSymbol.Value))
				{
					state = LexicalState::UnambiguousPunctuationState;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (Util::Vector::Contains<char>(this->AmbiguousPunctuationStartSymbols, _currentSymbol.Value))
				{
					state = LexicalState::AmbiguousPunctuation;
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
			}
				break;
			case LexicalState::Id:
				if (isalnum(_currentSymbol.Value))
				{
					state = LexicalState::Id;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					auto lexemeToLower = Util::String::ToLower(lexeme);
					auto type = Util::Map::Contains(this->ReservedWords, lexemeToLower)
								? this->ReservedWords[lexemeToLower]
								: TokenClass::Id;

					return Token(lexemeToLower,type,_row,_column);
				}
				break;
			case LexicalState::IntegerLiteralDecimal:
				if (isdigit(_currentSymbol.Value))
				{
					state = LexicalState::IntegerLiteralDecimal;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (_currentSymbol.Value == '.')
				{
					state = LexicalState::RealLiteral;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return Token(lexeme, TokenClass::IntegerLiteralDecimal, _row, _column);
				}
				break;
			case LexicalState::RealLiteral:
				if (isdigit(_currentSymbol.Value))
				{
					state = LexicalState::RealLiteral;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return Token(lexeme, TokenClass::RealLiteral, _row, _column);
				}
				break;
			case LexicalState::IntegerLiteralHexadecimalSingle:
				if (_currentSymbol.Value <= Util::Char::MaxSize
					&& Util::Vector::Contains<char>(this->ValidHexValues, tolower(_currentSymbol.Value)))
				{
					state = LexicalState::IntegerLiteralHexadecimalDigits;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					ThrowLexicalError("Expected a hexadecimal digit");
				}
				break;
			case LexicalState::IntegerLiteralHexadecimalDigits:
				if (_currentSymbol.Value <= Util::Char::MaxSize
					&& Util::Vector::Contains<char>(ValidHexValues, tolower(_currentSymbol.Value)))
				{
					state = LexicalState::IntegerLiteralHexadecimalDigits;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return Token(Util::String::ToLower(lexeme), TokenClass::IntegerLiteralHexadecimal, _row,
								 _column);
				}
				break;
			case LexicalState::IntegerLiteralOctal:
				if (_currentSymbol.Value <= Util::Char::MaxSize
					&& Util::Vector::Contains<char>(ValidOctValues, tolower(_currentSymbol.Value)))
				{
					state = LexicalState::IntegerLiteralOctal;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (lexeme.length() > 1)
				{
					return Token(Util::String::ToLower(lexeme), TokenClass::IntegerLiteralOctal, _row, _column);
				}
				else
				{
					ThrowLexicalError("Expected an octal digit"); // TODO: got <symbol>, lexeme: lexeme
				}
				break;
			case LexicalState::CharLiteral:
				if (lexeme.size() > 4)
				{
					ThrowLexicalError("Expected length exceeded"); // TODO: got <symbol>, lexeme: lexeme
				}
				if (isdigit(_currentSymbol.Value))
				{
					state = LexicalState::CharLiteral;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (lexeme.length() > 1)
				{
					return Token(Util::String::ToLower(lexeme), TokenClass::CharLiteral, _row, _column);
				}
				else
				{
					ThrowLexicalError("Expected a digit"); // TODO: got <symbol>, lexeme: lexeme
				}
				break;
			case LexicalState::StringLiteralDoubleQuote:
				if (_currentSymbol.Value == '\"')
				{
					state = LexicalState::StringLiteralClosingDoubleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (std::isprint(_currentSymbol.Value))
				{
					state = LexicalState::StringLiteralDoubleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					ThrowLexicalError("Expected a closing double quote."); // Todo: classes for exceptions
				}

				break;
			case LexicalState::StringLiteralClosingDoubleQuote:
				if (_currentSymbol.Value == '\"')
				{
					state = LexicalState::StringLiteralEscapedDoubleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return Token(lexeme, TokenClass::StringLiteralDoubleQuote, _row, _column);
				}
				break;
			case LexicalState::StringLiteralEscapedDoubleQuote:
				if (_currentSymbol.Value == '\"')
				{
					state = LexicalState::StringLiteralClosingDoubleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (std::isprint(_currentSymbol.Value))
				{
					state = LexicalState::StringLiteralDoubleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					ThrowLexicalError("Expected a closing double quote."); // Todo: classes for exceptions
				}
				break;
			case LexicalState::StringLiteralSingleQuote:
				if (_currentSymbol.Value == '\'')
				{
					state = LexicalState::StringLiteralClosingSingleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (std::isprint(_currentSymbol.Value))
				{
					state = LexicalState::StringLiteralSingleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					ThrowLexicalError("Expected a closing single quote."); // Todo: classes for exceptions
				}

				break;
			case LexicalState::StringLiteralClosingSingleQuote:
				if (_currentSymbol.Value == '\'')
				{
					state = LexicalState::StringLiteralEscapedSingleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return Token(lexeme, TokenClass::StringLiteralSingleQuote, _row, _column);
				}
				break;
			case LexicalState::StringLiteralEscapedSingleQuote:
				if (_currentSymbol.Value == '\'')
				{
					state = LexicalState::StringLiteralClosingSingleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (std::isprint(_currentSymbol.Value))
				{
					state = LexicalState::StringLiteralSingleQuote;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					ThrowLexicalError("Expected a closing Single quote."); // Todo: classes for exceptions
				}

			case LexicalState::UnambiguousPunctuationState:
				if (lexeme == OpenCommentCurlyBrace)
				{
					state = LexicalState::CurlyBraceOpenedCommentBody;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else if (lexeme == OpenCommentParenthesis)
				{
					state = LexicalState::ParenthesisOpenedCommentBody;
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return Token(lexeme, Punctuation.at(lexeme), _row, _column);
				}
				break;
			case LexicalState::CurlyBraceOpenedCommentBody:
			{
				auto size = lexeme.length();
				auto lastChar = lexeme.substr(size - 1);
				lexeme += _currentSymbol.Value;
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
				auto lastChar = lexeme.substr(size - 2);
				lexeme += _currentSymbol.Value;
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
				auto nextSymbol = _currentSymbol;
				auto nextLexeme = lexeme;
				auto isEndOfFile = nextSymbol.Value == nullTerminator;
				if (!isEndOfFile)
				{
					nextLexeme += nextSymbol.Value;
				}

				auto nextLexemeIsContained = this->Punctuation.find(nextLexeme) != this->Punctuation.end();
				if (!isEndOfFile && nextLexemeIsContained)
				{
					state = LexicalState::AmbiguousPunctuation;
					lexeme += _currentSymbol.Value;
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
				if (_currentSymbol.Value == '>')
				{
					//TODO: Ignore this token and return the enxt one immediately
					this->SetAnalysisMode(AnalysisMode::Html);
					this->ConsumeSymbol();
					return GetHtmlToken();
				}
				else
				{
					ThrowLexicalError("Expected '>'");
				}
				break;
			case LexicalState::EndOfFile:
				//TODO: Remove Temporary EOF, the actual thing should never end inside the Pascal code
				return Token(lexeme, TokenClass::EndOfFile, _row, _column);
		}
	}
}

Token Lexer::GetHtmlToken()
{
	std::string lexeme = "";
	auto state = LexicalState::Initial;
	this->UpdatePosition();
	while (true)
	{
		size_t lexemeSize = lexeme.size();
		size_t lexemeSizeMinusTwo = lexemeSize - 2;
		std::string lastTwo = lexemeSize >= 2
							  ? lexeme.substr(lexemeSizeMinusTwo)
							  : "";
		switch (state)
		{
			case LexicalState::Initial:
				if (isspace(_currentSymbol.Value))
				{
					state = LexicalState::Initial;
					this->ConsumeSymbol();
				}
				else if (lastTwo == OpenPascalCode)
				{
					this->SetAnalysisMode(AnalysisMode::Pascal);
					return Token
							(lexeme.substr(0, lexemeSizeMinusTwo), TokenClass::HtmlContent, _row, _column);
				}
				else if (_currentSymbol.Value == nullTerminator)
				{
					state = LexicalState::EndOfFile;
					this->UpdatePosition();
					lexeme = "@";
				}
				else if (lexemeSize == 0 && (_currentSymbol.Value == '<'))
				{
					this->UpdatePosition();
					lexeme += _currentSymbol.Value;
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
				lexeme += _currentSymbol.Value;
				bool isClosingTag = false;

				if (tolower(_currentSymbol.Value) == '/')
				{
					this->ConsumeSymbol();
					lexeme += _currentSymbol.Value;
					isClosingTag = true;
				}
				if (tolower(_currentSymbol.Value) != 'h')
				{
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += _currentSymbol.Value;
				if (tolower(_currentSymbol.Value) != 't')
				{
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += _currentSymbol.Value;
				if (tolower(_currentSymbol.Value) != 'm')
				{
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += _currentSymbol.Value;
				if (tolower(_currentSymbol.Value) != 'l')
				{
					this->ConsumeSymbol();
					state = LexicalState::HtmlContent;
					break;
				}
				this->ConsumeSymbol();
				lexeme += _currentSymbol.Value;
				if (tolower(_currentSymbol.Value) != '>')
				{
					this->ConsumeSymbol();
					state = LexicalState::Initial;
					break;
				}
				ConsumeSymbol();
				if (isClosingTag)
				{
					return Token(Util::String::ToLower(lexeme), TokenClass::HtmlCloseTag, _row, _column);
				}
				return Token(Util::String::ToLower(lexeme), TokenClass::HtmlOpenTag, _row, _column);
			}
			case LexicalState::EndOfFile:
				return Token(lexeme, TokenClass::EndOfFile, _row, _column);
			case LexicalState::HtmlContent:

				if (lastTwo == "<%")
				{
					this->SetAnalysisMode(AnalysisMode::Pascal);
					return Token(lexeme.substr(0, lexemeSizeMinusTwo), TokenClass::HtmlContent, _row,
								 _column);
				}
				else if (_currentSymbol.Value != '<'
						 && _currentSymbol.Value != nullTerminator)
				{
					lexeme += _currentSymbol.Value;
					this->ConsumeSymbol();
				}
				else
				{
					return Token(lexeme, TokenClass::HtmlContent, _row, _column);
				}
				break;
		}
	}
}

void Lexer::SetAnalysisMode(AnalysisMode mode)
{
	this->_analysisMode = mode;
}

void Lexer::ThrowLexicalError(std::string what)
{
	std::stringstream message;
	std::string errorType = "Lexical Exception occurred: ";
	message << errorType << what << std::endl
	<< "at col: " << _column << " line: " << _row << std::endl
	<< "got: " << _currentSymbol.Value << " instead " << std::endl;
	throw message.str();
}