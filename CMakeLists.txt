cmake_minimum_required(VERSION 3.20)

project(flappy_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost CONFIG REQUIRED COMPONENTS json)

message(STATUS "Boost_FOUND=${Boost_FOUND}")
message(STATUS "Boost_DIR=${Boost_DIR}")

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
)

target_include_directories(flappy_domain
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_features(flappy_domain
    PUBLIC
        cxx_std_20
)

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

    src/infrastructure/net/websocket_message_parser.cpp
)

target_include_directories(flappy_application
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(flappy_application
    PUBLIC
        flappy_domain
        Boost::json
)

add_executable(flappy_server
    src/main.cpp
)

target_include_directories(flappy_server
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(flappy_server
    PRIVATE
        flappy_application
)

add_executable(flappy_tests
    tests/domain/collision_service_tests.cpp
    tests/domain/game_session_tests.cpp
    tests/application/use_cases_tests.cpp
    tests/infrastructure/websocket_router_tests.cpp
)

target_include_directories(flappy_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(flappy_tests
    PRIVATE
        flappy_application
        Catch2::Catch2WithMain
)

include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)

catch_discover_tests(flappy_tests)

