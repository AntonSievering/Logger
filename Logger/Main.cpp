#include "Logger.h"


int main()
{
	Logger log = Logger(new StdOutDest(), new TimestampFormat<Timestamp::BOTH>());
	log.log("test");

	return 0;
}
