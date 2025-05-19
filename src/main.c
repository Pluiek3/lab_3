#include "../mongoose/mongoose.h"
#include "../include/http_handler.h"
#include <stdio.h>
#include <stdbool.h>

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        handle_request(c, (struct mg_http_message *)ev_data);
    }
}

int main() {
    struct mg_mgr mgr;
    
    // Инициализация менеджера Mongoose
    mg_mgr_init(&mgr); // Исправлено: убран второй аргумент
    
    // Запуск HTTP-сервера
    if (!mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL)) {
        fprintf(stderr, "Failed to start server\n");
        return 1; // Завершение программы при ошибке
    }
    
    printf("Server running on http://localhost:8000\n");
    
    // Основной цикл обработки событий
    while (true) {
        mg_mgr_poll(&mgr, 1000); // Обработка событий с тайм-аутом 1000 мс
    }
    
    // Освобождение ресурсов менеджера (достижимо только при завершении цикла)
    mg_mgr_free(&mgr);
    return 0;
}
