#include "actions.h"

#include "utils.h"

#include <chrono>
#include <string>
#include <thread>
#include <vector>

std::string take_action(action_t action, size_t usb_tick_period_ms)
{
  std::string message = "";
  switch (action) {
    case (action_t::select_usb): {
      message = select_usb();
    } break;

    case (action_t::set_amplitude): {
      message = set_amplitude_action(usb_tick_period_ms);
    } break;

    case (action_t::get_amplitude): {
      message = get_amplitude_action();
    } break;

    case (action_t::exit): {
      message = "Выход из программы";
    } break;
  }
  return message;
}

std::string select_usb()
{
  std::string action_result = "";

  std::vector<std::string> usb_list{};
  char* usb_devices = nullptr;
  get_usb_devices(&usb_devices);
  usb_list = split_string(usb_devices);
  free_usb_devices(&usb_devices);

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
    if (connect_usb(usb_list[option - 1].c_str())) {
      action_result = "Успешное подключение";
    } else {
      action_result = "Ошибка: не удалось подключиться";
    }
  } else {
    action_result = "Прибор не выбран";
  }

  return action_result;
}

std::string set_amplitude_action(size_t usb_tick_period_ms)
{
  if (!is_connected()) {
    return "Ошибка: нет соединения с Н4-25";
  }

  log_info("\nВведите амплитуду:");
  double target_amplitude = get_input<double>(0, 10);
  double current_amplitude = get_amplitude();

  set_amplitude(target_amplitude);

  while (current_amplitude != target_amplitude) {
    usb_tick();

    current_amplitude = get_amplitude();

    if (!is_connected()) {
      return "Ошибка: нет соединения с Н4-25";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(usb_tick_period_ms));
  }

  return "Амплитуда установлена";
}

std::string get_amplitude_action()
{
  if (is_connected()) {
    return "Текущий уровень сигнала " + std::to_string(get_amplitude());
  } else {
    return "Ошибка: нет соединения с Н4-25";
  }
}
