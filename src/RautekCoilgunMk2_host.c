#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main() {
	/* Setup your example here, code that should run once
	 */
	openlog("RTCGMk2", LOG_PID | LOG_CONS, LOG_USER);

	/* Code in this loop will run repeatedly
	 */
	for (;;) {

	}

	return 0;
}
