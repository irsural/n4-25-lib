#include "actions.h"

#include "utils.h"

#include <n4-25-api.h>
#include <n4-25.h>
#include <string>
#include <thread>
#include <vector>

std::string take_action(action_t action, const n4_25_api_t& n4_25_api, size_t usb_tick_period_ms)
{
  switch (action) {
    case (action_t::select_usb): {
      return select_usb(n4_25_api);
    } break;

    case (action_t::set_amplitude): {
      return set_amplitude(n4_25_api, usb_tick_period_ms);
    } break;

    case (action_t::get_amplitude): {
      return get_amplitude(n4_25_api);
    } break;

    case (action_t::exit): {
      return "Выход из программы";
    } break;
  }
}

std::string select_usb(const n4_25_api_t& n4_25_api)
{
  std::string action_result = "";

  std::vector<std::string> usb_list{};
  char* usb_devices = nullptr;
  n4_25_api.get_usb_devices(&usb_devices);
  usb_list = split_string(usb_devices);
  n4_25_api.free_usb_devices(&usb_devices);

  log_info("\nВыберите USB:");
  size_t i = 0;
  for (; i < usb_list.size(); ++i) {
    log_info(std::to_string(i + 1) + ") " + usb_list[i]);
  }
  log_info(std::to_string(i + 1) + ") Вернуться в меню");

  // NOTE: Вызов get_input блокирует тики usb. Это намеренное упрощение, в реальных программах
  //  лучше вызывать тики с одинаковой периодичностью
  uint32_t option = get_input<uint32_t>(1, usb_list.size() + 1);
  if (option != usb_list.size() + 1) {
    if (n4_25_api.connect_usb(usb_list[option - 1].c_str())) {
      action_result = "Успешное подключение";
    } else {
      action_result = "Ошибка: не удалось подключиться";
    }
  } else {
    action_result = "Прибор не выбран";
  }

  return action_result;
}

std::string set_amplitude(const n4_25_api_t& n4_25_api, size_t usb_tick_period_ms)
{
  if (!n4_25_api.is_connected()) {
    return "Ошибка: нет соединения с Н4-25";
  }

  log_info("\nВведите амплитуду:");
  double target_amplitude = get_input<double>(0, 10);
  double current_amplitude = n4_25_api.get_amplitude();

  n4_25_api.set_amplitude(target_amplitude);

  while (current_amplitude != target_amplitude) {
    n4_25_api.usb_tick();

    current_amplitude = n4_25_api.get_amplitude();

    if (!n4_25_api.is_connected()) {
      return "Ошибка: нет соединения с Н4-25";
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(usb_tick_period_ms));
  }

  return "Амплитуда установлена";
}

std::string get_amplitude(const n4_25_api_t& n4_25_api)
{
  if (n4_25_api.is_connected()) {
    return "Текущий уровень сигнала " + std::to_string(n4_25_api.get_amplitude());
  } else {
    return "Ошибка: нет соединения с Н4-25";
  }
}
