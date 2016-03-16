Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
    var url = 'http://dinmas.github.io/pebble-ofh3/settings.html';
    Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode the user's preferences
    var configData = JSON.parse(decodeURIComponent(e.response));
  // Send to the watchapp via AppMessage
    var dict = {
    'AppKeyDateString': configData.date_string,
    'AppKeyDebug': configData.debug        
    }
    
});

// Send to the watchapp
Pebble.sendAppMessage(dict, function() {
  console.log('Config data sent successfully!');
}, function(e) {
  console.log('Error sending config data!');
});
