var app = {
    name: 'app.js',
    startDate: new Date(),
};

// MARK: - Projector Helper Functions

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

var projectorSet = function(projector, x, y, color, shouldClear) {
  console.log('projector = {' + x + ', ' + y + ', ' + color + ', ' + shouldClear + '}');

  if (projector != null && x != null && y != null && color != null && shouldClear != null) {
    console.log('writing')
    projector.write('{\"x\": ' + x + ', \"y\": ' + y + ', \"color\": ' + color + ', \"shouldClear\": ' + shouldClear + '}\n');
  }
};

// MARK: - Run

var canvasWidth = 640;
var canvasHeight = 480;
// Math.round(canvasHeight * 640 / width) // Set width
// Math.round(canvasHeight * 480 / height) // Set height
initializeProjector(function(err, projector) {
  if (err) {
    console.log("==== projector failed ====");
    console.log(err);
  } else {
    console.log("projector initialized");
    app.projector = projector;
    startDrawLoop();
  }
});

var startDrawLoop = function() {
  var i = 0;
  var runInt = setInterval(function() {
    projectorSet(app.projector, i++, i++, 1, 1);
  }, 20);
};

// TODO: Figure out how to get these lines to run ^^

// TODO: Figure out how color should be formatted (i.e. experiment with it -- maybe hex string?)
//       Appears that shirishgoyal set color to blue and only communicates light/no light ...

