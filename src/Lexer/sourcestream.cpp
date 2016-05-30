#include "sourcestream.h"

using namespace WebPascal::Lexical;

SourceStream::SourceStream(std::string _sourceCode)
		: _sourceCode(_sourceCode)
{
}

void SourceStream::UpdatePosition(auto doNewLine)
{
	_col = doNewLine
		   ? 0
		   : _col + 1;
	_row = doNewLine
		   ? _row + 1
		   : _row;
}

Symbol SourceStream::GetNextSymbol()
{

	Symbol symbol{_row, _col, '\0'};
	if (_index >= _sourceCode.length())
	{
		this->UpdatePosition(false);
		return symbol;
	}

	symbol.Value = _sourceCode.at(_index++);
	auto isNewLine = symbol.Value == '\n';
	this->UpdatePosition(isNewLine);

	return symbol;
}

const std::string &SourceStream::GetSourceCode() const
{
	return _sourceCode;
}