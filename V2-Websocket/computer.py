from XInput import * 
from os import system
import time, websockets, asyncio, json



pos1 = 90
pos2 = 90


async def sendSrvMsg(btn, server):
    await server.send(btn)
    response = await server.recv()
    pos1 = await server.recv()
    pos2 = await server.recv()
    pos1 = int(pos1)
    pos2 = int(pos2)
    system("cls")
    print("-------------------------------")
    print(f"Position 1: {pos1}")
    print(f"Position 2: {pos2}")
    

async def controllerCheck(server):
    joystick_threshold = 0.2
    joystick_sens = 30
    buttonTranslation = {"A": "A", "B": "B", "LEFT_SHOULDER": "LT", "RIGHT_SHOULDER": "RT"}
    joystickIDS = ["L", "R"]
    controller = get_state(0)
    leftThumb = get_thumb_values(controller)[0]
    rightThumb = get_thumb_values(controller)[1]
    if leftThumb[0]!=0:
        await sendSrvMsg(f"LX{int(leftThumb[0]*joystick_sens)}", server)
    if leftThumb[1]!=0:
        await sendSrvMsg(f"LY{int(leftThumb[1]*joystick_sens)}", server)
    events = get_events()
    for event in events:
        if event.type==3:
            if event.button=="A":
                await sendSrvMsg("LX-180", server)
            elif event.button=="B":
                await sendSrvMsg("LY-180", server)

async def main():
    serverURL = "ws://192.168.1.91:420"
    async with websockets.connect(serverURL) as server:
        while True:
            task1 = loop.create_task(controllerCheck(server))
            await asyncio.wait([task1])



loop = asyncio.get_event_loop()
loop.run_until_complete(main())
