#ifndef LIBWPA_CLI_WRAPPER_H_
#define LIBWPA_CLI_WRAPPER_H_

// 扫描结果，ssid如果是中文，输出的是16进制的(wpa_supplicant后台服务传回的)，显示需要把16进制转换成字符串
/*
[root@localhost wpa_supplicant]# wpa_cli scan_r
Selected interface 'wls35u1'
bssid / frequency / signal level / flags / ssid
50:fa:84:00:30:4e	2462	-63	[WPA-PSK-CCMP][WPA2-PSK-CCMP][ESS]	\xb9\xfe\xb9\xfe\xbe\xb5
1c:88:79:52:22:cc	2462	-43	[WPA2-PSK-CCMP][ESS]	OpenWrt
ce:2d:83:3d:c6:e5	2417	-55	[WPA2-PSK-CCMP][ESS]	\xe5\x88\x98\xe5\x9d\x87
*/


/**
 * 目前实现了:
 * status, 
 * ifname, 
 * scan, 
 * scan_results, 
 * select_network(untest), 
 * add_network(untest), 
 * list_network(untest), 
 * set_network(untest),
 * remove_network(untest),
 * enable_network(untest),
 * disable_network(untest),
 * save_config(untest),
 */



#ifdef __cplusplus
extern "C" {
#endif

#define WPA_CLI_EXEC_RES_UNIMPLEMENTED -100

int init_wpa_cli();

int wpa_cli_execute(const char* cmd, char* buf, int buf_len);

int deinit_wpa_cli();

#ifdef __cplusplus
}
#endif

#endif
