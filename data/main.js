let loc = window.location;
let socket;

function connect() {
    if(!socket){
        socket = new WebSocket("ws://" + loc.host + "/socket");

        socket.onopen = function () {
          alert("Connected.");
        };
        
        socket.onclose = function (event) {
          alert("Disconnected. Code: " + event.code + " Reason: " + event.reason);
        };
        
        socket.onerror = function (error) {
          alert("Error: " + error);
        };
    }else{
        socket?.close();
        socket = undefined;
    }
}
function sendmessage() {
    if(socket && socket.readyState === 1){
        let text = document.getElementById("text_input").value;
        let repeat = parseInt(document.getElementById("repeat_input").value);
        socket.send(JSON.stringify({
            op: "say",
            txt: text,
            rpt: repeat || undefined
        }));
    }else{
       alert("No connection estabilished!"); 
    }
}