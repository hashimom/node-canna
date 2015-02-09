// index.js (server)
var http = require("http");
var socketio = require("socket.io");
var fs = require("fs");
var ncanna = require('./build/Release/node_canna.node');

var server = http.createServer(function(req, res) {
    res.writeHead(200, {"Content-Type":"text/html"});
    var output = fs.readFileSync("./index.html", "utf-8");
    res.end(output);
}).listen(process.env.VMC_APP_PORT || 3000);

var io = socketio.listen(server);
var canna = new ncanna.NodeCanna();
canna.Open();

io.sockets.on('connection', function(socket) {
    // 接続時
    socket.on('connect', function() {
        console.log('connect');
        canna.Open();
    });

    // メッセージ受信時
    socket.on('c2s_message', function(data) {
        var convdata = canna.Convert(data.value);
        io.sockets.emit("s2c_message", convdata);
    });

    // 切断時
    socket.on('disconnect', function() {
//        canna.Close();
    });
});

