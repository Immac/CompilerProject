//
// Created by Hakurei Shrine on 6/4/2016.
//

#ifndef SRC_REALNODE_H
#define SRC_REALNODE_H

#include <string>
#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class RealNode : public ExpressionNode
		{
		public:

			RealNode(std::string value);

			double GetValue() const;
			void SetValue(double value);
			void SetValue(std::string value);
		private:
			double value;
		};
	}
}


#endif //SRC_REALNODE_H
