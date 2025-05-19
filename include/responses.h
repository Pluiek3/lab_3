#ifndef RESPONSES_H
#define RESPONSES_H

#include "../mongoose/mongoose.h"
#include "sleep_logic.h"

void send_html_response(struct mg_connection *c);
void send_json_response(struct mg_connection *c, SleepResult result);

#endif
