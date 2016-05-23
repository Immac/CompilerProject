#include "sourcestream.h"
using namespace WebPascal::Lexical;

SourceStream::SourceStream(std::string _sourceCode): _sourceCode(_sourceCode)
{
}

void SourceStream::UpdatePosition(auto doNewLine)
{
	this->_col = doNewLine
			? 0
			: _col + 1;
	this->_row = doNewLine
			? _row + 1
			: 0;
}

Symbol SourceStream::GetNextSymbol()
{

	Symbol symbol{_row,_col,'\0'};
	auto sourceCodeLength = _sourceCode.length();
	if(_index >= sourceCodeLength)
	{
		return symbol;
	}

	symbol.Value = _sourceCode[_index++];
	auto isNewLine = symbol.Value == '\n';
	this->UpdatePosition(isNewLine);

	return symbol;
}

const std::string &SourceStream::GetSourceCode() const {
	return _sourceCode;
}