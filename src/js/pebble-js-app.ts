var Pebble:any;
var OWNER = "scottopell";
var REPO  = "todo";
var todoUrl = "http://api.github.com/repos/" + OWNER + "/" + REPO + "/issues";
var PADDING = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

interface Issue{
  id: number;
  url: string;
  html_url: string;
  number: number;
  state: string;
  title: string;
  body: string;
  created_at: string;
  updated_at: string;
}

function ajax( url: string,
               method: string,
               data: any,
               callback: (data: any) => void) {

  var req = new XMLHttpRequest();
  req.open(method, url, true);
  req.responseType = 'json';
  req.onload = function(e) {
    if (req.readyState === 4 && req.status === 200) {
      if (req.status === 200) {
        callback(req.response);
      } else {
        console.log("HTTP Error: " + JSON.stringify(req));
      }
    }
  };
  req.send(data);
}

function getTopLevelIssues( callback: (issues: Issue[]) => void) {
  console.log(todoUrl);
  ajax(todoUrl, 'get', null, callback);
}

// Set callback for appmessage events
Pebble.addEventListener("appmessage", function(e) {
  console.log("message");
  var isInitMsg;
  if (e.payload.TICKER) {
    isInitMsg = true;
  }
});

Pebble.addEventListener("ready", function(e) {
  getTopLevelIssues( function (issues: Issue[]){
    issues.forEach(function(issue: Issue){
      console.log(issue.title + " : " + issue.state);
      if (issue.state === "open"){
        var updated: string = "" + Math.floor(new Date(issue.updated_at).getTime() / 1000);
        var msg: string = (updated + PADDING).slice(-15) + issue.title;
        Pebble.sendAppMessage({ new_issue: msg});
      }
    });
  });
});
