//
// Created by Hakurei Shrine on 6/5/2016.
//

#ifndef SRC_INDEXACCESSOR_H
#define SRC_INDEXACCESSOR_H

#include "Accessor.h"
#include "ExpressionNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class IndexAccessor;

		using IndexAccessorUptr = std::unique_ptr<IndexAccessor>;

		class IndexAccessor : public Accessor
		{
		public:
			virtual ~IndexAccessor() override
			{ }

			IndexAccessor(ExpressionNode *expressionNode);

			Semantic::ExpressionNodeUptr Expression;
		};


	}
}


#endif //SRC_INDEXACCESSOR_H
