#include "src/http_core.h"
#include "routes/timer.h"
#include "routes/index.h"

int main()
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	initializeHashTable();
    insert("timer", timer_handler);
    insert("", index_route);
	insert("public", public_route);
	launch_server();

	return 0;
}
