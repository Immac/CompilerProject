//
// Created by Hakurei Shrine on 6/1/2016.
//

#ifndef SRC_UNARYOPERATORNODE_H
#define SRC_UNARYOPERATORNODE_H

#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class UnaryOperatorNode : public ExpressionNode
		{
			ExpressionNodeUptr Operand;
		};
	}
}


#endif //SRC_UNARYOPERATORNODE_H
