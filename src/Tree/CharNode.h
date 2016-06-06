//
// Created by Hakurei Shrine on 6/4/2016.
//

#ifndef SRC_CHARNODE_H
#define SRC_CHARNODE_H

#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class CharNode : public ExpressionNode
		{
		public:
			CharNode(std::string value);
			void SetValue(std::string value);
			char GetValue() const;
		private:
			char _value;
		};
	}
}


#endif //SRC_CHARNODE_H
