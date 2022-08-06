#include <assert.h>
#include <elogind/sd-login.h>
#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <stdio.h>

static inline gboolean active_session(){

  char *session=NULL;
  if(-ENODATA==sd_pid_get_session(getpid(), &session)){
    assert(!session);
    puts("no session");
    return FALSE;
  }

  printf("session '%s'\n", session);
  assert(session);
  assert(2==strlen(session));
  free(session); session=NULL;
  return TRUE;

}

static inline void nmcgpr(NMClient *const c){

  // #define NMCGPR(RESULT,PERMISSION) assert(RESULT==nm_client_get_permission_result(client, PERMISSION))
  #define NMCGPR(RESULT,PERMISSION) printf("%d\n", nm_client_get_permission_result(c, PERMISSION))
  if(active_session()){
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_CHECKPOINT_ROLLBACK);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_ENABLE_DISABLE_CONNECTIVITY_CHECK);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_ENABLE_DISABLE_NETWORK);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_ENABLE_DISABLE_STATISTICS);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_ENABLE_DISABLE_WIFI);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_ENABLE_DISABLE_WIMAX);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_ENABLE_DISABLE_WWAN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_NETWORK_CONTROL);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_RELOAD);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_SETTINGS_MODIFY_GLOBAL_DNS);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_SETTINGS_MODIFY_HOSTNAME);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_SETTINGS_MODIFY_OWN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_SETTINGS_MODIFY_SYSTEM);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_SLEEP_WAKE);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_WIFI_SCAN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_WIFI_SHARE_OPEN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_YES,  NM_CLIENT_PERMISSION_WIFI_SHARE_PROTECTED);
  }else{
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_CHECKPOINT_ROLLBACK);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_ENABLE_DISABLE_CONNECTIVITY_CHECK);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_ENABLE_DISABLE_NETWORK);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_ENABLE_DISABLE_STATISTICS);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_ENABLE_DISABLE_WIFI);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_ENABLE_DISABLE_WIMAX);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_ENABLE_DISABLE_WWAN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_NETWORK_CONTROL);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_RELOAD);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_SETTINGS_MODIFY_GLOBAL_DNS);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_SETTINGS_MODIFY_HOSTNAME);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_SETTINGS_MODIFY_OWN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_SETTINGS_MODIFY_SYSTEM);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_SLEEP_WAKE);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_AUTH, NM_CLIENT_PERMISSION_WIFI_SCAN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_WIFI_SHARE_OPEN);
    NMCGPR(NM_CLIENT_PERMISSION_RESULT_NO,   NM_CLIENT_PERMISSION_WIFI_SHARE_PROTECTED);
  }
  #undef NMCGPR

}

// robust
static inline void printROB(const char *const s){
  if(!s)
    puts("()");
  else if(0==strlen(s))
    puts("''");
  else
    puts(s);
}

static inline void rcs(const GPtrArray *cons){
  for(guint i=0; i<(cons->len); ++i){

    NMRemoteConnection *const rc=(cons->pdata)[i];

    assert(NM_TYPE_REMOTE_CONNECTION==G_OBJECT_TYPE(rc));

    // NMConnection
    // GObject
    // NMDevice
    // NMRemoteConnection

    // https://docs.gtk.org/gobject/#function_macros
    assert(NM_TYPE_REMOTE_CONNECTION==G_OBJECT_TYPE(rc));
    assert(!strcmp("NMRemoteConnection",G_OBJECT_TYPE_NAME(rc)));

    assert(G_TYPE_CHECK_INSTANCE(rc));
    assert(!G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE(rc, NM_TYPE_OBJECT));
    assert(!G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE(rc, NM_TYPE_REMOTE_CONNECTION));
    assert(!G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE(rc, NM_TYPE_CONNECTION));
    assert(!G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE(rc, NM_TYPE_DEVICE));
    assert(!G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE(rc, NM_TYPE_SETTING));

    puts(g_type_name(G_TYPE_FUNDAMENTAL(NM_TYPE_REMOTE_CONNECTION)));

    // TYPE_CHECK_INSTANCE_CAST

    assert(!nm_remote_connection_get_unsaved(rc));
    printROB(nm_remote_connection_get_filename(rc));
    assert(nm_remote_connection_get_visible(rc));

    // assert(!NM_IS_DEVICE(rc));
    // assert(!NM_IS_SETTING(rc));

    // NMSettingConnection *sc=nm_connection_get_setting_connection(NM_CONNECTION(rc));
    // guint n=0;
    // const char **ss=nm_setting_option_get_all_names(NM_SETTING(sc), &n);
    // // assert(n>=1);
    // puts(ss[0]);
    // // for(guint i=0; i<n; ++i)
    // //   puts(ss[i]);
    // puts(nm_setting_connection_get_autoconnect(sc)?"auto":"noauto");
    // nm_setting_option_set_boolean(NM_SETTING(sc), "autoconnect", FALSE);
    // nm_connection_add_setting(rc, NM_SETTING(sc));

  }
}

