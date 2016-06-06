//
// Created by Hakurei Shrine on 6/5/2016.
//

#ifndef SRC_IDNODE_H
#define SRC_IDNODE_H

#include <vector>
#include <string>
#include <list>
#include "Accessor.h"
#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class IdNode;
		using IdNodePtr = std::unique_ptr<IdNode>;
		class IdNode : public ExpressionNode
		{
		public:
			IdNode(std::string basic_string, std::list<Accessor *> list);

			std::list<Accessor *> AccessorList;
			std::string Value;
		};
	}
}


#endif //SRC_IDNODE_H
