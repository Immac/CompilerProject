//
// Created by Hakurei Shrine on 6/4/2016.
//

#ifndef SRC_STRINGNODE_H
#define SRC_STRINGNODE_H

#include <string>
#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class StringNode : public ExpressionNode
		{
		public:
			StringNode(std::string value);

			const std::string &GetValue() const;

			void SetValue(const std::string &value);

		private:
			std::string value;
		};
	}
}


#endif //SRC_STRINGNODE_H
