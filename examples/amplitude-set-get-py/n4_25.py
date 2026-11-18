from __future__ import annotations

import ctypes
from pathlib import Path
import enum
import os
import sys
from collections.abc import Callable
from ctypes import POINTER, c_char_p, c_double, c_int, c_size_t
from typing import TYPE_CHECKING, Any

if TYPE_CHECKING:
    from ctypes import _Pointer


class dc_polarity_t(enum.IntEnum):
    pos = 0
    neg = 1


class signal_type_t(enum.IntEnum):
    aci = 0
    acv = 1
    dci = 2
    dcv = 3


def load_n4_25_lib() -> n4_25_api_t:
    if sys.platform == "win32":
        library_folder = "windows-amd64"
    elif sys.platform.startswith("linux"):
        library_folder = "linux_amd64"
    else:
        raise OSError(f"Неподдерживаемая платформа: {sys.platform}")
    
    library_full_path = Path(os.path.dirname(__file__) + f"/../../lib/{library_folder}/n4-25.dll").resolve()
    library = ctypes.CDLL(str(library_full_path))
    return n4_25_api_t(library)


# Порядок и сигнатуры повторяют публичный n4-25.h.
_BINDINGS: tuple[tuple[str, type[Any] | None, list[type[Any]]], ...] = (
    ("revision", c_int, []),
    ("usb_init", None, [c_size_t]),
    ("usb_deinit", None, []),
    ("usb_tick", None, []),
    ("usb_devices_changed", c_int, []),
    ("get_usb_devices", None, [POINTER(c_char_p)]),
    ("free_usb_devices", None, [POINTER(c_char_p)]),
    ("get_usb_status", c_int, []),
    ("connect_usb", c_int, [c_char_p]),
    ("disconnect_usb", None, []),
    ("get_amplitude", c_double, []),
    ("set_amplitude", None, [c_double]),
    ("get_frequency", c_double, []),
    ("set_frequency", None, [c_double]),
    ("enabled", c_int, []),
    ("signal_enable", None, [c_int]),
    ("get_signal_type", c_int, []),
    ("set_signal_type", None, [c_int]),
    ("get_polarity", c_int, []),
    ("set_polarity", None, [c_int]),
    ("is_connected", c_int, []),
    ("is_signal_ready", c_int, []),
)


class n4_25_api_t:
    revision: Callable[[], int]
    usb_init: Callable[[int], None]
    usb_deinit: Callable[[], None]
    usb_tick: Callable[[], None]
    usb_devices_changed: Callable[[], int]
    get_usb_devices: Callable[[_Pointer[c_char_p]], None]
    free_usb_devices: Callable[[_Pointer[c_char_p]], None]
    get_usb_status: Callable[[], int]
    connect_usb: Callable[[bytes], int]
    disconnect_usb: Callable[[], None]
    get_amplitude: Callable[[], float]
    set_amplitude: Callable[[float], None]
    get_frequency: Callable[[], float]
    set_frequency: Callable[[float], None]
    enabled: Callable[[], int]
    signal_enable: Callable[[int], None]
    get_signal_type: Callable[[], int]
    set_signal_type: Callable[[int], None]
    get_polarity: Callable[[], int]
    set_polarity: Callable[[int], None]
    is_connected: Callable[[], int]
    is_signal_ready: Callable[[], int]

    def __init__(self, library: ctypes.CDLL) -> None:
        self._library = library
        for name, restype, argtypes in _BINDINGS:
            function = getattr(library, name)
            function.restype = restype
            function.argtypes = argtypes
            setattr(self, name, function)
