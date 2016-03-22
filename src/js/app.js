Pebble.addEventListener('ready', ready); 

Pebble.addEventListener('showConfiguration', function() {
    var url = 'https://dinmas.github.io/pebble-ofh3/settings2.html';
    Pebble.openURL(url);
});

function ready(e) {
    console.log('Pebble JS is ready.');
    apirequest= require('apirequest');
    console.log("app.js" + apirequest.nextTrailFetch());
};
