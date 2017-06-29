#include "wpa_ctrl.h"
#include "build_config.h"
#include "common.h"
#include "includes.h"
#include "os.h"  
#include "wpabuf.h"
#include "wpa_debug.h"

#include <unistd.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>


typedef unsigned int size_t;


struct wpa_ctrl * ctrl_conn = NULL;

struct wpa_ctrl * mon_conn = NULL;

const char* client_socket_dir = "/var/run/wpa_supplicant";

const char* ctrl_iface_dir = "/sys/class/net";

// ctrl_iface_dir 

int interactive = 0;

int wpa_cli_attached = 0;

static char *ifname_prefix = NULL;


static void update_ifnames(struct wpa_ctrl *ctrl)
{
	char buf[4096];
	size_t len = sizeof(buf);
	int ret;
	char *cmd = "INTERFACES";
	char *pos, *end;
	char txt[200];

	cli_txt_list_flush(&ifnames);

	if (ctrl == NULL)
		return;
	ret = wpa_ctrl_request(ctrl, cmd, os_strlen(cmd), buf, &len, NULL);
	if (ret < 0)
		return;
	buf[len] = '\0';

	pos = buf;
	while (pos) {
		end = os_strchr(pos, '\n');
		if (end == NULL)
			break;
		*end = '\0';
		ret = os_snprintf(txt, sizeof(txt), "ifname=%s", pos);
		if (!os_snprintf_error(sizeof(txt), ret))
			cli_txt_list_add(&ifnames, txt);
		pos = end + 1;
	}
}

static int wpa_cli_open_connection(const char *ifname, int attach)
{
#if defined(CONFIG_CTRL_IFACE_UDP) || defined(CONFIG_CTRL_IFACE_NAMED_PIPE)
	ctrl_conn = wpa_ctrl_open(ifname);
	if (ctrl_conn == NULL)
		return -1;

	if (attach && interactive)
		mon_conn = wpa_ctrl_open(ifname);
	else
		mon_conn = NULL;
#else /* CONFIG_CTRL_IFACE_UDP || CONFIG_CTRL_IFACE_NAMED_PIPE */
	char *cfile = NULL;
	int flen, res;

	if (ifname == NULL)
		return -1;

#ifdef ANDROID
	if (access(ctrl_iface_dir, F_OK) < 0) {
		cfile = os_strdup(ifname);
		if (cfile == NULL)
			return -1;
	}
#endif /* ANDROID */

	if (client_socket_dir && client_socket_dir[0] &&
		access(client_socket_dir, F_OK) < 0) {
		perror(client_socket_dir);
		os_free(cfile);
		return -1;
	}

	if (cfile == NULL) {
		flen = os_strlen(ctrl_iface_dir) + os_strlen(ifname) + 2;
		cfile = os_malloc(flen);
		if (cfile == NULL)
			return -1;
		res = os_snprintf(cfile, flen, "%s/%s", ctrl_iface_dir,
			ifname);
		if (os_snprintf_error(flen, res)) {
			os_free(cfile);
			return -1;
		}
	}

	ctrl_conn = wpa_ctrl_open2(cfile, client_socket_dir);
	if (ctrl_conn == NULL) {
		os_free(cfile);
		return -1;
	}

	if (attach && interactive)
		mon_conn = wpa_ctrl_open2(cfile, client_socket_dir);
	else
		mon_conn = NULL;
	os_free(cfile);
#endif /* CONFIG_CTRL_IFACE_UDP || CONFIG_CTRL_IFACE_NAMED_PIPE */

	if (mon_conn) {
		if (wpa_ctrl_attach(mon_conn) == 0) {
			wpa_cli_attached = 1;
		}
		else {
			printf("Warning: Failed to attach to "
				"wpa_supplicant.\n");
			wpa_cli_close_connection();
			return -1;
		}
	}

	return 0;
}

static void wpa_cli_msg_cb(char *msg, size_t len)
{
	printf("%s\n", msg);
}

