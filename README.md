# Описание

Динамическая библиотека n4-25-lib для управления калибратором Н4-25.

## Поддерживаемые платформы

| Каталог              | Runtime-файл       | Import library | Архитектура |
| -------------------- | ------------------ | -------------- | ----------- |
| `lib/windows-x86`    | `n4-25.dll`        | `n4-25.lib`    | Windows x86 |
| `lib/windows-amd64`  | `n4-25.dll`        | `n4-25.lib`    | Windows x64 |
| `lib/linux-x86`      | `libn4-25.so`      | —              | Linux x86   |
| `lib/linux-amd64`    | `libn4-25.so`      | —              | Linux x64   |

## Файлы проекта

- `src/n4-25.h` — публичный C/C++-заголовок с API.
- `lib/<platform>-<arch>` — runtime-библиотеки.
- `examples/amplitude-set-get` — пример на C++.
- `examples/amplitude-set-get-py` — пример на Python.

CMake определяет цель `n4_25::n4_25`. Она автоматически выбирает библиотеку для текущей
платформы и разрядности. При необходимости каталог можно переопределить через
`N4_25_ARTIFACT_DIR`.

## Подключение

```cmake
add_subdirectory(path/to/n4-25-lib)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE n4_25::n4_25)
```

Runtime-файл библиотеки должен находиться в путях поиска динамического загрузчика.
Например:

```bash
LD_LIBRARY_PATH="$PWD/lib/linux-amd64:$LD_LIBRARY_PATH" ./app
```

## Примеры

Пример `amplitude-set-get` демонстрирует подключение к Н4-25, установку и чтение амплитуды.

Python-пример `amplitude-set-get-py`  загружает библиотеку явно, по полному пути, с
помощью ``ctypes.CDLL``.

```bash
make run_py
```

### Linux

Зависимости:

- `libudev.so.1`

Сборка и запуск:

```bash
make build
make run
```

`make run` добавляет `lib/linux-amd64` в `LD_LIBRARY_PATH`.

### Windows

Зависимости:

- Microsoft Visual C++ Redistributable 2015–2022

Проект CMake можно открыть в Visual Studio:

- открыть корневую папку репозитория через **File → Open → Folder**;
- выбрать пресет сборки `windows-amd64-debug` или `windows-amd64-release`;
- выбрать цель `amplitude-set-get`.

Пресет добавляет `lib/windows-amd64` в `PATH`.

## Доступ к USB-устройству на Linux

Для доступа к USB-устройству у пользователя должны быть необходимые права.

Права можно обеспечить запуском программы от суперпользователя либо правилом udev.

### Запуск с правами суперпользователя

```bash
make build
sudo make run
```

### Правило udev

Для дистрибутивов на базе Debian создайте файл
`/etc/udev/rules.d/99-n4-25.rules`:

```udev
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5711", MODE="0660", GROUP="plugdev"
```

Перезагрузите правила:

```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Если пользователь не входит в группу `plugdev`, добавьте его:

```bash
sudo usermod -aG plugdev <user>
```

После добавления пользователя в группу нужно перезайти в учётную запись.
