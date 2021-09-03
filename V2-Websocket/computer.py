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
        response = await server.recv()
        pos1 = await server.recv()
        pos2 = await server.recv()
        print("-------------------------------")
        print(f"Position 1: {pos1}")
        print(f"Position 2: {pos2}")


async def controllerCheck():
    joystick_threshold = 0.2
    joystick_sens = 45
    buttonTranslation = {"A": "A", "B": "B", "LEFT_SHOULDER": "LT", "RIGHT_SHOULDER": "RT"}
    joystickIDS = ["L", "R"]
    events = get_events()
    for event in events:
        if event.type == 3:
            try:
                await sendSrvMsg(buttonTranslation[event.button])
            except KeyError:
                print("Invalid input")
        elif event.type == 6:
            if event.value>joystick_threshold or (event.value*-1)>joystick_threshold:
                joystick = joystickIDS[event.stick]
                print(f"{joystick}X{int(event.x*joystick_sens)}")
                print(f"{joystick}Y(int(event.y*joystick_sens)}")

    await asyncio.sleep(0.0001)

async def main():
    while True:
        task1 = loop.create_task(controllerCheck())
        await asyncio.wait([task1])



loop = asyncio.get_event_loop()
loop.run_until_complete(main())
