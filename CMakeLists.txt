cmake_minimum_required(VERSION 3.20)

project(flappy_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

add_library(flappy_domain
    src/domain/game/collision_service.cpp
    src/domain/game/obstacle_generator.cpp
    src/domain/game/physics_engine.cpp
    src/domain/session/game_session.cpp
    # src/domain/player/player_id.cpp
)

target_include_directories(flappy_domain
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_features(flappy_domain
    PUBLIC
        cxx_std_20
)

add_executable(flappy_server
    src/main.cpp
    src/infrastructure/logging/console_logger.cpp
)

target_include_directories(flappy_server
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(flappy_server
    PRIVATE
        flappy_domain
)

add_executable(flappy_tests
    tests/domain/collision_service_tests.cpp
    tests/domain/game_session_tests.cpp
)

target_include_directories(flappy_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(flappy_tests
    PRIVATE
        flappy_domain
        Catch2::Catch2WithMain
)

include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)

catch_discover_tests(flappy_tests)
