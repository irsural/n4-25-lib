#ifndef N4_25_H
#define N4_25_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum dc_polarity_t {
  pos = 0,
  neg = 1
} dc_polarity_t;

typedef enum signal_type_t {
  aci = 0,
  acv = 1,
  dci = 2,
  dcv = 3
} signal_type_t;

int revision(void);

void usb_init(size_t a_data_size);
void usb_deinit(void);
void usb_tick(void);

int usb_devices_changed(void);
void get_usb_devices(char** ap_devices);
void free_usb_devices(char** ap_devices);
int get_usb_status(void);

int connect_usb(const char* ap_device_path);
void disconnect_usb(void);

double get_amplitude(void);
void set_amplitude(double a_amplitude);

double get_frequency(void);
void set_frequency(double a_frequency);

int enabled(void);
void signal_enable(int a_enable);

int get_signal_type(void);
void set_signal_type(int a_signal_type);

int get_polarity(void);
void set_polarity(int a_polarity);

int is_connected(void);
int is_signal_ready(void);

#ifdef __cplusplus
}
#endif

#endif // N4_25_H