void nm(){

  NMClient *client=nm_client_new(NULL, NULL);
  assert(client);

  g_print("NetworkManager version: %s\n", nm_client_get_version(client));

  // /usr/include/libnm/nm-dbus-interface.h
  // NM_STATE_*
  printf("enum_state=%d\n", nm_client_get_state(client));

  assert(!nm_client_get_startup(client));

  assert(nm_client_get_nm_running(client));

  // /usr/include/libnm/nm-dbus-interface.h - NM_METERED_*
  printf("enum_metered=%d\n", nm_client_get_metered(client));

  assert(nm_client_networking_get_enabled(client));
  printf("wlan=%d\n", nm_client_wireless_get_enabled(client));
  printf("wlan_hw=%d\n", nm_client_wireless_hardware_get_enabled(client));
  printf("wwan=%d\n", nm_client_wwan_get_enabled(client));
  printf("wwan_hw=%d\n", nm_client_wwan_hardware_get_enabled(client));

  assert((NM_RADIO_FLAG_WLAN_AVAILABLE|NM_RADIO_FLAG_WWAN_AVAILABLE)==nm_client_get_radio_flags(client));

  printf("con_chk_avail=%d\n", nm_client_connectivity_check_get_available(client));
  printf("con_chk=%d\n", nm_client_connectivity_check_get_enabled(client));

  // nmcgpr(c)

  // const GPtrArray *devs=nm_client_get_devices(client);
  const GPtrArray *devs=nm_client_get_all_devices(client);
  puts("");
  gboolean uniq_wifi_found=FALSE;
  for(guint i=0; i<(devs->len); ++i){
    NMDevice *const d=(devs->pdata)[i];
    assert(nm_device_is_real(d));
    printROB(nm_device_get_iface(d));
    printROB(nm_device_get_ip_iface(d));
    printROB(nm_device_get_udi(d));
    printROB(nm_device_get_path(d));
    printROB(nm_device_get_driver_version(d));
    printROB(nm_device_get_firmware_version(d));
    printROB(nm_device_get_type_description(d));
    printROB(nm_device_get_hw_address(d));
    puts(nm_device_get_autoconnect(d)?"auto":"noauto");
    puts(nm_device_get_firmware_missing(d)?"missfw":"nomissfw");
    printROB(nm_device_get_product(d));
    printROB(nm_device_get_vendor(d));
    printROB(nm_device_get_description(d));
    puts(nm_device_get_managed(d)?"managed":"stray");
    // nm_device_set_autoconnect()
    if(NM_DEVICE_TYPE_WIFI==nm_device_get_device_type(d)){
      assert(!uniq_wifi_found); uniq_wifi_found=TRUE;
      // rcs(nm_device_get_available_connections(d));
      GPtrArray *wificon=nm_device_filter_connections(d, nm_client_get_connections(client));
      rcs(wificon);
      g_ptr_array_unref(wificon); wificon=NULL;
    }
    puts("");
  }


  // void cb(GObject* source_object, GAsyncResult* res, __attribute__((unused)) gpointer user_data){
  //   puts("cb()");
  // }
  // GError *e=NULL;
  // nm_client_reload_connections_async(client, NULL, &cb, NULL);
  // while(!nm_client_reload_connections_finish(client, NULL, &e)){
  //   printf(".s923$sk");fflush(stdout);
  // }

  // nm_device_filter_connections()

  g_object_unref(client); client=NULL;

}

int main(){

  nm();

  return 0;

}
