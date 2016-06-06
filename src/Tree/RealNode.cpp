//
// Created by Hakurei Shrine on 6/4/2016.
//

#include <stdexcept>
#include <sstream>
#include "RealNode.h"
#include "../Semantic/SemanticException.h"

double WebPascal::Semantic::RealNode::GetValue() const
{
	return value;
}

void WebPascal::Semantic::RealNode::SetValue(double value)
{
	RealNode::value = value;
}

void WebPascal::Semantic::RealNode::SetValue(std::string value)
{
	try
	{
		this->value = stod(value,0);
	}
	catch (const std::invalid_argument &ia)
	{
		std::stringstream message;
		message << "Value " << value << "is not a valid integer" << std::endl;
		throw SemanticException(message.str());
	}
}

WebPascal::Semantic::RealNode::RealNode(std::string value)
{
	this->SetValue(value);
}



