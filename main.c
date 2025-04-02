#include "src/http_core.h"
#include "routes/timer.h"
#include "routes/index.h"

int main()
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	HashTable table;
	initializeHashTable(&table);
    insert(&table, "timer", timer_handler);
    insert(&table, "", index_route);
    insert(&table, "scripts.min.js", index_route);
    insert(&table, "style.min.css", index_route);
    insert(&table, "favicon.png", index_route);
    insert(&table, "installHook.js.map", index_route);

	launch_server(&table);

	freeHashTable(&table);

	return 0;
}
