var app = {
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

// MARK: - Projector Client

const canvasWidth = 640;
const canvasHeight = 480;
// Math.round(canvasHeight * 640 / width) // Set width
// Math.round(canvasHeight * 480 / height) // Set height
const ip = 'localhost';
const port = 32001;

var initializeProjector = function(callback) {
  var net = require('net');
  var client = new net.Socket();
  client.connect(port, ip, function() {
    callback(null, client);
  });
  client.on('error', function(err) {
    callback(err, client);
  });
};

// MARK: - Draw Commands

var clearScreen = function(projector) {
  console.log('clearScreen');
  if (projector != null) {
    console.log('writing');
    projector.write('{"command": "clearScreen"}\n');
  }
};

// color is expected as [r, g, b, a]
var drawPoint = function(projector, x, y, color) {
  console.log('drawPoint = {' + x + ', ' + y + ', ' + color + '}');
  if (projector != null && x != null && y != null && color != null) {
    console.log('writing');
    projector.write('{"command": "drawPoint"'
      + ', "x": ' + x
      + ', "y": ' + y
      + ', "color": ' + arrayToString(color) + '}\n');
  }
};

// vertices is expected as an array of [x, y]
// color is expected as [r, g, b, a]
var drawLine = function(projector, vertices, color) {
  console.log('drawLine = {' + vertices + ', ' + color + '}');
  if (projector != null && vertices != null && color != null) {
    console.log('writing');
    projector.write('{"command": "drawLine"'
      + ', "vertices": ' + arrayToString(vertices)
      + ', "color": ' + arrayToString(color) + '}\n');
  }
};

// vertices is expected as an array of [x, y]
// color is expected as [r, g, b, a]
var drawShape = function(projector, vertices, color, shouldFill) {
  console.log('drawShape = {' + vertices + ', ' + color + ', ' + shouldFill + '}');
  if (projector != null && vertices != null && color != null && shouldFill != null) {
    console.log('writing');
    projector.write('{"command": "drawShape"'
      + ', "vertices": ' + arrayToString(vertices)
      + ', "color": ' + arrayToString(color)
      + ', "shouldFill": ' + boolToString(shouldFill) + '}\n');
  }
};

// color is expected as [r, g, b, a]
var drawEllipse = function(projector, centerX, centerY, width, height, color,
    shouldFill) {
  console.log('drawEllipse = {' + centerX + ', ' + centerY + ', ' + width
    + ', ' + height + ', ' + color + '}');
  if (projector != null && centerX != null && centerY != null && width != null
    && height != null && color != null && shouldFill != null) {
    console.log('writing');
    projector.write('{"command": "drawEllipse"'
      + ', "centerX": ' + centerX
      + ', "centerY": ' + centerY
      + ', "width": ' + width
      + ', "height": ' + height
      + ', "color": ' + arrayToString(color)
      + ', "shouldFill": ' + boolToString(shouldFill) + '}\n');
  }
};

// MARK: - Run

initializeProjector(function(err, projector) {
  if (err) {
    console.log('==== projector failed ====');
    console.log(err);
  } else {
    console.log('projector initialized');
    app.projector = projector;
    startDrawLoop();
  }
});

var startDrawLoop = function() {
  // Draw blue points top to bottom, left to right across entire canvas
  // var r = 0;
  // var c = 0;
  // var runInt = setInterval(function() {
  //   drawPoint(app.projector, c, r++, [0, 0, 255, 1]);
  //   if (r >= canvasHeight) {
  //     r = 0;
  //     c++;
  //   }
  // }, 500);

  // Draw two sides of an obtuse blue triangle from top left to center to middle right
  // var runInt = setInterval(function() {
  //   let vertices = [[0, 0], [canvasWidth / 2, canvasHeight / 2], [canvasWidth, canvasHeight / 2]];
  //   drawLine(app.projector, vertices, [0, 0, 255, 1]);
  // }, 5000);
  
  // Draw a filled blue triangle from top left to center to middle left
  // var runInt = setInterval(function() {
  //   drawShape(app.projector, [[0, 0], [canvasWidth / 2, canvasHeight / 2], [0, canvasHeight / 2]], [0, 0, 255, 1], true);
  // }, 5000);

  // Draw a filled blue ellipse in the center of the screen.
  var runInt = setInterval(function() {
    let x = canvasWidth / 2;
    let y = canvasHeight / 2;
    let w = canvasWidth / 2;
    let h = canvasHeight / 2;
    drawEllipse(app.projector, x, y, w, h, [0, 0, 255, 1], true);
  }, 5000);
};
