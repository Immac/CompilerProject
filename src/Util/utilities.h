#ifndef SRC_UTILITIES_H
#define SRC_UTILITIES_H

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

	template <class T>
	static bool Contains(std::vector<T> list, T what)
	{
		return std::find(list.begin(),list.end(),what) != list.end();
	}
	
}


#endif //SRC_STRINGEXTENSION_H
