
#include "libwpa_cli_wrapper.h"
#include <stdio.h>

int main()
{
	int res = init_wpa_cli();
	if (res < 0)
	{
		printf("init wpa client failed!\n");
		return -1;
	}


	char buf[1024];
	res = wpa_cli_execute("status", buf, sizeof(buf));
	if (res <= 0)
	{
		printf("wpa_cli_execute failed!\n");
		return -1;
	}

	deinit_wpa_cli();

	return res;
}


