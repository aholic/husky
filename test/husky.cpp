#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "src/HuskyServer.hpp"
#include "src/Limonp/ArgvContext.hpp"

using namespace Husky;

class ReqHandle: public IRequestHandler
{
	public:
        ReqHandle(){};
        virtual ~ReqHandle(){};
		virtual bool init(){return true;};
		virtual bool dispose(){return true;};
	public:
        virtual bool do_GET(const HttpReqInfo& httpReq, string& strSnd)
        {
            strSnd << httpReq;
            LogInfo(strSnd);
            return true;
        }
};

int main(int argc,char* argv[])
{
    unsigned int port = 11257, threadNum = 8;

    ReqHandle reqh;
    HuskyServer sf(port, threadNum, &reqh);
    return !(sf.init() && sf.run());
}
