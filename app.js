var projector = {
  name: 'app.js',
  startDate: new Date(),
};

// MARK: - JSON Helpers

var boolToString = function(bool) {
  return bool ? "true" : "false";
}

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

// MARK: - Draw Commands

var clearScreen = function(ofApp) {
  console.log('clearScreen');
  if (ofApp != null) {
    console.log('writing');
    ofApp.write('{"command": "clearScreen"}\n'); // \n marks end of message
  }
};

// color is expected as [r, g, b, a]
var drawPoint = function(ofApp, x, y, color) {
  console.log('drawPoint = {' + x + ', ' + y + ', ' + color + '}');
  if (ofApp != null && x != null && y != null && color != null) {
    console.log('writing');
    ofApp.write('{"command": "drawPoint"'
      + ', "x": ' + x
      + ', "y": ' + y
      + ', "color": ' + arrayToString(color) + '}\n');
  }
};

// vertices is expected as an array of [x, y]
// color is expected as [r, g, b, a]
var drawLine = function(ofApp, vertices, color) {
  console.log('drawLine = {' + vertices + ', ' + color + '}');
  if (ofApp != null && vertices != null && color != null) {
    console.log('writing');
    ofApp.write('{"command": "drawLine"'
      + ', "vertices": ' + arrayToString(vertices)
      + ', "color": ' + arrayToString(color) + '}\n');
  }
};

// vertices is expected as an array of [x, y]
// color is expected as [r, g, b, a]
var drawShape = function(ofApp, vertices, color, shouldFill) {
  console.log('drawShape = {' + vertices + ', ' + color + ', ' + shouldFill + '}');
  if (ofApp != null && vertices != null && color != null && shouldFill != null) {
    console.log('writing');
    ofApp.write('{"command": "drawShape"'
      + ', "vertices": ' + arrayToString(vertices)
      + ', "color": ' + arrayToString(color)
      + ', "shouldFill": ' + boolToString(shouldFill) + '}\n');
  }
};

// color is expected as [r, g, b, a]
var drawEllipse = function(ofApp, centerX, centerY, width, height, color,
    shouldFill) {
  console.log('drawEllipse = {' + centerX + ', ' + centerY + ', ' + width
    + ', ' + height + ', ' + color + '}');
  if (ofApp != null && centerX != null && centerY != null && width != null
    && height != null && color != null && shouldFill != null) {
    console.log('writing');
    ofApp.write('{"command": "drawEllipse"'
      + ', "centerX": ' + centerX
      + ', "centerY": ' + centerY
      + ', "width": ' + width
      + ', "height": ' + height
      + ', "color": ' + arrayToString(color)
      + ', "shouldFill": ' + boolToString(shouldFill) + '}\n');
  }
};

// MARK: - Initialization

const canvasWidth = 640;
const canvasHeight = 480;
// Math.round(canvasHeight * 640 / width) // Set width
// Math.round(canvasHeight * 480 / height) // Set height
const projectorIp = 'localhost';
const projectorPort = 32001;

var initializeProjector = function(callback) {
  var net = require('net');
  var client = new net.Socket();
  client.connect(projectorPort, projectorIp, function() {
    callback(null, client);
  });
  client.on('error', function(err) {
    callback(err, client);
  });
};

var runLoop = function() {
  // Draw blue points top to bottom, left to right across entire canvas
  // var r = 0;
  // var c = 0;
  // var runInt = setInterval(function() {
  //   drawPoint(projector.ofApp, c, r++, [0, 0, 255, 1]);
  //   if (r >= canvasHeight) {
  //     r = 0;
  //     c++;
  //   }
  // }, 500);

  // Draw two sides of an obtuse blue triangle from top left to center to middle right
  // var runInt = setInterval(function() {
  //   let vertices = [[0, 0], [canvasWidth / 2, canvasHeight / 2], [canvasWidth, canvasHeight / 2]];
  //   drawLine(projector.ofApp, vertices, [0, 0, 255, 1]);
  // }, 5000);
  
  // Draw a filled blue triangle from top left to center to middle left
  // var runInt = setInterval(function() {
  //   drawShape(projector.ofApp, [[0, 0], [canvasWidth / 2, canvasHeight / 2], [0, canvasHeight / 2]], [0, 0, 255, 1], true);
  // }, 5000);

  // Draw a filled blue ellipse in the center of the screen.
  // var runInt = setInterval(function() {
  //   let x = canvasWidth / 2;
  //   let y = canvasHeight / 2;
  //   let w = canvasWidth / 2;
  //   let h = canvasHeight / 2;
  //   drawEllipse(projector.ofApp, x, y, w, h, [0, 0, 255, 1], true);
  // }, 5000);

  var runInt = setInterval(function() {
    console.log('alive');
  }, 5000);
};

initializeProjector(function(err, ofApp) {
  if (err) {
    console.log('==== projector failed to connect ====');
    console.log(err);
  } else {
    console.log('projector connected');
    projector.ofApp = ofApp;
    runLoop();
  }
});

var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);

const projectorServerPort = 32003;
server.listen(projectorServerPort);

app.get('/', function (req, res) {
  res.sendfile(__dirname + '/index.html');
});

// MARK: - Socket.IO Commands

io.on('connection', function (socket) {
  console.log('socket-io: connection');
  socket.emit('reply', { message: ':: welcome to euglenalab-projector-server ::' });
  socket.on('clearScreen', function (data) {
    console.log('socket-io: clearScreen - ' + JSON.stringify(data, null, 2));
    clearScreen(projector.ofApp);
    socket.emit('reply', { message: 'screen cleared' });
  });
});
