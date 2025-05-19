#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "../mongoose/mongoose.h"

// Обрабатывает входящий HTTP-запрос
// Параметры:
// c - указатель на соединение
// hm - указатель на HTTP-сообщение
void handle_request(struct mg_connection *c, struct mg_http_message *hm);

#endif // HTTP_HANDLER_H
