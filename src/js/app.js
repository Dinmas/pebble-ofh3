Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
    var url = 'http://dinmas.github.io/pebble-ofh3/settings.html';
    Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
    var configData = JSON.parse(decodeURIComponent(e.response));
    
    var dict = {
    'AppKeyDateString': configData.date_string,
    'AppKeyDebug': configData.debug        
    };
        
    Pebble.sendAppMessage(dict,function() {
      console.log('Config sent successfully!');
    }, function(e) {
      console.log('Failed to send config!');
    });
});
   