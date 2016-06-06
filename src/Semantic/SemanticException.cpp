//
// Created by Hakurei Shrine on 6/4/2016.
//

#include <sstream>
#include "SemanticException.h"
using namespace WebPascal::Semantic;

SemanticException::SemanticException(std::string what)
:_what(what)
{
	std::stringstream message;
	message << name << "occurred. " << std::endl
	<< what << std::endl;
	_what = message.str();
}

void SemanticException::Throw()
{
	throw what();
}



