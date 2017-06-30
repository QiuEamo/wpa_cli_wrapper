
#include "libwpa_cli_wrapper.h"
#include <stdio.h>
#include <string.h>

int main()
{
	int res = init_wpa_cli();
	if (res < 0)
	{
		printf("init wpa client failed!\n");
		return -1;
	}
	

	char buf[4096] = {0};
	
	char cmd[1024] = {0};
	
	while (scanf("%s", cmd) == 1)
	{
		res = wpa_cli_execute(cmd, buf, sizeof(buf));
		if (res < 0)
		{
			printf("wpa_cli_execute %s failed!\n", cmd);
			continue;
		}
		else if (res == WPA_CLI_EXEC_RES_UNIMPLEMENTED)
		{
			printf("not implemented!\n");
			continue;
		}
		
		printf(">%s\n", buf);
		
		memset(cmd, 0x00, sizeof(cmd));
		memset(buf, 0x00, sizeof(buf));
	}
	
	

	deinit_wpa_cli();

	return res;
}


