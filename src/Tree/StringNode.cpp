//
// Created by Hakurei Shrine on 6/4/2016.
//

#include "StringNode.h"

WebPascal::Semantic::StringNode::StringNode(std::string value)
{
	this->SetValue(value);
}

const std::string &WebPascal::Semantic::StringNode::GetValue() const
{
	return value;
}

void WebPascal::Semantic::StringNode::SetValue(const std::string &value)
{
	StringNode::value = value;
}