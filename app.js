var app = {
    name: 'app.js',
    startDate: new Date(),
};

// MARK: - Projector

const canvasWidth = 640;
const canvasHeight = 480;
// Math.round(canvasHeight * 640 / width) // Set width
// Math.round(canvasHeight * 480 / height) // Set height

var initializeProjector = function(callback) {
  var net = require('net');
  var client = new net.Socket();
  client.connect(32001, 'localhost', function() {
    callback(null, client);
  });
  client.on('error', function(err) {
    callback(err, client);
  });
};

var lightValues = {
  projectorX: -1,
  projectorY: -1,
  projectorColor: 0,
  projectorShouldClear: 1
};

var boolToString = function(bool) {
  return bool ? "true" : "false";
}

var arrayToString = function(array) {
  var arrayString = '[';
  for (var vertex in vertices) {
    verticesString += vertex + ', ';
  }
  arrayString += ']';
  return arrayString
}

var drawPoint = function(projector, x, y, color, shouldClear) {
  console.log('drawPoint = {' + x + ', ' + y + ', ' + color + ', ' + shouldClear + '}');

  if (projector != null && x != null && y != null && color != null && shouldClear != null) {
    console.log('writing')
    projector.write( '{\"type\": \"point\", \"x\": ' + x + ', \"y\": ' + y
      + ', \"color\": ' + color + ', \"shouldClear\": ' + boolToString(shouldClear) + '}\n');
  }
};

// vertices is expected as an array of [x, y]
// color is expected as [r, g, b, a]
var drawShape = function(projector, vertices, color, shouldFill, shouldClear) {
  console.log('drawShape = {' + vertices + ', ' + color + ', ' + shouldFill + ', ' + shouldClear + '}');

  if (projector != null && vertices != null && color != null && shouldFill != null && shouldClear != null) {
    console.log('writing')
    
    projector.write('{\"type\": \"shape\", \"vertices\": ' + arrayToString(vertices)
      + ', \"color\": ' + color + ', \"shouldFill\": ' + boolToString(shouldFill)
      + ', \"shouldClear\": ' + boolToString(shouldClear) + '}\n');
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
  //   drawPoint(app.projector, c, r++, [0, 0, 255, 1], false);
  //   if (r >= canvasHeight) {
  //     r = 0;
  //     c++;
  //   }
  // }, 500);
  // Draw blue triangle from top left to center to middle left
  var runInt = setInterval(function() {
    drawShape(app.projector, [[0, 0], [canvasWidth / 2, canvasHeight / 2], [0, canvasHeight / 2]], [0, 0, 255, 1], 1, 0);
  }, 5000);
};

