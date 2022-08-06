#pragma once

#include <glib.h>

typedef struct {const char *const cc; GType t; } S;
void relationship(gpointer, const S*);
void type_info(const GType);
