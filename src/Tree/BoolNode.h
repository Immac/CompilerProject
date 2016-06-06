//
// Created by Hakurei Shrine on 6/4/2016.
//

#ifndef SRC_BOOLNODE_H
#define SRC_BOOLNODE_H

#include "ExpressionNode.h"
#include "../Lexer/token.h"

namespace WebPascal
{
	namespace Semantic
	{
		class BoolNode : public ExpressionNode
		{
		public:
			BoolNode(std::string value);

			bool GetValue() const;
			void SetValue(bool value);
			void SetValue(std::string value);
			std::string GetName() const;
		private:
			bool _value;
			std::string _name = "bool";
		};
	}
}


#endif //SRC_BOOLNODE_H
