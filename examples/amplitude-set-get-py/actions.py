import ctypes
import enum
import time

from n4_25 import n4_25_api_t
from utils import get_input


class action_t(enum.IntEnum):
    select_usb = 1
    set_amplitude = 2
    get_amplitude = 3
    exit = 4


def take_action(
    action: action_t, n4_25_api: n4_25_api_t, usb_tick_period_ms: int
) -> str:
    message = ""
    if action == action_t.select_usb:
        message = select_usb(n4_25_api)
    elif action == action_t.set_amplitude:
        message = set_amplitude(n4_25_api, usb_tick_period_ms)
    elif action == action_t.get_amplitude:
        message = get_amplitude(n4_25_api)
    elif action == action_t.exit:
        message = "Выход из программы"
    return message


def select_usb(n4_25_api: n4_25_api_t) -> str:
    usb_devices = ctypes.c_char_p()
    usb_devices_ptr = ctypes.pointer(usb_devices)
    n4_25_api.get_usb_devices(usb_devices_ptr)

    usb_list_str = (usb_devices.value or b"").decode("utf-8")
    usb_list = [part for part in usb_list_str.split(";") if part]

    n4_25_api.free_usb_devices(usb_devices_ptr)

    print("\nВыберите USB:")
    for i, device in enumerate(usb_list):
        print(f"{i + 1}) {device}")
    print(f"{len(usb_list) + 1}) Вернуться в меню")

    # NOTE: Вызов get_input блокирует тики usb. Это намеренное упрощение, в реальных программах
    #  лучше вызывать тики с одинаковой периодичностью
    option = get_input(int, 1, len(usb_list) + 1)
    if option != len(usb_list) + 1:
        if n4_25_api.connect_usb(usb_list[option - 1].encode("utf-8")):
            return "Успешное подключение"
        return "Ошибка: не удалось подключиться"
    return "Прибор не выбран"


def set_amplitude(n4_25_api: n4_25_api_t, usb_tick_period_ms: int) -> str:
    if not n4_25_api.is_connected():
        return "Ошибка: нет соединения с Н4-25"

    print("\nВведите амплитуду:")
    target_amplitude = get_input(float, 0, 10)
    current_amplitude = n4_25_api.get_amplitude()

    n4_25_api.set_amplitude(target_amplitude)

    while current_amplitude != target_amplitude:
        n4_25_api.usb_tick()

        current_amplitude = n4_25_api.get_amplitude()

        if not n4_25_api.is_connected():
            return "Ошибка: нет соединения с Н4-25"

        time.sleep(usb_tick_period_ms / 1000)

    return "Амплитуда установлена"


def get_amplitude(n4_25_api: n4_25_api_t) -> str:
    if n4_25_api.is_connected():
        return f"Текущий уровень сигнала {n4_25_api.get_amplitude():f}"
    return "Ошибка: нет соединения с Н4-25"

