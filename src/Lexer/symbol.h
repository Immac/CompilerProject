#ifndef SYMBOL_H
#define SYMBOL_H
#include <map>
namespace WebPascal {
	namespace Lexical {
		class Symbol
		{
			public:
				unsigned Row;
				unsigned Column;
				char Value;
		};
	}
}
#endif // SYMBOL_H
