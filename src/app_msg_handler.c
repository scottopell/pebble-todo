#include <pebble.h>
#include <string.h>
#include "common.h"

extern char issues[MAX_ISSUES][sizeof(issue_t)];
extern void issue_received(issue_t);

enum {
  NEW_ISSUE_KEY = 0x0,
};

static char *translate_error(AppMessageResult result);

static void out_sent_handler( DictionaryIterator *sent,
                              void *context ) {
  // outgoing message was delivered
  APP_LOG(APP_LOG_LEVEL_DEBUG, "message sent\n" );
}

static void out_failed_handler( DictionaryIterator * failed,
                                AppMessageResult reason,
                                void * context ) {
  // outgoing message failed
  APP_LOG(APP_LOG_LEVEL_DEBUG, "outbox failure - %s\n", translate_error(reason) );
  // retry on failure
  app_message_outbox_send();
}

static void in_received_handler( DictionaryIterator * received,
                                 void * context ) {
  // incoming message received
  APP_LOG(APP_LOG_LEVEL_DEBUG, "MESSAGE RECIEVED\n" );

  Tuple * issue_tuple = dict_find( received, NEW_ISSUE_KEY );
  if (issue_tuple != NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "GOT A VALUE, length %d", issue_tuple->length );
    APP_LOG(APP_LOG_LEVEL_DEBUG, "GOT A VALUE, val %d", *(issue_tuple->value->data) );
    issue_t issue;
    strncpy(issue.title, (issue_tuple->value->cstring + 15), sizeof(issue.title));
    char buf[15];
    memcpy(buf, issue_tuple->value->cstring, 15);
    issue.timestamp = atoi(buf);

    (*issue_received)(issue);
  }

}

static void in_dropped_handler( AppMessageResult reason, void * context ) {
  // incoming message dropped
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Dropped incoming message\n" );
}

void app_msg_init(){
  /* register message handlers  */
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_sent(out_sent_handler);
  app_message_register_outbox_failed(out_failed_handler);

  /* message handling init */
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);
}

static char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}
