var app = {
  name: 'app.js',
  startDate: new Date(),
};

const canvasWidth = 640;
const canvasHeight = 480;
// Math.round(canvasHeight * 640 / width) // Set width
// Math.round(canvasHeight * 480 / height) // Set height

// MARK: - Projector Draw Commands

var commands = ['clearScreen', 'drawPoint', 'drawLine', 'drawShape', 'drawEllipse'];

var clearScreen = function(command) {
  console.log('clearScreen');
  if (app.projector != null) {
    console.log('writing');
    app.projector.write(JSON.stringify(command) + '\n');
    return true;
  } else {
    return false;
  }
};

// x, y are expected as integers
// color is expected as [r, g, b, a]
var drawPointParams = ['x', 'y', 'color'];
var drawPoint = function(command) {
  if (app.projector != null
      && command['x'] != null
      && command['y'] != null
      && command['color'] != null) {
    console.log('writing');
    app.projector.write(JSON.stringify(command) + '\n');
    return true;
  } else {
    return false;
  }
};

// vertices is expected as an array of [x, y]
// color is expected as [r, g, b, a]
var drawLineParams = ['vertices', 'color'];
var drawLine = function(command) {
  console.log('drawLine');
  if (app.projector != null
      && command['vertices'] != null
      && command['color'] != null) {
    console.log('writing');
    app.projector.write(JSON.stringify(command) + '\n');
    return true;
  } else {
    return false;
  }
};

// vertices is expected as an array of [x, y]
// color is expected as [r, g, b, a]
// shouldFill is expected as a boolean
var drawShapeParams = ['vertices', 'color', 'shouldFill'];
var drawShape = function(command) {
  console.log('drawShape');
  if (app.projector != null
      && command['vertices'] != null
      && command['color'] != null
      && command['shouldFill'] != null) {
    console.log('writing');
    app.projector.write(JSON.stringify(command) + '\n');
    return true;
  } else {
    return false;
  }
};

// centerX, centerY, width, heighe are expected as integers
// color is expected as [r, g, b, a]
// shouldFill is expected as a boolean
var drawShapeParams = ['centerX', 'centerY', 'width', 'height', 'color', 'shouldFill'];
var drawEllipse = function(command) {
  console.log('drawEllipse');
  if (app.projector != null
      && command['centerX'] != null
      && command['centerY'] != null
      && command['width'] != null
      && command['height'] != null
      && command['color'] != null
      && command['shouldFill'] != null) {
    console.log('writing');
    app.projector.write(JSON.stringify(command) + '\n');
    return true;
  } else {
    return false;
  }
};

// MARK: - JSON Helpers

var arrayToString = function(array) {
  var arrayString = '[';
  for (var i = 0; i < array.length; i++) {
    if (Array.isArray(array[i])) {
      arrayString += arrayToString(array[i]);
    } else {
      arrayString += array[i];
    }
    if (i < array.length -1) {
      arrayString += ', ';
    }
  }
  arrayString += ']';
  return arrayString;
}

// MARK: - Initialization

var runLoop = function() {
  var runInt = setInterval(function() {
    console.log('alive');
  }, 5000);
};

const projectorIp = 'localhost';
const projectorPort = 32001;
var net = require('net');
app.projector = new net.Socket()
  .connect(projectorPort, projectorIp, function() {
    console.log('projector connected');
    runLoop();
  })
  .on('error', function(error) {
    console.log('==== projector failed to connect ====');
    console.log(error);
  });

// MARK: - Socket.IO

var expressApp = require('express')();
var server = require('http').Server(expressApp);
var io = require('socket.io')(server);

const projectorServerPort = 32003;
server.listen(projectorServerPort);

expressApp.get('/', function (req, res) {
  res.sendfile(__dirname + '/index.html');
});

io.on('connection', function (socket) {
  console.log('socket-io: connection');
  socket.emit('reply', { message: ':: welcome to euglenalab-projector-server ::' });
  if (!app.projector) {
    socket.emit('reply', { message: 'warning: projector is down' });
  }
  socket.on('command', function (data) {
    console.log('socket-io: command - ' + JSON.stringify(data, null, 2));
    if (!app.projector) {
      socket.emit('reply', { error: 'projector is down' });
    } else if (!data) {
      socket.emit('reply', { error: 'no payload found' });
    } else if (!data['command']) {
      socket.emit('reply', { error: 'command must be one of '
        + arrayToString(commands) });
    } else {
      switch (data['command']) {
        case 'clearScreen':
          if (clearScreen(data['command'])) {
            socket.emit('reply', { message: 'clearScreen command received' });
          } else {
            socket.emit('reply', { error: 'clearScreen command - unable to execute' });
          }
          break;
        case 'drawPoint':
          if (drawPoint(data['command'])) {
            socket.emit('reply', { message: 'drawPoint command received' });
          } else {
            socket.emit('reply', { error: 'drawPoint command requires '
              + arrayToString(drawPointParams) });
          }
          break;
        case 'drawLine':
          if (drawLine(data['command'])) {
            socket.emit('reply', { message: 'drawLine command received' });
          } else {
            socket.emit('reply', { error: 'drawLine command requires '
              + arrayToString(drawLineParams) });
          }
          break;
        case 'drawShape':
          if (drawShape(data['command'])) {
            socket.emit('reply', { message: 'drawShape command received' });
          } else {
            socket.emit('reply', { error: 'drawShape command requires '
              + arrayToString(drawShapeParams) });
          }
          break;
        case 'drawEllipse':
          if (drawEllipse(data['command'])) {
            socket.emit('reply', { message: 'drawEllipse command received' });
          } else {
            socket.emit('reply', { error: 'drawEllipse command requires '
              + arrayToString(drawEllipseParams) });
          }
          break;
        default:
          socket.emit('reply', { error: 'command must be one of '
            + arrayToString(commands) });
      }
    }
  });
});
