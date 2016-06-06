//
// Created by Hakurei Shrine on 6/4/2016.
//

#ifndef SRC_SEMANTICEXCEPTION_H
#define SRC_SEMANTICEXCEPTION_H

#include <exception>
#include <string>

namespace WebPascal
{
	namespace Semantic
	{
		class SemanticException
		{

		public:

			SemanticException(std::string what);

			const char *what() const
			{
				return _what.c_str();
			}

			void Throw();

		private:
			std::string _what;
			const std::string name = "Semantic Exception";
		};

	}
}


#endif //SRC_SEMANTICEXCEPTION_H
