//
// Created by Hakurei Shrine on 6/4/2016.
//

#include <sstream>
#include "CharNode.h"
#include "../Lexer/lexer.h"
#include "../Semantic/SemanticException.h"

using namespace WebPascal::Semantic;

WebPascal::Semantic::CharNode::CharNode(std::string value)
{
	auto mark_position = value.find(Lexical::Lexer::CharSymbol);
	auto temp = value.replace(mark_position, 1,"");
	SetValue(temp);
}

void WebPascal::Semantic::CharNode::SetValue(std::string value)
{
	try
	{
		_value = (char) stoi(value, 0);
	}
	catch (const std::invalid_argument &ia)
	{
		std::stringstream message;
		message << "Value " << value << "is not a valid char" << std::endl;
		SemanticException(message.str()).Throw();
	}
}



