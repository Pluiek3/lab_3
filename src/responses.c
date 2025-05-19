#include "../include/responses.h"
#include <stdio.h>

void send_html_response(struct mg_connection *c) {
    mg_http_reply(
        c, 200,
        "Content-Type: text/html\r\n",
        "<!DOCTYPE html><html><head><title>Калькулятор сна</title>"
        "<style>body{font-family:Arial;max-width:500px;margin:0 auto;padding:20px}"
        "input,button{padding:8px;margin:5px 0}.result{margin-top:20px;padding:10px}"
        "</style></head><body>"
        "<h1>Калькулятор сна</h1>"
        "<form onsubmit=\"calculateSleep(event)\">"
        "<label>Время отхода ко сну: <input type='time' name='bedtime' required></label><br>"
        "<label>Время пробуждения: <input type='time' name='waketime' required></label><br>"
        "<button type='submit'>Рассчитать</button>"
        "</form>"
        "<div id='result' class='result'></div>"
        "<script>"
        "function calculateSleep(e){e.preventDefault();"
        "fetch(`/calculate?${new URLSearchParams(new FormData(e.target))}`)"
        ".then(r=>r.json()).then(d=>{"
        "document.getElementById('result').innerHTML=`"
        "<h3>Результат:</h3><p>Вы спали <strong>${d.hours}ч ${d.minutes}мин</strong></p>"
        "<p>${d.comment}</p>`;})}"
        "</script></body></html>"
    );
}

void send_json_response(struct mg_connection *c, SleepResult result) {
    mg_http_reply(
        c, 200,
        "Content-Type: application/json\r\n",
        "{\"hours\":%d,\"minutes\":%d,\"comment\":\"%s\"}",
        result.hours, result.minutes, result.comment
    );
}
