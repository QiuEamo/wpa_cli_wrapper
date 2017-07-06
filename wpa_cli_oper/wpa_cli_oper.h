#ifndef WPA_CLI_OPER_H_
#define WPA_CLI_OPER_H_

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

#include "smartlist.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
去读取配置文件，如果没有配置连接哪个wifi，就直接定时去扫，扫出来和以前连接过wifi热点进行对比，如果可以连接成功就直接通过这个上线
*/

// 配置连接的wifi
typedef struct WifiConfig
{
	char		ssid[128];
	char		password;
	int			flag; // 0 wep, 1 wpa, 2 无密码
}WifiConfig;


// 扫描到的结果
typedef struct WifiScanItem
{
	char	bssid[18];
	int		frequency;
	int		signal_level;
	char	flags[256];
	char	ssid[256];
}WifiScanItem;

typedef struct WifiStatus
{
	int		wpa_state;		// = INACTIVE
	char	address[18];	// = 8a:02 : 32 : db : b3 : b8
							// ... 
}WifiStatus;

/**
 * @brief 获取wifi配置
 * @return 成功：0，失败:<0
 */
int get_wifi_config(WifiConfig* config);

/**
* @brief 获取连接过的wifi列表
* @return 成功：smartlist_t*<WifiConfig>，失败:NULL
*/
smartlist_t* get_connected_wifi_list();


/**
* @brief 获取扫描到的wifi列表
* @return 成功：smartlist_t*<WifiScanItem>，失败:NULL
*/
smartlist_t* get_wifi_scan_results();

/**
* @brief 连接wifi
* @return 成功：0，失败:<0
*/
int connect_wifi(WifiConfig* config);

/**
* @brief 断开wifi
* @return 成功：0，失败:<0
*/
int disconnect_wifi(WifiConfig* config);


/**
* @brief 断开wifi
* @return 成功：0，失败:<0
*/
int get_wifi_status(WifiStatus* status);


#ifdef __cplusplus
}
#endif

#endif
