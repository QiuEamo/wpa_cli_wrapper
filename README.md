# wpa_cli_wrapper
wpa_cli 封装，可以在代码里直接调用wpa_cli命令的相关操作相关的操作

使用方法：进入wpa_cli_wrapper/wpa_supplicant-2.6/wpa_supplicant目录

执行：make -f Makefile.wrapper, 在当前目录下生成 libwpa_cli_wrapper.a 和 libwpa_cli_wrapper_test

libwpa_cli_wrapper_test 支持 wpa_cli 的 部分命令，

        status, ifname, scan, scan_results,
        select_network, add_network, list_network, set_network, remove_network,
        enable_network, disable_network, save_config,