static int _wpa_ctrl_command(struct wpa_ctrl *ctrl, char *cmd, int print)
{
	char buf[4096];
	size_t len;
	int ret;

	if (ctrl_conn == NULL) {
		printf("Not connected to wpa_supplicant - command dropped.\n");
		return -1;
	}
	if (ifname_prefix) {
		os_snprintf(buf, sizeof(buf), "IFNAME=%s %s",
			ifname_prefix, cmd);
		buf[sizeof(buf) - 1] = '\0';
		cmd = buf;
	}
	len = sizeof(buf) - 1;
	ret = wpa_ctrl_request(ctrl, cmd, os_strlen(cmd), buf, &len,
		wpa_cli_msg_cb);
	if (ret == -2) {
		printf("'%s' command timed out.\n", cmd);
		return -2;
	}
	else if (ret < 0) {
		printf("'%s' command failed.\n", cmd);
		return -1;
	}
	if (print) {
		buf[len] = '\0';
		printf("%s", buf);
		if (interactive && len > 0 && buf[len - 1] != '\n')
			printf("\n");
	}
	return 0;
}

static void wpa_cli_close_connection(void)
{
	if (ctrl_conn == NULL)
		return;

	if (wpa_cli_attached) {
		wpa_ctrl_detach(interactive ? mon_conn : ctrl_conn);
		wpa_cli_attached = 0;
	}
	wpa_ctrl_close(ctrl_conn);
	ctrl_conn = NULL;
	if (mon_conn) {
		wpa_ctrl_close(mon_conn);
		mon_conn = NULL;
	}
}

static int wpa_ctrl_command(struct wpa_ctrl *ctrl, char *cmd)
{
	return _wpa_ctrl_command(ctrl, cmd, 1);
}

int write_cmd(char *buf, size_t buflen, const char *cmd, int argc, char *argv[])
{
	int i, res;
	char *pos, *end;

	pos = buf;
	end = buf + buflen;

	res = os_snprintf(pos, end - pos, "%s", cmd);
	if (os_snprintf_error(end - pos, res))
		goto fail;
	pos += res;

	for (i = 0; i < argc; i++) {
		res = os_snprintf(pos, end - pos, " %s", argv[i]);
		if (os_snprintf_error(end - pos, res))
			goto fail;
		pos += res;
	}

	buf[buflen - 1] = '\0';
	return 0;

fail:
	printf("Too long command\n");
	return -1;
}

static int wpa_cli_cmd(struct wpa_ctrl *ctrl, const char *cmd, int min_args,
	int argc, char *argv[])
{
	char buf[4096];
	if (argc < min_args) {
		printf("Invalid %s command - at least %d argument%s "
			"required.\n", cmd, min_args,
			min_args > 1 ? "s are" : " is");
		return -1;
	}
	if (write_cmd(buf, sizeof(buf), cmd, argc, argv) < 0)
		return -1;

	return wpa_ctrl_command(ctrl, buf);
}

static int wpa_cli_cmd_scan(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	return wpa_cli_cmd(ctrl, "SCAN", 0, argc, argv);
}


static int wpa_cli_cmd_scan_results(struct wpa_ctrl *ctrl, int argc,
	char *argv[])
{
	return wpa_ctrl_command(ctrl, "SCAN_RESULTS");
}

// 测试基本的用法
int main()
{
	const char* ctrl_ifname = "wls35u1";

	// 1、连接到wpa_supplicant

	struct wpa_ctrl * ctrl_conn = wpa_ctrl_open("/var/run/wpa_supplicant");
	update_ifnames(ctrl_conn);

	if (wpa_cli_open_connection(ctrl_ifname, 0) < 0)
	{
		fprintf(stderr, "Failed to connect to non-global "
			"ctrl_ifname: %s  error: %s\n",
			ctrl_ifname ? ctrl_ifname : "(nil)",
			strerror(errno));
		return -1;
	}


	// 2、执行扫描命令
	wpa_cli_cmd_scan(ctrl_conn, 0, NULL);

	// 3、选择热点，进行连接

}