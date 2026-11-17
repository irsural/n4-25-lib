#include "actions.h"
#include "ts_queue.h"
#include "utils.h"

#include <n4-25.h>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

const size_t USB_TICK_PERIOD_MS = 10;

struct usb_sync_t
{
  ts_queue_t<action_t> actions_queue;
  ts_queue_t<std::string> results_queue;
} sync;

void usb_worker()
{
  const size_t accept_action_period_ms = 100;
  const size_t usb_tick_count_to_accept_action = accept_action_period_ms / USB_TICK_PERIOD_MS;

  usb_init(1400);

  size_t usb_tick_counter = 0;
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(USB_TICK_PERIOD_MS));

    usb_tick();

    usb_tick_counter++;
    if (usb_tick_counter >= usb_tick_count_to_accept_action) {
      usb_tick_counter = 0;

      std::string result_message = "";
      if (!sync.actions_queue.empty()) {
        action_t action = sync.actions_queue.pop();
        result_message = take_action(action, USB_TICK_PERIOD_MS);

        sync.results_queue.push(result_message);
        if (action == action_t::exit) {
          break;
        }
      }
    }
  }

  usb_deinit();
}

void main_loop()
{
  std::thread usb_tick_thread(usb_worker);
  std::string action_result;

  while (true) {
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
  SetConsoleOutputCP(65001);
#endif

  log_info("Версия n4-25-lib: " + std::to_string(revision()));

  main_loop();
}
