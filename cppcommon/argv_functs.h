/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
************************************/

#ifndef CPPCOMMON_ARGV_FUNCTS_H
#define CPPCOMMON_ARGV_FUNCTS_H

#include <set>
#include <sstream>
#include "str_functs.h"
#include "map_functs.h"
#include "vec_functs.h"

namespace CPPCOMMON
{
	using namespace std;
	bool getArgvMap(int argc, const char* const* argv, map<string, string>& mpss);
	class ArgvContext
	{
		public:
			ArgvContext(int argc, const char* const * argv);
			~ArgvContext();
		public:
			string toString();
			string operator [](uint i);
			string operator [](const string& key);
		public:
			bool isKeyExist(const string& key);
			uint getSize() const;
		private:
			vector<string> _args;
			map<string, string> _mpss;
			set<string> _sset; 

	};
}

#endif
