// Mock furi.h for simulation
#pragma once

#include <stdint.h>
#include <stdbool.h>

// Basic types
typedef int FuriStatus;
typedef struct FuriMessageQueue FuriMessageQueue;
typedef struct NotificationApp NotificationApp;

// Status codes
#define FURI_OK 0
#define FURI_LOCKED 1
#define FURI_FAIL 2

// Constants
#define FuriStatusOk 0
#define FuriStatusError 1
#define FuriStatusErrorTimeout 2
#define FuriWaitForever -1

// Assert function - casts pointer to bool for compatibility
void furi_assert(int condition);

// Message queue functions
FuriMessageQueue* furi_message_queue_alloc(uint32_t size, uint32_t item_size);
void furi_message_queue_free(FuriMessageQueue* queue);
int furi_message_queue_get(FuriMessageQueue* queue, void* message, uint32_t timeout);
void furi_message_queue_put(FuriMessageQueue* queue, void* message, uint32_t timeout);

// Record functions
void* furi_record_open(const char* record_name);
void furi_record_close(const char* record_name);

// Record names
#define RECORD_GUI "gui"
#define RECORD_NOTIFICATION "notification"

// Macro for unused parameters
#define UNUSED(x) (void)(x)