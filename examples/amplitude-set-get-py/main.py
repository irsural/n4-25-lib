import ctypes
import queue
import sys
import threading
import time

from actions import action_t, take_action
from n4_25 import free_n4_25_lib, load_n4_25_lib, n4_25_api_t, n4_25_init_api
from utils import get_input

USB_TICK_PERIOD_MS = 10


class usb_sync_t:
    actions_queue: queue.Queue[action_t]
    results_queue: queue.Queue[str]

    def __init__(self) -> None:
        self.actions_queue = queue.Queue()
        self.results_queue = queue.Queue()


sync = usb_sync_t()


def usb_tick(n4_25_api: n4_25_api_t) -> None:
    accept_action_period_ms = 100
    usb_tick_count_to_accept_action = accept_action_period_ms // USB_TICK_PERIOD_MS

    n4_25_api.usb_init(1400)

    usb_tick_counter = 0
    while True:
        time.sleep(USB_TICK_PERIOD_MS / 1000)

        n4_25_api.usb_tick()

        usb_tick_counter += 1
        if usb_tick_counter >= usb_tick_count_to_accept_action:
            usb_tick_counter = 0

            result_message = ""
            if not sync.actions_queue.empty():
                action = sync.actions_queue.get()
                try:
                    result_message = take_action(action, n4_25_api, USB_TICK_PERIOD_MS)
                except EOFError:
                    sync.results_queue.put("")
                    break

                sync.results_queue.put(result_message)
                if action == action_t.exit:
                    break

    n4_25_api.usb_deinit()


def main_loop(n4_25_api: n4_25_api_t) -> None:
    usb_tick_thread = threading.Thread(target=usb_tick, args=(n4_25_api,))
    usb_tick_thread.start()
    action_result = ""

    while True:
        print(
            "\nВыберите операцию:\n"
            "1) Выбрать USB\n"
            "2) Установить амплитуду\n"
            "3) Считать амплитуду\n"
            "4) Выйти"
        )

        if action_result:
            print("\n" + action_result)

        try:
            action = action_t(
                get_input(int, int(action_t.select_usb), int(action_t.exit))
            )
        except EOFError:
            sync.actions_queue.put(action_t.exit)
            break

        sync.actions_queue.put(action)
        action_result = sync.results_queue.get()

        if action == action_t.exit:
            print("\n" + action_result)
            break

    usb_tick_thread.join()


def main() -> int:
    if sys.platform == "win32":
        ctypes.windll.kernel32.SetConsoleOutputCP(65001)

    try:
        n4_25_lib = load_n4_25_lib()
    except OSError as error:
        print(
            f"Не удалось загрузить библиотеку n4-25. Ошибка: {error}", file=sys.stderr
        )
        return 1

    n4_25_api = n4_25_init_api(n4_25_lib)
    if n4_25_api is None:
        print("Не удалось инициализировать библиотеку n4-25", file=sys.stderr)
        free_n4_25_lib(n4_25_lib)
        return 1

    print(f"Библиотека n4-25 загружена. Версия: {n4_25_api.revision()}")

    exit_code = main_loop(n4_25_api)

    free_n4_25_lib(n4_25_lib)
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
