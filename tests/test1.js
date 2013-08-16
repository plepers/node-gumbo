
var addon = require('../build/Release/gumbo');

var fs = require('fs');

fs.readFile('page1.html', 'utf8', function (err,data) {
  if (err) {
    return console.log(err);
  }
  console.log("html loaded");
    console.log( addon.add( data ) ); // 'world'
});
