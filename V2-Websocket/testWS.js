const ws = require("ws");

const validBtns = ["A", "B", "X", "Y"];

const server = new ws.Server({
    port:4200
  });

console.log("Started websocket server");

server.on("connection", socket => {
    socket.on("message", message => {
        if(message.toString()=="bark"){
            console.log("Thy dog hath barketh.");
        }
        else if(message.toString()=="walk"){
            console.log("Thy dog hath walketh");
        }
        else {
            console.log("Your dog is as confused as a non-native in an American law court.")
        }
    })
})