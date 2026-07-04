cmake_minimum_required(VERSION 3.20)

project(flappy_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# ==================== Boost ====================
find_package(Boost CONFIG REQUIRED COMPONENTS json system)

# Если Boost установлен через vcpkg / conan / system и не находит — можно добавить:
# set(Boost_USE_STATIC_LIBS ON)
# find_package(Boost REQUIRED COMPONENTS json system)

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")

# ==================== Catch2 ====================
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

# ==================== Libraries ====================

add_library(flappy_domain
    src/domain/game/collision_service.cpp
    src/domain/game/obstacle_generator.cpp
    src/domain/game/physics_engine.cpp
    src/domain/session/game_session.cpp
)

target_include_directories(flappy_domain PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_features(flappy_domain PUBLIC cxx_std_20)

add_library(flappy_application
    src/application/ports/simple_id_generator.cpp

    src/application/use_cases/session_service.cpp
    src/application/use_cases/create_session_use_case.cpp
    src/application/use_cases/join_session_use_case.cpp
    src/application/use_cases/start_match_use_case.cpp
    src/application/use_cases/submit_input_use_case.cpp
    src/application/use_cases/tick_session_use_case.cpp
    src/application/use_cases/finish_match_use_case.cpp
    src/application/use_cases/game_loop_service.cpp

    src/infrastructure/logging/console_logger.cpp
    src/infrastructure/logging/console_session_broadcaster.cpp

    src/infrastructure/net/connection_registry.cpp
    src/infrastructure/net/game_loop_timer.cpp
    src/infrastructure/net/websocket_message_handler.cpp
    src/infrastructure/net/websocket_message_parser.cpp
    src/infrastructure/net/websocket_message_serializer.cpp
    src/infrastructure/net/websocket_router.cpp
    src/infrastructure/net/websocket_server.cpp
    src/infrastructure/net/websocket_session.cpp
    src/infrastructure/net/websocket_session_broadcaster.cpp
)

target_include_directories(flappy_application PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(flappy_application
    PUBLIC
        flappy_domain
        Boost::json
        Boost::system
        # Boost::beast
)

# Опционально, но рекомендуется явно:
# target_link_libraries(flappy_application INTERFACE Boost::headers)

# ==================== Executable ====================

add_executable(flappy_server src/main.cpp)

target_include_directories(flappy_server PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(flappy_server PRIVATE flappy_application)

# ==================== Tests ====================

add_executable(flappy_tests
    tests/domain/collision_service_tests.cpp
    tests/domain/game_session_tests.cpp
    tests/application/use_cases_tests.cpp
    tests/infrastructure/websocket_router_tests.cpp
    tests/infrastructure/websocket_message_serializer_tests.cpp
    tests/infrastructure/websocket_message_parser_tests.cpp
)

target_include_directories(flappy_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(flappy_tests
    PRIVATE
        flappy_application
        Catch2::Catch2WithMain
)

include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(flappy_tests)

