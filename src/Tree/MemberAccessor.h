//
// Created by Hakurei Shrine on 6/5/2016.
//

#ifndef SRC_MEMBERACCESSOR_H
#define SRC_MEMBERACCESSOR_H

#include "Accessor.h"
#include "IdNode.h"

namespace WebPascal
{
	namespace Semantic
	{
		class MemberAccessor : public Accessor
		{
		public:
			MemberAccessor(IdNode *idReference);
			IdNodePtr IdReference;
			virtual ~MemberAccessor() override
			{ }
		};
	}
}


#endif //SRC_MEMBERACCESSOR_H
