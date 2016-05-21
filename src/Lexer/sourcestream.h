#ifndef SOURCESTREAM_H
#define SOURCESTREAM_H
#include <string>
#include <memory>
#include "symbol.h"

namespace WebPascal {

	namespace Lexical {

		class SourceStream
		{
			public:
				SourceStream(std::string _sourceCode);

				Symbol GetNextSymbol();
				Symbol PeekNextSymbol();
			private:
				void UpdatePosition(auto doNewLine);
				std::string _sourceCode;
				unsigned _row = 0;
				unsigned _col = 0;
				unsigned _index = 0;
		};
	}
}

#endif // SOURCESTREAM_H
