var Pebble;
var OWNER = "scottopell";
var REPO = "todo";
var todoUrl = "http://api.github.com/repos/" + OWNER + "/" + REPO + "/issues";
var PADDING = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
function ajax(url, method, data, callback) {
    var req = new XMLHttpRequest();
    req.open(method, url, true);
    req.responseType = 'json';
    req.onload = function (e) {
        if (req.readyState === 4 && req.status === 200) {
            if (req.status === 200) {
                callback(req.response);
            }
            else {
                console.log("HTTP Error: " + JSON.stringify(req));
            }
        }
    };
    req.send(data);
}
function getTopLevelIssues(callback) {
    console.log(todoUrl);
    ajax(todoUrl, 'get', null, callback);
}
// Set callback for appmessage events
Pebble.addEventListener("appmessage", function (e) {
    console.log("message");
    var isInitMsg;
    if (e.payload.TICKER) {
        isInitMsg = true;
    }
});
Pebble.addEventListener("ready", function (e) {
    getTopLevelIssues(function (issues) {
        issues.forEach(function (issue) {
            console.log(issue.title + " : " + issue.state);
            if (issue.state === "open") {
                var updated = "" + Math.floor(new Date(issue.updated_at).getTime() / 1000);
                var msg = (updated + PADDING).slice(-15) + issue.title;
                Pebble.sendAppMessage({ new_issue: msg });
            }
        });
    });
});
