var express = require('express');
var app = express();
var path = require('path');
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = process.env.PORT || 3000;
var fs = require('fs');

var WebSocket = require('ws');
const WS_PORT1  = 8888;
const WS_PORT2  = 8889;

const wsServer1 = new WebSocket.Server({port: WS_PORT1}, ()=> console.log(`WS Server is listening at ${WS_PORT1}`));
const wsServer2 = new WebSocket.Server({port: WS_PORT2}, ()=> console.log(`WS Server is listening at ${WS_PORT2}`));


let connectedClients = [];
wsServer1.on('connection', (ws1, req)=>{
    console.log('Connected');
    connectedClients.push(ws1);

    ws1.on('message', data => {
        connectedClients.forEach((ws1,i)=>{
            if(ws1.readyState === ws1.OPEN){
                ws1.send(data);
                //console.log(data);
            }else{
                connectedClients.splice(i ,1);
            }
        })
    });
});

wsServer2.on('connection', (ws2, req)=>{
    console.log('Connected');
    connectedClients.push(ws2);

    ws2.on('message', data => {
        connectedClients.forEach((ws2,i)=>{
            if(ws2.readyState === ws2.OPEN){
                ws2.send(data);
                //console.log(data);
            }else{
                connectedClients.splice(i ,1);
            }
        })
    });
});

var cmdA = '';
var cmd = '';

var readJSON = function(local){
    let string = fs.readFileSync(local)
    let object = JSON.parse(string)
    return object;
}

var writeJSON = function(obj, local){
    fs.writeFileSync(local, JSON.stringify(obj))
    console.log('Gravado:')
    console.log(obj)
    console.log('Em:')
    console.log(local)
}

server.listen(port, () => {
    console.log('Server listening at port %d', port);
});

app.use(express.static(path.join(__dirname, 'public')));
//app.get('/',(req,res)=>res.sendFile(path.join(__dirname, './public/index.html')));

io.on('connection', (socket) => {
    let interval = setInterval(() => {
        
        if (cmd != ''){
            //console.log(cmd);
            socket.emit('ESP32_msg', cmd)
            //cmdA = cmd;            
        }
        //console.log(msg);
        
    }, 10);//Tempo entre chamadas
  
    console.log('novo client com o id:', socket.id)

    socket.on('update', (msg) => {
    //console.log('update')
        socket.emit('update', readJSON('db/state.json'))
    })

    socket.on('move', (msg) => {
        console.log(msg)
        //socket.emit('arduino_cmd', msg)
        //writeJSON({cmd: msg}, 'sv_arduino/tmp.json')
        cmd = msg

    })

    socket.on('init_msg_esp32', (msg) => {
        console.log(msg)
    })

    socket.emit('msg', 'eu, "O servidor", digo que deixo você ficar connectado')

    socket.on('disconnect', () => {
        console.log('foi desconectado o client: ', socket.id)
    });
});