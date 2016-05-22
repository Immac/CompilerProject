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
			const std::string & GetSourceCode() const;

		private:
			std::string _sourceCode;
			unsigned _row = 0;
			unsigned _col = 0;
			unsigned _index = 0;

			void UpdatePosition(auto doNewLine);
		};
	}
}

#endif // SOURCESTREAM_H
