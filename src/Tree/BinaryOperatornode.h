//
// Created by Hakurei Shrine on 6/1/2016.
//

#ifndef SRC_BINARYOPERATORNODE_H
#define SRC_BINARYOPERATORNODE_H

#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class BinaryOperatorNode : public ExpressionNode
		{
		public:
			ExpressionNodeUptr LeftOperand;
			ExpressionNodeUptr RightOperand;
		};
	}
}


#endif //SRC_BINARYOPERATORNODE_H
