```terminal
flappy-server/
├── CMakeLists.txt
├── README.md
├── docker-compose.yml
├── .env
├── docs/
│   ├── architecture.md
│   ├── websocket_protocol.md
│   ├── http_api.md
│   ├── game_rules.md
│   └── database_schema.md
├── migrations/
│   ├── 001_init.sql
│   └── 002_indexes.sql
├── src/
│   ├── main.cpp
│   ├── bootstrap/
│   │   ├── app_builder.h
│   │   ├── app_builder.cpp
│   │   ├── config.h
│   │   ├── config.cpp
│   │   └── dependency_container.h
│   ├── domain/
│   │   ├── common/
│   │   │   ├── types.h
│   │   │   ├── errors.h
│   │   │   └── clock.h
│   │   ├── player/
│   │   │   ├── player.h
│   │   │   ├── player_id.h
│   │   │   └── nickname.h
│   │   ├── game/
│   │   │   ├── bird_state.h
│   │   │   ├── pipe.h
│   │   │   ├── obstacle_generator.h
│   │   │   ├── physics_engine.h
│   │   │   ├── collision_service.h
│   │   │   ├── score_service.h
│   │   │   ├── game_rules.h
│   │   │   └── world_snapshot.h
│   │   ├── session/
│   │   │   ├── session_id.h
│   │   │   ├── game_session.h
│   │   │   ├── session_state.h
│   │   │   ├── player_session_state.h
│   │   │   └── input_command.h
│   │   └── match/
│   │       ├── match_result.h
│   │       ├── leaderboard_entry.h
│   │       └── ranking_service.h
│   ├── application/
│   │   ├── dto/
│   │   │   ├── join_session_command.h
│   │   │   ├── player_input_command.h
│   │   │   ├── session_snapshot_dto.h
│   │   │   └── match_result_dto.h
│   │   ├── ports/
│   │   │   ├── player_repository.h
│   │   │   ├── session_repository.h
│   │   │   ├── match_repository.h
│   │   │   ├── leaderboard_repository.h
│   │   │   ├── unit_of_work.h
│   │   │   ├── session_broadcaster.h
│   │   │   └── id_generator.h
│   │   ├── services/
│   │   │   ├── session_service.h
│   │   │   ├── game_loop_service.h
│   │   │   ├── matchmaking_service.h
│   │   │   └── leaderboard_service.h
│   │   └── use_cases/
│   │       ├── create_session_use_case.h
│   │       ├── join_session_use_case.h
│   │       ├── mark_ready_use_case.h
│   │       ├── submit_input_use_case.h
│   │       ├── start_match_use_case.h
│   │       ├── tick_session_use_case.h
│   │       ├── finish_match_use_case.h
│   │       ├── get_leaderboard_use_case.h
│   │       └── reconnect_player_use_case.h
│   ├── infrastructure/
│   │   ├── db/
│   │   │   ├── postgres/
│   │   │   │   ├── postgres_connection_pool.h
│   │   │   │   ├── postgres_unit_of_work.h
│   │   │   │   ├── postgres_player_repository.h
│   │   │   │   ├── postgres_match_repository.h
│   │   │   │   ├── postgres_leaderboard_repository.h
│   │   │   │   └── pqxx_helpers.h
│   │   ├── net/
│   │   │   ├── websocket/
│   │   │   │   ├── websocket_server.h
│   │   │   │   ├── websocket_session.h
│   │   │   │   ├── websocket_router.h
│   │   │   │   ├── websocket_message_parser.h
│   │   │   │   └── websocket_message_serializer.h
│   │   │   └── http/
│   │   │       ├── http_server.h
│   │   │       ├── http_router.h
│   │   │       ├── leaderboard_handler.h
│   │   │       └── healthcheck_handler.h
│   │   ├── time/
│   │   │   ├── steady_clock.h
│   │   │   └── timer_loop.h
│   │   ├── ids/
│   │   │   └── uuid_generator.h
│   │   └── logging/
│   │       ├── logger.h
│   │       └── spdlog_logger.h
│   └── presentation/
│       ├── ws/
│       │   ├── ws_message_types.h
│       │   ├── ws_controller.h
│       │   └── ws_error_mapper.h
│       └── http/
│           ├── http_controller.h
│           └── json_mapper.h
└── tests/
    ├── domain/
    ├── application/
    ├── integration/
    └── test_helpers/
```
