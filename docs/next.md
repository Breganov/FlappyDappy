# Что сейчас работает:

- PhysicsEngine;
- CollisionService;
- unit test для collision;
- Catch2 + CTest;

# Что не закончено:

- GameSession только определение в .h;
  - надо проверить то, как вообще работает эта система
- score rules. Как считать -- не ясно;
  - вроде бы как на самом деле сделано, но мне не очень нравится реализация
- webscoket layer только скелет;

# Следующие шаги:

1. Реализовать GameSession::Tick;
2. Добавить GameSession tests;
3. Собрать headless симуляцию.

# Architecture

1. слои
2. основные сущности
3. игровой цикл
4. что authoritative на сервере
5. какие части уже реализованы

# Решения

1. Почему `bird_x` хранится в `config`;
2. Почему cthdth authoritative;
3. Почему храним активные матчи в памяти;
4. Почему `distance` счыитаем вместо сложного `score`;
5. Реализовать `game_sessions.cpp`;
6. Добавить `game_sessions_tests.cpp`;

# Отложить

1. Reconnect
2. HTTP
3. LeaderBoard
4. Полный WebSocket Stack
