//
// Created by Hakurei Shrine on 6/3/2016.
//

#include <sstream>
#include "IntegerNode.h"
#include "../Semantic/SemanticException.h"
#include "../Lexer/lexer.h"

using namespace WebPascal::Semantic;

IntegerNode::IntegerNode(std::string value)
{
	const unsigned int hexPosition = value.find(Lexical::Lexer::HexSymbol);
	auto isHex = hexPosition != std::string::npos;

	const unsigned int octPosition = value.find(Lexical::Lexer::OctalSymbol);
	auto isOctal = octPosition != std::string::npos;

	if (isHex)
	{
		value = value.replace(hexPosition, sizeof(hexPosition), "");
		SetValue(value,16);
		return;
	}

	if (isOctal)
	{
		value = value.replace(octPosition, sizeof(octPosition), "");
		SetValue(value,8);
		return;
	}

	SetValue(value,10);
}

void IntegerNode::SetValue(const std::string &value, int base)
{
	try
	{
		this->value = stoi(value,0,base);
	}
	catch (const std::invalid_argument &ia)
	{
		std::stringstream message;
		message << "Value " << value << "is not a valid integer" << std::endl;
		throw SemanticException(message.str());
	}
}

int IntegerNode::GetValue() const
{
	return value;
}



