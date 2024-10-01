#ifndef N4_25_API_H
#define N4_25_API_H

#include <stdint.h>


enum dc_polarity_t {
  pos = 0,
  neg = 1
};

enum signal_type_t {
  aci = 0,
  acv = 1,
  dci = 2,
  dcv = 3
};

typedef int (*revision_func)();

typedef void (*usb_init_func)(size_t a_data_size);

typedef void (*usb_deinit_func)();
typedef void (*usb_tick_func)();

typedef int (*usb_devices_changed_func)();
typedef void (*get_usb_devices_func)(char** ap_devices);
typedef void (*free_usb_devices_func)(char** ap_devices);
typedef int (*get_usb_status_func)();

typedef int (*connect_usb_func)(const char *ap_device_path);
typedef void (*disconnect_usb_func)();


typedef double (*get_amplitude_func)();
typedef void (*set_amplitude_func)(double a_amplitude);

typedef double (*get_frequency_func)();
typedef void (*set_frequency_func)(double a_frequency);

typedef int (*enabled_func)();
typedef void (*signal_enable_func)(int a_enable);

typedef signal_type_t (*get_signal_type_func)();
typedef void (*set_signal_type_func)(signal_type_t a_signal_type);

typedef dc_polarity_t (*get_polarity_func)();
typedef void (*set_polarity_func)(dc_polarity_t a_polarity);

typedef int (*is_connected_func)();

typedef int (*is_signal_ready_func)();


struct n4_25_api_t
{
  revision_func revision;

  usb_init_func usb_init;

  usb_deinit_func usb_deinit;
  usb_tick_func usb_tick;

  usb_devices_changed_func usb_devices_changed;
  get_usb_devices_func get_usb_devices;
  free_usb_devices_func free_usb_devices;
  get_usb_status_func get_usb_status;

  connect_usb_func connect_usb;
  disconnect_usb_func disconnect_usb;


  get_amplitude_func get_amplitude;
  set_amplitude_func set_amplitude;

  get_frequency_func get_frequency;
  set_frequency_func set_frequency;

  enabled_func enabled;
  signal_enable_func signal_enable;

  get_signal_type_func get_signal_type;
  set_signal_type_func set_signal_type;

  get_polarity_func get_polarity;
  set_polarity_func set_polarity;

  is_connected_func is_connected;

  is_signal_ready_func is_signal_ready;

};


#endif // N4_25_API_H


