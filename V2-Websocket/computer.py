from XInput import * 
import time
import websockets
import asyncio
import json

serverURL = "ws://192.168.1.91:420"


async def sendSrvMsg(btn):
    async with websockets.connect(serverURL) as server:
        await server.send(btn)
        print(f"Sent button: {btn}")


async def controllerCheck():
    events = get_events()
    for event in events:
        if event.type == 3:
            if event.button == "A":
                await sendSrvMsg("A")
            elif event.button == "B":
                await sendSrvMsg("B")
            else:
                print("ooga booga monke no know how button work")
    await asyncio.sleep(0.0001)

async def main():
    while True:
        task1 = loop.create_task(controllerCheck())
        await asyncio.wait([task1])

loop = asyncio.get_event_loop()
loop.run_until_complete(main())