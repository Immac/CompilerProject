//
// Created by Hakurei Shrine on 6/5/2016.
//

#include "IndexAccessor.h"
using namespace WebPascal::Semantic;

IndexAccessor::IndexAccessor(ExpressionNode *expressionNode)
{
	this->Expression.reset(expressionNode);
}

