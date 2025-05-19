#include "../mongoose/mongoose.h"
#include "../include/http_handler.h"
#include "../include/responses.h"
#include "../include/sleep_logic.h"
#include "../include/time_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


static bool uri_matches(const struct mg_str *uri, const char *pattern) {
    size_t n = strlen(pattern);
    if (uri->len == n && memcmp(uri->buf, pattern, n) == 0) {
        return true;
    }
    return false;
}


// Обработка входящих HTTP-запросов
void handle_request(struct mg_connection *c, struct mg_http_message *hm) {
    // Проверка, совпадает ли URI с "/calculate"
    if (uri_matches(&hm->uri, "/calculate")) {
        char bedtime[16] = {0}, waketime[16] = {0};
        
        // Получение параметров запроса "bedtime" и "waketime"
        mg_http_get_var(&hm->query, "bedtime", bedtime, sizeof(bedtime));
        mg_http_get_var(&hm->query, "waketime", waketime, sizeof(waketime));
        
        // Проверка, что оба значения были получены
        if (bedtime[0] && waketime[0]) {
            SleepData data = {
                .bedtime = parse_time(bedtime), // Предполагается, что parse_time возвращает правильный тип
                .waketime = parse_time(waketime)
            };
            SleepResult result = calculate_sleep(data); // Убедитесь, что calculate_sleep возвращает правильный тип
            send_json_response(c, result); // Убедитесь, что send_json_response принимает правильные аргументы
            return;
        }
    }
    send_html_response(c); // Обработка случая, если URI не совпадает
}
