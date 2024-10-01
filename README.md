# n4-25-lib

## Описание

Динамическая библиотека для управления калибратором N4-25.

Библиотека собрана для платформы x86, поэтому проекты, подключающие библиотеку, должны также
собираться под x86.


## Файлы проекта

- папка **src** содержит header-файлы с объявлениями функций API.
- папка **lib** содержит саму библиотеку (DLL). Путь к папке, в которой находится библиотека,
  должен быть добавлен в переменную окружения **PATH**!
- папка **examples** содержит решение Visual Studio, с примерами использования библиотеки.
