//
// Created by Hakurei Shrine on 6/4/2016.
//

#include "BoolNode.h"
#include "../Util/utilities.h"
#include "../Semantic/SemanticException.h"
#include "../Lexer/token.h"

void WebPascal::Semantic::BoolNode::SetValue(bool value)
{
	BoolNode::_value = value;
}

bool WebPascal::Semantic::BoolNode::GetValue() const
{
	return _value;
}

WebPascal::Semantic::BoolNode::BoolNode(std::string value)
{

}

void WebPascal::Semantic::BoolNode::SetValue(std::string value)
{
	value = Util::String::ToLower(value);
	if(value == "true")
		this->_value = true;
	if(value == "false")
		this->_value = false;

	std::stringstream what;
	what << "Cannot assign " << value << " to " << this->GetName();
	Semantic::SemanticException(what.str()).Throw();
}

std::string WebPascal::Semantic::BoolNode::GetName() const
{
	return _name;
}





