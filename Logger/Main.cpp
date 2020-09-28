#include <iostream>
#include "Logger.h"

int main()
{
	Logger log("test.txt", true, true, true);
	log.Log("test", "main");

	return 0;
}