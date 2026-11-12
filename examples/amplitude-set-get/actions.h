#include <n4-25-api.h>
#include <n4-25.h>
#include <string>

enum class action_t {
  select_usb = 1,
  set_amplitude = 2,
  get_amplitude = 3,
  exit = 4
};

std::string take_action(action_t action, const n4_25_api_t& n4_25_api, size_t usb_tick_period_ms);

std::string select_usb(const n4_25_api_t& n4_25_api);
std::string set_amplitude(const n4_25_api_t& n4_25_api, size_t usb_tick_period_ms);
std::string get_amplitude(const n4_25_api_t& n4_25_api);
