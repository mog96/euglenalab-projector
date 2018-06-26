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
        // callback(err, client);
    });
};

var lightValues = {
    projectorX: -1,
    projectorY: -1,
    projectorColor: 0,
    projectorAlpha: 1
};

var projectorSet = function(projector, x, y, color, alpha) {
    console.log('projector = {' + x + ', ' + y + ', ' + color + ', ' + alpha + '}');

    if (projector != null && x != null && y != null && color != null && alpha != null) {
        projector.write('{\"x\": ' + x + ', \"y\": ' + y + ', \"color\": ' + color + ', \"alpha\": ' + alpha + '}\n');
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
        app.projector = projector;
    }
});
projectorSet(app.projector, 0, 0, 0, 1);
