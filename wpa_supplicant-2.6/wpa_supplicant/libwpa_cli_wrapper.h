#ifndef LIBWPA_CLI2_H_
#define LIBWPA_CLI2_H_


#ifdef __cplusplus
extern "C" {
#endif

int init_wpa_cli();

int wpa_cli_execute(const char* cmd, char* buf, int buf_len);

int deinit_wpa_cli();

#ifdef __cplusplus
}
#endif

#endif
