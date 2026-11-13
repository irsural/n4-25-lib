import sys
from collections.abc import Callable
from typing import overload


@overload
def get_input(convert: Callable[[str], int], min_value: int, max_value: int) -> int: ...


@overload
def get_input(
    convert: Callable[[str], float], min_value: float, max_value: float
) -> float: ...


def get_input(
    convert: Callable[[str], int | float], min_value: float, max_value: float
) -> int | float:
    while True:
        print(">>> ", end="", flush=True)
        line = sys.stdin.readline()
        if line == "":
            raise EOFError

        try:
            value: int | float | None = convert(line.strip())
        except ValueError:
            value = None

        if value is not None and min_value <= value <= max_value:
            return value

        print(f"Ошибка: введите число от {min_value} до {max_value}")
