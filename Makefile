APP_NAME = dictionary

# Entry point
SRC_C = dictionary_app.c

# All other source files
SRC_C += dictionary_data.c
SRC_C += dictionary_ui.c
SRC_C += icons/dictionary_10px.c

# Extra includes and defines
CFLAGS += -I.

# Include the core firmware makefile
APP_ROLLBACK_ALLOWED = 1
NO_COMPRESS = 1

all: $(APP_NAME).fap

include $(APP_TEMPLATES_DIR)/app_template.mk