function timeConversion(ms) {
    var seconds = parseInt(ms / 1000);
    var minutes = parseInt(ms / (1000 * 60));
    var hours = parseInt(ms / (1000 * 60 * 60));
    var days = parseInt(ms / (1000 * 60 * 60 * 24));
    var stime;
    if (seconds < 60) {
        stime = `${seconds} seconds`;
    } else if (minutes < 60) {
        stime = `${minutes} minutes, ${seconds - minutes * 60} seconds`;
    } else if (hours < 24) {
        stime = `${hours} hours, ${minutes - hours * 60} minutes, ${seconds - minutes * 60} seconds`;
    } else {
        stime = `${days} days, ${hours - days * 24} hours, ${minutes - hours * 60} minutes, ${seconds - minutes * 60} seconds`;
    }
    return stime;
};

function parseShortTime(raw_data){
    raw_data = raw_data.toLowerCase();
    var secs = 0;

    var reg = /([0-9][0-9]*?)(d|h|m|s)/g;
    var match;
    while (match = reg.exec(raw_data)) {
        switch(match[2]){
            case "d":
                secs += parseInt(match[1]) * 24 * 60 * 60;
                break;
            case "h":
                secs += parseInt(match[1]) * 60 * 60;
                break;
            case "m":
                secs += parseInt(match[1]) * 60;
                break;
            case "s":
                secs += parseInt(match[1]);
                break;
        }
        reg.lastIndex -= 1;
    }
    return secs;
}

let state = false;

async function ch_state(){
    if(state){
        await fetch("/api/state/off", { method: "POST" });
    }else{
        await fetch("/api/state/on", { method: "POST" });
    }
}

async function set_timer(){
    let text_input = document.getElementById("text_input");
    let formData = new FormData();
    formData.append('time', parseShortTime(text_input.value.toLowerCase()) * 1000);
    await fetch("/api/timer/set", { method: "POST", body: formData });
}

async function clr_timer(){
    await fetch("/api/timer/clear", { method: "POST" });
}

document.addEventListener("DOMContentLoaded", () => {
    let lamp_state = document.getElementById("lamp_state");
    let state_btn = document.getElementById("state_btn");
    let time_remaining = document.getElementById("time_remaining");

    setInterval(async () => {
        let response = await fetch("/api/timer");
        let data = await response.json();
        time_remaining.textContent = timeConversion(data.timer);

        response = await fetch("/api/state");
        data = await response.json();
        state = data.state;
        lamp_state.textContent = data.state ? "ON" : "OFF";
        state_btn.textContent = data.state ? "Disable" : "Enable";
    }, 1000);
});