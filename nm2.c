#include <NetworkManager.h>

#include "nm2.h"

NMClient *nm2_init(){

  NMClient *c=nm_client_new(/*GCancellable*/NULL, /*GError*/NULL); g_assert_true(c);

  _Static_assert(NM_CHECK_VERSION(1,38,2), ""); g_assert_true(0==g_strcmp0("1.38.2", nm_client_get_version(c)));

  g_assert_true(!nm_client_get_startup(c));
  g_assert_true(nm_client_get_nm_running(c));
  g_assert_true((NM_RADIO_FLAG_WLAN_AVAILABLE|NM_RADIO_FLAG_WWAN_AVAILABLE)==nm_client_get_radio_flags(c));

  g_assert_true(nm_client_networking_get_enabled(c));

  return c;

}

// Returns: (transfer full) (element-type NMConnection): array of all wifi connections
GPtrArray *nm2_getwificons(NMClient *c){
  GPtrArray *r=NULL;
  const GPtrArray *devs=nm_client_get_all_devices(c); // nm_client_get_devices()
  gboolean uniq_wifidev_found=FALSE;
  for(guint i=0; i<(devs->len); ++i){
    NMDevice *const d=(devs->pdata)[i];
    g_assert_true(nm_device_is_real(d)); g_assert_true(!nm_device_get_firmware_missing(d)); g_assert_true(0==g_strcmp0(nm_device_get_iface(d), nm_device_get_description(d)));
    if(NM_DEVICE_TYPE_WIFI==nm_device_get_device_type(d)){
      g_assert_true(!uniq_wifidev_found); uniq_wifidev_found=TRUE;
      g_assert_true(nm_device_get_managed(d)); g_assert_true(0==g_strcmp0("wlan0", nm_device_get_iface(d)));
      r=nm_device_filter_connections(d, nm_client_get_connections(c)); g_assert_true(r && 1<=(r->len));
    }
  }
  return r;
}
