#pragma once

#include <NetworkManager.h>

NMClient *nm2_init();
GPtrArray *nm2_getwificons(NMClient*);
void nm2_autoconnect_toggle(NMClient*, NMConnection*, const gboolean);
void nm2_wireless_disable(NMClient **const);
