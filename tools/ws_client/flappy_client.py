"""Ручной тестовый клиент для flappy_server.

Примеры:
  python flappy_client.py create --player player-1
  python flappy_client.py join --session <id> --player player-2

После подключения жми Enter, чтобы отправить jump. Ctrl+C — выход.
"""

import argparse
import asyncio
import json

import websockets


async def listen(ws, state):
    """Печатает всё, что присылает сервер, и запоминает session_id из ack."""
    async for raw in ws:
        msg = json.loads(raw)

        # Сервер вернул ack на create/join — запоминаем сессию,
        # чтобы подставлять её в последующие jump.
        if msg.get("type") == "ack" and msg.get("session_id"):
            state["session_id"] = msg["session_id"]
            print(f"[ack] session_id = {msg['session_id']}")
            continue

        # Снапшоты летят ~60 раз в секунду — печатаем кратко, одной строкой,
        # иначе консоль превратится в водопад.
        if "players" in msg:
            players = ", ".join(
                f"{p['player_id']}: y={p['y']:.1f} alive={p['alive']}"
                for p in msg["players"]
            )
            print(f"[tick {msg.get('tick')}] {players}", end="\r")
            continue

        print(f"\n[server] {msg}")


async def send_jumps(ws, state, player_id):
    """Ждёт Enter в консоли и шлёт jump. input() блокирует поток,
    поэтому уводим его в отдельный поток через asyncio.to_thread."""
    while True:
        await asyncio.to_thread(input)
        await ws.send(json.dumps({
            "type": "jump",
            "session_id": state["session_id"],
            "player_id": player_id,
        }))
        print("[sent] jump")


async def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=["create", "join"])
    parser.add_argument("--url", default="ws://localhost:8080")
    parser.add_argument("--player", default="player-1")
    parser.add_argument("--session", default="",
                        help="обязателен для join, не нужен для create")
    args = parser.parse_args()

    state = {"session_id": args.session}

    async with websockets.connect(args.url) as ws:
        first = {"type": args.command, "player_id": args.player}
        if args.command == "join":
            first["session_id"] = args.session
        await ws.send(json.dumps(first))
        print(f"[sent] {first}")

        # Слушаем сервер и ждём Enter параллельно — две задачи.
        await asyncio.gather(
            listen(ws, state),
            send_jumps(ws, state, args.player),
        )


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nbye")
