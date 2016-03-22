//Making XMLHttpRequest to Google Calendar API for OFH3 calendar. 
function nextTrailFetch() {
    var method = "GET";
    var apiKey = "AIzaSyDVeiQlGX-7uvtmeGhDuNqTnG19USMKscQ";
    var url = "https://www.googleapis.com/calendar/v3/calendars/an1unkotcvke952c0ugledrafg%40group.calendar.google.com/events?maxResults=25&orderBy=startTime&singleEvents=true&timeMin=" + (new Date()).toISOString() + "&key=" + apiKey ;
    
    console.log("Request URL:" + url);
        
    function debugListener() {
        this.responseType = "json";        
        console.log(this.response);
        console.log(this.responseXML);
        
    }
    
    function debugError() {
        console.log("An error occured: " + this.statusText);
    }
    
    var request = new XMLHttpRequest(); 
    
    request.addEventListener("load",debugListener);
    request.addEventListener("error",debugError);
    
    request.open(method,url);   
    request.send(); 
}

module.exports.nextTrailFetch = nextTrailFetch