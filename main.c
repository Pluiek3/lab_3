#include "./mongoose/mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Константы для путей к файлам
#define PATH_INDEX_HTML "html/index.html"
#define PATH_STYLES_CSS "html/styles.css"

// Константы для типов контента
#define CONTENT_TYPE_HTML "text/html; charset=utf-8"
#define CONTENT_TYPE_CSS "text/css"
#define CONTENT_TYPE_JSON "application/json"

// Функция для чтения файла
static char *read_file(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) return NULL;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *content = malloc(size + 1);
    fread(content, 1, size, fp);
    content[size] = '\0';
    
    fclose(fp);
    return content;
}

// Функция для вычисления продолжительности сна
static void calculate_sleep(const char *bedtime_str, const char *waketime_str, 
                           int *hours, int *minutes, char *comment) {
    struct tm bedtime = {0}, waketime = {0};
    
    // Парсим время отхода ко сну
    strptime(bedtime_str, "%H:%M", &bedtime);
    
    // Парсим время пробуждения
    strptime(waketime_str, "%H:%M", &waketime);
    
    // Преобразуем в time_t (секунды с эпохи)
    time_t bt = mktime(&bedtime);
    time_t wt = mktime(&waketime);
    
    // Если время пробуждения раньше времени отхода ко сну, 
    // значит сон перешел через полночь
    if (wt <= bt) {
        wt += 24 * 3600; // Добавляем 24 часа
    }
    
    // Вычисляем разницу в секундах
    double diff = difftime(wt, bt);
    
    // Переводим в часы и минуты
    *hours = (int)(diff / 3600);
    *minutes = (int)((diff - (*hours * 3600)) / 60;
    
    int total_minutes = *hours * 60 + *minutes;
    if (total_minutes < 240) {       
        strcpy(comment, "Маловато! Вас случайно не Жанна звать? ");
    } else if (total_minutes < 360) { 
        strcpy(comment, "Мало. Старайтесь спать 7-9 часов.");
    } else if (total_minutes < 540) { // 7-9 часов
        strcpy(comment, "Нормально! Это рекомендуемая продолжительность сна.");
    } else if (total_minutes < 660) { // 9-11 часов
        strcpy(comment, "Очень хорошо! Вы хорошо высыпаетесь.");
    } else {                          // Более 11 часов
        strcpy(comment, "Слишком много! Возможно, у вас пересып.");
    }
}

// Обработчик HTTP-запросов
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        
        // Обработка GET-запроса на главную страницу
        if (mg_http_match_uri(hm, "/")) {
            char *html = read_file(PATH_INDEX_HTML);
            mg_http_reply(c, 200, CONTENT_TYPE_HTML, "%s", html ? html : "Error loading page");
            free(html);
        }
        // Обработка GET-запроса для CSS
        else if (mg_http_match_uri(hm, "/styles.css")) {
            char *css = read_file(PATH_STYLES_CSS);
            mg_http_reply(c, 200, CONTENT_TYPE_CSS, "%s", css ? css : "");
            free(css);
        }
        // Обработка POST-запроса с данными формы
        else if (mg_http_match_uri(hm, "/calculate") && mg_vcasecmp(&hm->method, "POST") == 0) {
            char bedtime[6] = {0}, waketime[6] = {0};
            
            // Извлекаем параметры из тела запроса
            mg_http_get_var(&hm->body, "bedtime", bedtime, sizeof(bedtime));
            mg_http_get_var(&hm->body, "waketime", waketime, sizeof(waketime));
            
            // Вычисляем продолжительность сна
            int hours, minutes;
            char comment[256];
            calculate_sleep(bedtime, waketime, &hours, &minutes, comment);
            
            // Формируем JSON-ответ
            mg_http_reply(c, 200, CONTENT_TYPE_JSON, 
                "{\"status\":\"ok\",\"hours\":%d,\"minutes\":%d,\"total_minutes\":%d,\"comment\":\"%s\"}",
                hours, minutes, hours * 60 + minutes, comment);
        }
        // Все остальные запросы
        else {
            mg_http_reply(c, 404, CONTENT_TYPE_HTML, "Not Found");
        }
    }
}

int main(void) {
    struct mg_mgr mgr;                          // Менеджер событий
    mg_mgr_init(&mgr);                          // Инициализация
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);  // Создание HTTP-сервера
    printf("Sleep calculator server running on http://localhost:8000\n");
    
    for (;;) mg_mgr_poll(&mgr, 1000);           // Бесконечный цикл обработки событий
    mg_mgr_free(&mgr);                          // Освобождение ресурсов (недостижимо)
    return 0;
}
