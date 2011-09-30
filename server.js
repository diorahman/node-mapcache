/**
 * Set up GeoCache as a server
 *
 * This can be tested after running it using:
 *
 * curl 'http://localhost:3000/?LAYERS=aerial-photos&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&FORMAT=image%2Fjpeg&SRS=EPSG%3A27700&BBOX=431380.11079545,89627.893687851,432044.11115055,89950.632057879&WIDTH=1398&HEIGHT=679'
 */

var url = require('url');
var geocache = require('./build/default/bindings.node');
var http = require('http');
var path = require('path');

var conffile = path.resolve('./geocache.xml');
var service = new geocache.GeoCache(conffile);

http.createServer(function (req, res) {
    var baseUrl = "http://localhost:3000";
    var urlParts = url.parse(req.url);
    var params = urlParts.query || '';
    var pathInfo = urlParts.pathname || "/";
    
    service.get(baseUrl, pathInfo, params, function (err, response) {
        if (err) {
            res.writeHead(500);
            res.end(err.stack);
            console.error(err.stack);
            return;
        }

        var header, i, headers = {};
        for (header in response.headers) {
            if (response.headers.hasOwnProperty(header)) {
                values = response.headers[header];
                headers[header] = values[0]; // get the first header value
            }
        }
        res.writeHead(response.code, headers);
        res.end(response.data);
        console.log('Serving ' + req.url);
    });    
}).listen(3000, "localhost");
console.log('Server running at http://localhost:3000/');
