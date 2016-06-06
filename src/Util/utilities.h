#ifndef SRC_UTILITIES_H
#define SRC_UTILITIES_H

#include <string>
#include <algorithm>
#include <map>
#include <climits>
#include <fstream>
#include <sstream>

namespace Util
{
	namespace String
	{
		static std::string ToLower(std::string str)
		{
			std::string output = str;
			std::transform(output.begin(), output.end(), output.begin(), tolower);
			return output;
		}
	}

	namespace Vector
	{
		template<class T>
		static bool Contains(std::vector<T> list, T what)
		{
			return std::find(list.begin(), list.end(), what) != list.end();
		}
	}
	namespace Map
	{
		template<class TKey, class TValue>
		static bool Contains(std::map<TKey, TValue> map, TKey what)
		{
			return map.find(what) != map.end();
		};
	}
	namespace Char
	{
		const constexpr static size_t MaxSize = CHAR_MAX;
	}
	namespace File
	{
		static std::string LoadTextFile(std::string path)
		{
			std::string s;
			std::stringstream output;
			std::ifstream my_file;
			my_file.open(path);
			if (my_file.is_open())
			{
				while (std::getline(my_file, s))
				{
					output << s << '\n';
				}
				my_file.close();
			}
			else
			{
				std::stringstream exception;
				exception << "The file requested was not found or could not be opened. File location: "
				<< path << std::endl;
				throw exception.str();
			}
			my_file.close();
			return output.str();
		}
	}
}


#endif //SRC_STRINGEXTENSION_H
