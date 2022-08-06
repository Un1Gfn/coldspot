#include <elogind/sd-login.h>
#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <stdio.h>

#include "hier.h"

// robust
static inline void printROB(const char *const s){
  if(!s)
    puts("()");
  else if(0==strlen(s))
    puts("''");
  else
    puts(s);
}

static inline void coldspot(const GPtrArray *cons){

  for(guint i=0; i<(cons->len); ++i){

    NMRemoteConnection *const rc=(cons->pdata)[i];
    g_assert_true(!nm_remote_connection_get_unsaved(rc));
    g_assert_true(nm_remote_connection_get_visible(rc));
    printROB(nm_remote_connection_get_filename(rc));

    relationship(rc,(const S[]){
      {"OO", G_TYPE_OBJECT},
      {" O", NM_TYPE_OBJECT},
      {" O", NM_TYPE_CONNECTION},
      {" O", NM_TYPE_REMOTE_CONNECTION},
      {"  ", NM_TYPE_DEVICE},
      {"  ", NM_TYPE_SETTING},
      {"  ", NM_TYPE_SETTING_CONNECTION},
      {NULL, G_TYPE_NONE}
    });

    NMConnection *c=NM_CONNECTION(rc);
    NMSettingConnection *sc=nm_connection_get_setting_connection(c);
    g_assert_true(sc==(gpointer)nm_connection_get_setting_by_name(c, "connection"));

    relationship(sc,(const S[]){
      {"OO", G_TYPE_OBJECT},
      {"  ", NM_TYPE_OBJECT},
      {"  ", NM_TYPE_CONNECTION},
      {"  ", NM_TYPE_REMOTE_CONNECTION},
      {"  ", NM_TYPE_DEVICE},
      {" O", NM_TYPE_SETTING},
      {" O", NM_TYPE_SETTING_CONNECTION},
      {NULL, G_TYPE_NONE}
    });

    NMSetting *s=NM_SETTING(sc);
    puts(nm_setting_to_string(s));
    g_assert_true(nm_setting_verify(s, c, NULL));

    nm_setting_option_set_boolean(s, "autoconnect", TRUE);
    puts(nm_setting_to_string(s));
    g_assert_true(nm_setting_verify(s, c, NULL));

    nm_connection_add_setting(c, s);
    s=sc=NULL;

    GError *e=NULL;
    if(!nm_remote_connection_commit_changes(rc,TRUE,NULL,&e)){
      printf("err: %s\n", e->message);
      exit(0);
    }

    // guint n=0;
    // const char **ss=nm_setting_option_get_all_names(NM_SETTING(sc), &n);
    // // g_assert_true(n>=1);
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
  g_assert_true(client);
  g_print("NetworkManager version: %s\n", nm_client_get_version(client));
  g_assert_true(!nm_client_get_startup(client));
  g_assert_true(nm_client_get_nm_running(client));
  g_assert_true(nm_client_networking_get_enabled(client));
  g_assert_true((NM_RADIO_FLAG_WLAN_AVAILABLE|NM_RADIO_FLAG_WWAN_AVAILABLE)==nm_client_get_radio_flags(client));

  // /usr/include/libnm/nm-dbus-interface.h - NM_METERED_*

  const GPtrArray *devs=nm_client_get_all_devices(client); // nm_client_get_devices()
  gboolean uniq_wifi_found=FALSE;
  for(guint i=0; i<(devs->len); ++i){
    NMDevice *const d=(devs->pdata)[i];
    g_assert_true(nm_device_is_real(d));
    g_assert_true(!nm_device_get_firmware_missing(d));
    g_assert_true(!strcmp(nm_device_get_iface(d), nm_device_get_description(d)));
    // nm_device_set_autoconnect()
    if(NM_DEVICE_TYPE_WIFI==nm_device_get_device_type(d)){
      g_assert_true(!uniq_wifi_found); uniq_wifi_found=TRUE;
      g_assert_true(nm_device_get_managed(d));
      g_assert_true(!strcmp("wlan0", nm_device_get_iface(d)));
      // coldspot(nm_device_get_available_connections(d));
      GPtrArray *wificon=nm_device_filter_connections(d, nm_client_get_connections(client));
      coldspot(wificon);
      g_ptr_array_unref(wificon); wificon=NULL;
    }
  }

  g_object_unref(client); client=NULL;

}

int main(){

  // register before looking up
  // NMSettingConnection *garbage=nm_setting_connection_new();
  // puts(g_type_name(g_type_parent(g_type_from_name("NMSettingConnection"))));
  // g_object_unref(garbage);

  // type_info(G_TYPE_OBJECT);
  // type_info(NM_TYPE_OBJECT);
  // type_info(NM_TYPE_DEVICE);
  // type_info(NM_TYPE_REMOTE_CONNECTION);
  // type_info(NM_TYPE_SETTING);
  // type_info(NM_TYPE_SETTING_CONNECTION);
  // type_info(NM_TYPE_CONNECTION);

  nm();

  return 0;

}
