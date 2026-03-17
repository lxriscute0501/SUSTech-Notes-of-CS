import asyncio
import websockets


class DanmakuServer:
    def __init__(self):
        # TODO: define your variables needed in this class
        raise NotImplementedError

    async def reply(self, websocket):
        # TODO: design your reply method
        raise NotImplementedError


if __name__ == "__main__":
    server = DanmakuServer()
    asyncio.get_event_loop().run_until_complete(
        websockets.serve(server.reply, 'localhost', 8765))
    asyncio.get_event_loop().run_forever()
