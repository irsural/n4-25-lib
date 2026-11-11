#ifndef N4_25_H
#define N4_25_H

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
using n4_25_library_handle_t = HMODULE;
#define N4_25_GET_SYMBOL GetProcAddress

#elif defined(__linux__)

#include <dlfcn.h>
using n4_25_library_handle_t = void *;
#define N4_25_GET_SYMBOL dlsym

#else
#error Неподдерживаемая платформа: поддерживаются Windows x86 и Linux x86-64
#endif

#include <string>
#include <n4-25-api.h>

inline n4_25_library_handle_t load_n4_25_lib(std::string &error_message) {
#if defined(_WIN32)
  n4_25_library_handle_t library = LoadLibraryA("n4-25.dll");
  if (library == nullptr) {
    error_message =  std::to_string(GetLastError());
  }
#else
  n4_25_library_handle_t library = dlopen("libn4-25.so", RTLD_NOW | RTLD_LOCAL);
  if (library == nullptr) {
    error_message = std::string(dlerror());
  }
#endif
  return library;
}

inline void free_n4_25_lib(n4_25_library_handle_t library) {
#if defined(_WIN32)
  FreeLibrary(library);
#else
  dlclose(library);
#endif
}

#define N4_25_INIT_API(field, dll, function)                                   \
  field = reinterpret_cast<function##_func>(N4_25_GET_SYMBOL(dll, #function)); \
  if (field == nullptr) {                                                      \
    return false;                                                              \
  }

inline bool n4_25_init_api(n4_25_library_handle_t a_n4_25_dll,
                           n4_25_api_t *a_api) {
  N4_25_INIT_API(a_api->revision, a_n4_25_dll, revision);

  N4_25_INIT_API(a_api->usb_init, a_n4_25_dll, usb_init);

  N4_25_INIT_API(a_api->usb_deinit, a_n4_25_dll, usb_deinit);
  N4_25_INIT_API(a_api->usb_tick, a_n4_25_dll, usb_tick);

  N4_25_INIT_API(a_api->usb_devices_changed, a_n4_25_dll, usb_devices_changed);
  N4_25_INIT_API(a_api->get_usb_devices, a_n4_25_dll, get_usb_devices);
  N4_25_INIT_API(a_api->free_usb_devices, a_n4_25_dll, free_usb_devices);
  N4_25_INIT_API(a_api->get_usb_status, a_n4_25_dll, get_usb_status);

  N4_25_INIT_API(a_api->connect_usb, a_n4_25_dll, connect_usb);
  N4_25_INIT_API(a_api->disconnect_usb, a_n4_25_dll, disconnect_usb);

  N4_25_INIT_API(a_api->get_amplitude, a_n4_25_dll, get_amplitude);
  N4_25_INIT_API(a_api->set_amplitude, a_n4_25_dll, set_amplitude);

  N4_25_INIT_API(a_api->get_frequency, a_n4_25_dll, get_frequency);
  N4_25_INIT_API(a_api->set_frequency, a_n4_25_dll, set_frequency);

  N4_25_INIT_API(a_api->enabled, a_n4_25_dll, enabled);
  N4_25_INIT_API(a_api->signal_enable, a_n4_25_dll, signal_enable);

  N4_25_INIT_API(a_api->get_signal_type, a_n4_25_dll, get_signal_type);
  N4_25_INIT_API(a_api->set_signal_type, a_n4_25_dll, set_signal_type);

  N4_25_INIT_API(a_api->get_polarity, a_n4_25_dll, get_polarity);
  N4_25_INIT_API(a_api->set_polarity, a_n4_25_dll, set_polarity);

  N4_25_INIT_API(a_api->is_connected, a_n4_25_dll, is_connected);

  N4_25_INIT_API(a_api->is_signal_ready, a_n4_25_dll, is_signal_ready);

  return true;
}

#undef N4_25_INIT_API
#undef N4_25_GET_SYMBOL

#endif // N4_25_H
