#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <string>

#include <n4-25-api.h>
#include <n4-25-windows.h>

#include "utils.h"


enum class action_t {
  select_usb = 1,
  set_amplitude = 2,
  get_amplitude = 3,
  exit = 4
};


struct usb_sync_t {
  std::mutex api_mutex{};
  std::condition_variable cv{};
  bool ready = false;
  bool stop = false;
} usb_sync;


void usb_tick(const n4_25_api_t& n4_25_api)
{
  {
    std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);
    n4_25_api.usb_init(1400);
    usb_sync.ready = true;
  }
  usb_sync.cv.notify_one();

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);

    if (usb_sync.stop) {
      break;
    }
    n4_25_api.usb_tick();
  }

  {
    std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);
    n4_25_api.usb_deinit();
    usb_sync.ready = true;
  }
  usb_sync.cv.notify_one();
}


void main_loop(n4_25_api_t& n4_25_api)
{
  std::thread usb_tick_thread(usb_tick, n4_25_api);

  {
    std::unique_lock<std::mutex> usb_cv_lock(usb_sync.api_mutex);
    usb_sync.cv.wait(usb_cv_lock, []() { return usb_sync.ready; });
  }

  while(1) {
    log_info(
      "\nВыберите операцию:\n"
      "1) Выбрать USB\n"
      "2) Установить амплитуду\n"
      "3) Считать амплитуду\n"
      "4) Выйти"
    );
    action_t action = static_cast<action_t>(
      get_input<int>(static_cast<int>(action_t::select_usb), static_cast<int>(action_t::exit))
    );

    switch (action) {
      case (action_t::select_usb): {
        std::vector<std::string> usb_list{};
        {
          std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);
          char* usb_devices = nullptr;
          n4_25_api.get_usb_devices(&usb_devices);
          usb_list = split_string(usb_devices);
          n4_25_api.free_usb_devices(&usb_devices);
        }

        log_info("\nВыберите USB:");
        size_t i = 0;
        for (; i < usb_list.size(); ++i) {
          log_info(std::to_string(i + 1) + ") " + usb_list[i]);
        }
        log_info(std::to_string(i + 1) + ") Вернуться в меню");

        uint32_t option = get_input<uint32_t>(1, usb_list.size() + 1);
        if (option != usb_list.size() + 1) {
          if (n4_25_api.connect_usb(usb_list[option - 1].c_str())) {
            log_info("Успешное подключение");
          } else {
            log_error("Не удалось подключиться");
          }
         }
      } break;

      case (action_t::set_amplitude): {
        bool n4_25_connected = false;
        {
          std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);
          if (n4_25_api.is_connected()) {
            n4_25_connected = true;
          } else {
            log_error("Нет соединения с N4-25");
          }
        }

        if (n4_25_connected) {
          log_info("\nВведите амплитуду:");
          double target_amplitude = get_input<double>(0, 10);
          double current_amplitude = 0;

          {
            std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);
            current_amplitude = n4_25_api.get_amplitude();
            n4_25_api.set_amplitude(target_amplitude);
          }

          while(1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            if (current_amplitude == target_amplitude) {
              log_info("Амплитуда установлена");
              break;
            }

            std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);
            if (!n4_25_api.is_connected()) {
              log_error("Нет соединения с N4-25");
              break;
            }
            current_amplitude = n4_25_api.get_amplitude();
          }
        }
      } break;

      case (action_t::get_amplitude): {
        std::lock_guard<std::mutex> usb_lock(usb_sync.api_mutex);
        if (n4_25_api.is_connected()) {
          log_info("Текущий уровень сигнала " + std::to_string(n4_25_api.get_amplitude()));
        } else {
          log_error("Нет соединения с N4-25");
        }
      } break;

      case (action_t::exit): {
        log_info("Выход из программы");
        {
          std::unique_lock<std::mutex> usb_lock(usb_sync.api_mutex);
          usb_sync.ready = false;
          usb_sync.stop = true;
          usb_sync.cv.wait(usb_lock, []() { return usb_sync.ready; });
        }
        usb_tick_thread.join();
        return;
      } break;
    }
  }
}


int main()
{
  setlocale(LC_ALL, "Russian");

  HINSTANCE n4_25_dll = LoadLibraryA("n4-25.dll");
  if (n4_25_dll == nullptr) {
    log_info("Не удалось загрузить библиотеку n4-24.dll");
    return 1;
  }

  n4_25_api_t n4_25_api{};
  if (!n4_25_init_api(n4_25_dll, &n4_25_api)) {
    log_info("Не удалось инициализировать библиотеку N4-24.dll");
    return 1;
  }

  log_info("Библиотека n4-24.dll загружена. Версия: " + std::to_string(n4_25_api.revision()));

  main_loop(n4_25_api);

  FreeLibrary(n4_25_dll);
}
