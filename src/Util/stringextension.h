#ifndef SRC_STRINGEXTENSION_H
#define SRC_STRINGEXTENSION_H

#include <string>
#include <algorithm>
namespace Util
{
	static std::string ToLower (std::string str)
	{
		std::string output = str;
		std::transform(output.begin(),output.end(),output.begin(), tolower);
		return output;
	}
}


#endif //SRC_STRINGEXTENSION_H
