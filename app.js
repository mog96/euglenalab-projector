var app = {
    //Basic
    name:'app.js',
    startDate:new Date(),
};

var initializeProjector = function(cb_fn) {
    var net = require('net');
    var client = new net.Socket();
    client.connect(32001, 'localhost', function() {
        cb_fn(null, client);
    });
    client.on('error', function(err) {
        cb_fn(err, client);
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

initializeProjector(function(err, projector) {
    if (err) {
        console.log("==== projector failed ====");
        console.log(err);
        // err = fName + ' initializeProjector ' + err;
        // return callback(err);
    } else {
        app.projector = projector;
    }
});
var canvasWidth = 640;
var canvasHeight = 480;
var projectorSetObj = {
    sentTime: new Date().getTime(),
    projectorX: Math.round(canvasHeight * 640 / width),
    projectorY: Math.round(canvasHeight * 480 / height)
}
projectorSet(projectorSetObj, 0, 0);
