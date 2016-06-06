//
// Created by Hakurei Shrine on 6/3/2016.
//

#ifndef SRC_INTEGERNODE_H
#define SRC_INTEGERNODE_H

#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class IntegerNode : public ExpressionNode
		{
		public:
			IntegerNode(std::string value);
			void SetValue(const std::string &value, int base);
			int GetValue() const;
		private:
			int value;

		};
	}
}


#endif //SRC_INTEGERNODE_H
