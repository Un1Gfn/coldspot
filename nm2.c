#include <NetworkManager.h>

#include "nm2.h"

NMClient *nm_client_new2(){

  NMClient *c=nm_client_new(/*GCancellable*/NULL, /*GError*/NULL); g_assert_true(c);

  _Static_assert(NM_CHECK_VERSION(1,38,2), ""); g_assert_true(0==g_strcmp0("1.38.2", nm_client_get_version(c)));

  g_assert_true(!nm_client_get_startup(c));
  g_assert_true(nm_client_get_nm_running(c));
  g_assert_true((NM_RADIO_FLAG_WLAN_AVAILABLE|NM_RADIO_FLAG_WWAN_AVAILABLE)==nm_client_get_radio_flags(c));

  g_assert_true(nm_client_networking_get_enabled(c));

  return c;

}
