# wpa_cli_wrapper
wpa_cli_wrapper 是对 wpa_cli.c 代码进行封装，即把 wpa_cli 命令的相关操作集成到c代码里，并返回操作的结果。

# 编译方法
进入 wpa_cli_wrapper/wpa_supplicant-2.6/wpa_supplicant 目录，执行 make -f Makefile.wrapper, 在当前目录下生成 libwpa_cli_wrapper.a 和 libwpa_cli_wrapper_test；如果遇到提示类似“致命错误：netlink/genl/genl.h：没有那个文件或目录 ”的错误，请安装libnl库重试。

# 如何使用
设置 /etc/wpa_supplicant/wpa_supplicant.conf 的内容为 ctrl_interface=/var/run/wpa_supplicant, 然后执行 wpa_supplicant -B -iwls35u1 -c /etc/wpa_supplicant/wpa_supplicant.conf -P/var/run/wpa_supplicant.pid, wls35u1 是无线网卡名。

libwpa_cli_wrapper.a 静态库封装了wpa_cli的相关操作，接口参考libwpa_cli_wrapper.h头文件，使用请参考libwpa_cli_wrapper_test.c 

# 支持的部分命令
libwpa_cli_wrapper.a 支持 wpa_cli 的部分命令：
1. status
1. ifname
1. scan
1. scan_results
1. select_network
1. add_network
1. list_network
1. set_network 
1. remove_network
1. enable_network
1. disable_network
1. save_config

