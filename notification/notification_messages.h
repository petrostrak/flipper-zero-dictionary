// Mock notification_messages.h for simulation
#pragma once

#include "../furi.h"

// Define a proper structure for notification sequences
typedef struct {
    int dummy;
} NotificationSequence;

// Function to send notifications
void notification_message(NotificationApp* app, const NotificationSequence* sequence);

// Notification sequences
extern const NotificationSequence sequence_display_backlight_on;
