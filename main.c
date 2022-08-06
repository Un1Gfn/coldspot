#include <elogind/sd-login.h>
#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <stdio.h>

// robust
static inline void printROB(const char *const s){
  if(!s)
    puts("()");
  else if(0==strlen(s))
    puts("''");
  else
    puts(s);
}

static inline void typedemo(gpointer o){
  // NM_CONNECTION(obj)
  // G_TYPE_CHECK_INSTANCE_CAST()
  // type_check_instance_cast()
  #define X(r1, r2, r3, r4, r5, r6, F, O) { \
    /*GObject*/            g_assert_true(r1==F(o, G_TYPE_OBJECT)); \
    /*NMObject*/           g_assert_true(r2==F(o, NM_TYPE_OBJECT)); \
    /*NMConnection*/       g_assert_true(r3==F(o, NM_TYPE_CONNECTION)); \
    /*NMRemoteConnection*/ g_assert_true(r4==F(o, NM_TYPE_REMOTE_CONNECTION)); \
    /*NMDevice*/           g_assert_true(r5==F(o, NM_TYPE_DEVICE)); \
    /*NMSetting*/          g_assert_true(r6==F(o, NM_TYPE_SETTING)); \
  }
  // https://docs.gtk.org/gobject/#function_macros
  g_assert_true(NM_TYPE_REMOTE_CONNECTION==G_OBJECT_TYPE(o));
  g_assert_true(!strcmp("NMRemoteConnection",G_OBJECT_TYPE_NAME(o)));
  g_assert_true(G_TYPE_CHECK_INSTANCE(o));
  X(1, 1, 1, 1, 0, 0, G_TYPE_CHECK_INSTANCE_TYPE, o);
  X(1, 0, 0, 0, 0, 0, G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE, o);
  // https://docs.gtk.org/gobject/#functions
  X(1, 1, 1, 1, 0, 0, g_type_check_instance_is_a, o);
  X(1, 0, 0, 0, 0, 0, g_type_check_instance_is_fundamentally_a, o);
  #undef X
}

typedef struct {const char *const cc; GType t; } S;
static inline void hier(gpointer instance, const S *s){
  g_assert_true(G_TYPE_CHECK_INSTANCE(instance));
  g_assert_true(g_type_check_instance(instance));
  for(; s->cc; ++s){
    // https://stackoverflow.com/a/1596970/
    // logical xor  ((!A)!=(!B))
    // boolean equ  (!A==!B)
    // boolean conv (!!A)
    g_assert_true(!('O'==s->cc[0]) == !G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE(instance, s->t));
    g_assert_true(!('O'==s->cc[0]) == !g_type_check_instance_is_fundamentally_a(instance, s->t));
    g_assert_true(!('O'==s->cc[1]) == !G_TYPE_CHECK_INSTANCE_TYPE(instance, s->t));
    g_assert_true(!('O'==s->cc[1]) == !g_type_check_instance_is_a(instance, s->t));
  }
  g_assert_true(G_TYPE_NONE==s->t);
}

static inline void coldspot(const GPtrArray *cons){

  for(guint i=0; i<(cons->len); ++i){

    NMRemoteConnection *const rc=(cons->pdata)[i];
    g_assert_true(!nm_remote_connection_get_unsaved(rc));
    g_assert_true(nm_remote_connection_get_visible(rc));
    printROB(nm_remote_connection_get_filename(rc));

    hier(rc,(const S[]){
      {"OO", G_TYPE_OBJECT},
      {" O", NM_TYPE_OBJECT},
      {" O", NM_TYPE_CONNECTION},
      {" O", NM_TYPE_REMOTE_CONNECTION},
      {"  ", NM_TYPE_DEVICE},
      {"  ", NM_TYPE_SETTING},
      {NULL, G_TYPE_NONE}
    });

    NMConnection *c=NM_CONNECTION(rc);
    NMSettingConnection *sc=nm_connection_get_setting_connection(c);
    g_assert_true(sc==(gpointer)nm_connection_get_setting_by_name(c, "connection"));

    hier(sc,(const S[]){
      {NULL, G_TYPE_NONE}
    });

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

  nm();

  return 0;

}
