#include "actions.h"
#include "ts_queue.h"
#include "utils.h"

#include <chrono>
#include <iostream>
#include <n4-25-api.h>
#include <n4-25.h>
#include <string>
#include <thread>

const size_t USB_TICK_PERIOD_MS = 10;

struct usb_sync_t
{
  ts_queue_t<action_t> actions_queue;
  ts_queue_t<std::string> results_queue;
} sync;

void usb_tick(const n4_25_api_t& n4_25_api)
{
  const size_t accept_action_period_ms = 100;
  const size_t usb_tick_count_to_accept_action = accept_action_period_ms / USB_TICK_PERIOD_MS;

  n4_25_api.usb_init(1400);

  size_t usb_tick_counter = 0;
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(USB_TICK_PERIOD_MS));

    n4_25_api.usb_tick();

    usb_tick_counter++;
    if (usb_tick_counter >= usb_tick_count_to_accept_action) {
      usb_tick_counter = 0;

      std::string result_message = "";
      if (!sync.actions_queue.empty()) {
        action_t action = sync.actions_queue.pop();
        result_message = take_action(action, n4_25_api, USB_TICK_PERIOD_MS);

        sync.results_queue.push(result_message);
        if (action == action_t::exit) {
          break;
        }
      }
    }
  }

  n4_25_api.usb_deinit();
}

void main_loop(n4_25_api_t& n4_25_api)
{
  std::thread usb_tick_thread(usb_tick, n4_25_api);
  std::string action_result;

  while (1) {
    log_info(
      "\nВыберите операцию:\n"
      "1) Выбрать USB\n"
      "2) Установить амплитуду\n"
      "3) Считать амплитуду\n"
      "4) Выйти"
    );

    if (!action_result.empty()) {
      log_info("\n" + action_result);
    }

    action_t action = static_cast<action_t>(
      get_input<int>(static_cast<int>(action_t::select_usb), static_cast<int>(action_t::exit))
    );

    sync.actions_queue.push(action);
    action_result = sync.results_queue.pop();

    if (action == action_t::exit) {
      log_info("\n" + action_result);
      break;
    }
  }

  usb_tick_thread.join();
}

int main()
{
#ifdef _WIN32
  SetConsoleOutputCP(65001); "Russian");
#endif // _WIN32

  std::string error_message;
  n4_25_library_handle_t n4_25_dll = load_n4_25_lib(error_message);
  if (n4_25_dll == nullptr) {
    std::cerr << "Не удалось загрузить библиотеку n4-25. Ошибка: " << error_message << std::endl;
    return 1;
  }

  n4_25_api_t n4_25_api{};
  if (!n4_25_init_api(n4_25_dll, &n4_25_api)) {
    std::cerr << "Не удалось инициализировать библиотеку n4-25" << std::endl;
    free_n4_25_lib(n4_25_dll);
    return 1;
  }

  log_info("Библиотека n4-25 загружена. Версия: " + std::to_string(n4_25_api.revision()));

  main_loop(n4_25_api);

  free_n4_25_lib(n4_25_dll);
}
