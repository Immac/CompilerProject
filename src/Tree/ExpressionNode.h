//
// Created by Hakurei Shrine on 6/1/2016.
//

#ifndef SRC_EXPRESSIONNODE_H
#define SRC_EXPRESSIONNODE_H

#include <memory>

namespace WebPascal
{
	namespace Semantic
	{
		class ExpressionNode
		{
		public:
			ExpressionNode() = default;

			virtual  ~ExpressionNode() = 0;
		};

		using ExpressionNodeUptr = std::unique_ptr<ExpressionNode>;
	}

}


#endif //SRC_EXPRESSIONNODE_H
