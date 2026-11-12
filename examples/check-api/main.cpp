#include <iostream>
#include <n4-25.h>
#include <string>

int main()
{
  std::string error_message;
  n4_25_library_handle_t library = load_n4_25_lib(error_message);
  if (library == nullptr) {
    std::cerr << "Не удалось загрузить библиотеку n4-25. Ошибка: " << error_message << std::endl;
    return 1;
  }

  n4_25_api_t api{};
  const bool initialized = n4_25_init_api(library, &api);
  if (!initialized) {
    std::cerr << "Не удалось инициализировать библиотеку n4-25" << std::endl;
  }

  free_n4_25_lib(library);
  return initialized ? 0 : 1;
}
