#ifndef N4_25_WINDOWS_H
#define N4_25_WINDOWS_H

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <stdint.h>
#include <windows.h>
#include <n4-25-api.h>


#define INIT_N4_25_API(field,dll,function) \
  field = reinterpret_cast<function ## _func>(GetProcAddress(dll, #function));\
  if (field == nullptr) {\
    return false;\
  }


bool n4_25_init_api(HINSTANCE a_n4_25_dll, n4_25_api_t* a_api) {
  INIT_N4_25_API(a_api->revision, a_n4_25_dll, revision);

  INIT_N4_25_API(a_api->usb_init, a_n4_25_dll, usb_init);

  INIT_N4_25_API(a_api->usb_deinit, a_n4_25_dll, usb_deinit);
  INIT_N4_25_API(a_api->usb_tick, a_n4_25_dll, usb_tick);

  INIT_N4_25_API(a_api->usb_devices_changed, a_n4_25_dll, usb_devices_changed);
  INIT_N4_25_API(a_api->get_usb_devices, a_n4_25_dll, get_usb_devices);
  INIT_N4_25_API(a_api->free_usb_devices, a_n4_25_dll, free_usb_devices);
  INIT_N4_25_API(a_api->get_usb_status, a_n4_25_dll, get_usb_status);

  INIT_N4_25_API(a_api->connect_usb, a_n4_25_dll, connect_usb);
  INIT_N4_25_API(a_api->disconnect_usb, a_n4_25_dll, disconnect_usb);


  INIT_N4_25_API(a_api->get_amplitude, a_n4_25_dll, get_amplitude);
  INIT_N4_25_API(a_api->set_amplitude, a_n4_25_dll, set_amplitude);

  INIT_N4_25_API(a_api->get_frequency, a_n4_25_dll, get_frequency);
  INIT_N4_25_API(a_api->set_frequency, a_n4_25_dll, set_frequency);

  INIT_N4_25_API(a_api->enabled, a_n4_25_dll, enabled);
  INIT_N4_25_API(a_api->signal_enable, a_n4_25_dll, signal_enable);

  INIT_N4_25_API(a_api->get_signal_type, a_n4_25_dll, get_signal_type);
  INIT_N4_25_API(a_api->set_signal_type, a_n4_25_dll, set_signal_type);

  INIT_N4_25_API(a_api->get_polarity, a_n4_25_dll, get_polarity);
  INIT_N4_25_API(a_api->set_polarity, a_n4_25_dll, set_polarity);

  INIT_N4_25_API(a_api->is_connected, a_n4_25_dll, is_connected);

  INIT_N4_25_API(a_api->is_signal_ready, a_n4_25_dll, is_signal_ready);

  return true;
}


#endif // _WIN32

#endif // N4_25_WINDOWS_H
