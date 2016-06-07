//
// Created by Hakurei Shrine on 6/5/2016.
//

#include <list>
#include "IdNode.h"

WebPascal::Semantic::IdNode::IdNode(std::string basic_string, std::list<Accessor *> &list)
:Value(basic_string),AccessorList(list)
{
}

