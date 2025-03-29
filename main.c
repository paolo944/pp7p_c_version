#include "src/http_core.h"
#include "routes/timer.h"

int main()
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	HashTable table;
	initializeHashTable(&table);
    insert(&table, "timer", timer_handler);

	launch_server(&table);

	freeHashTable(&table);

	return 0;
}
