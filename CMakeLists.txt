cmake_minimum_required(VERSION 3.20)

# Имя проекта нужно CMake для общей информации о сборке.
project(flappy_server LANGUAGES CXX)

# Один стандарт упрощает сборку и убирает случайные различия между файлами.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Этот файл помогает редакторам и анализаторам понимать,
# какими флагами реально компилируется проект.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Catch2 подтягивается автоматически, чтобы тесты собирались
# без ручной установки зависимости отдельно в систему.
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

# Домен — это игровое ядро, которое удобно собирать и тестировать отдельно.
add_library(flappy_domain
    src/domain/game/collision_service.cpp
    src/domain/game/physics_engine.cpp
    src/domain/game/obstacle_generator.cpp
    src/domain/session/game_session.cpp
)

# Корень include-ов задаём target'у один раз,
# чтобы в коде можно было писать include от src.
target_include_directories(flappy_domain PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

# очень хочется include path-ы и архитектурное разделение
# поэтому добавляю `INTERFACE`
# Пока ниего не компилирует, но задаёт правила подключения
add_library(flappy_application INTERFACE)

target_include_directories(flappy_application INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(flappy_application INTERFACE
    flappy_domain
)

# Тесты — отдельный исполняемый файл, который использует доменное ядро.
add_executable(flappy_tests
    tests/domain/collision_service_tests.cpp
)

# Зависимости описываются через target'ы,
# тогда include path и link requirements передаются автоматически.
target_link_libraries(flappy_tests PRIVATE
    flappy_domain
    Catch2::Catch2WithMain
)

include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)

# Список тестов не нужно поддерживать вручную —
# CTest найдёт их через интеграцию с Catch2.
catch_discover_tests(flappy_tests)
