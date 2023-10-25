
#include "uuid.h"
#include <time.h>
#include <cstdlib>

void uuid::init()
{
	// Initializing random
	srand(time(NULL));
}

uuid::uuid()
{
	value.a = rand();
	value.b = rand();
	value.c = rand();
	value.d = rand();
}
