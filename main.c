#include <elogind/sd-login.h>
#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <stdio.h>

// #include "hier.h"
#include "nm2.h"

// robust
// static inline void printROB(const char *const s){
//   if(!s)
//     puts("()");
//   else if(0==strlen(s))
//     puts("''");
//   else
//     puts(s);
// }

// static gboolean predicate(const char *s){
//   return FALSE;
//   // return 0==strcmp(s, "autoconnect");
// }

// static inline void coldspot(const GPtrArray *cons){

//   for(guint i=0; i<(cons->len); ++i){

//     NMRemoteConnection *const rc=(cons->pdata)[i];
//     g_assert_true(!nm_remote_connection_get_unsaved(rc));
//     g_assert_true(nm_remote_connection_get_visible(rc));
//     printROB(nm_remote_connection_get_filename(rc));

//     relationship(rc,(const S[]){
//       {"OO", G_TYPE_OBJECT},
//       {" O", NM_TYPE_OBJECT},
//       {" O", NM_TYPE_CONNECTION},
//       {" O", NM_TYPE_REMOTE_CONNECTION},
//       {"  ", NM_TYPE_DEVICE},
//       {"  ", NM_TYPE_SETTING},
//       {"  ", NM_TYPE_SETTING_CONNECTION},
//       {NULL, G_TYPE_NONE}
//     });

//     NMConnection *c=NM_CONNECTION(rc);
//     NMSettingConnection *sc=nm_connection_get_setting_connection(c);
//     g_assert_true(sc==(gpointer)nm_connection_get_setting_by_name(c, "connection"));

//     relationship(sc,(const S[]){
//       {"OO", G_TYPE_OBJECT},
//       {"  ", NM_TYPE_OBJECT},
//       {"  ", NM_TYPE_CONNECTION},
//       {"  ", NM_TYPE_REMOTE_CONNECTION},
//       {"  ", NM_TYPE_DEVICE},
//       {" O", NM_TYPE_SETTING},
//       {" O", NM_TYPE_SETTING_CONNECTION},
//       {NULL, G_TYPE_NONE}
//     });

//     NMSetting *s=NM_SETTING(sc);
//     /**/puts(nm_setting_to_string(s));
//     g_assert_true(nm_setting_verify(s, c, NULL));

//     // /usr/include/libnm/nm-core-types.h: typedef gboolean (*NMUtilsPredicateStr)(const char *str);
//     // nm_setting_option_clear_by_name(s, &predicate);
//     // nm_setting_option_clear_by_name(s, NULL);
//     // nm_setting_option_set(s, "autoconnect", NULL);
//     // /**/puts(nm_setting_to_string(s));

//     NMSetting *s2=nm_setting_duplicate(s);
//     nm_setting_option_set(s2, "autoconnect", NULL);
//     nm_setting_option_set_boolean(s2, "autoconnect", TRUE);
//     /**/puts(nm_setting_to_string(s2));

//     exit(0);

//     nm_setting_option_set_boolean(s, "autoconnect", TRUE);
//     /**/puts(nm_setting_to_string(s));

//     g_assert_true(nm_setting_verify(s, c, NULL));

//     nm_connection_add_setting(c, s);
//     s=NULL;
//     sc=NULL;

//     GError *e=NULL;
//     if(!nm_remote_connection_commit_changes(rc,TRUE,NULL,&e)){
//       printf("err: %s\n", e->message);
//       exit(0);
//     }

//     // guint n=0;
//     // const char **ss=nm_setting_option_get_all_names(NM_SETTING(sc), &n);
//     // // g_assert_true(n>=1);
//     // puts(ss[0]);
//     // // for(guint i=0; i<n; ++i)
//     // //   puts(ss[i]);
//     // puts(nm_setting_connection_get_autoconnect(sc)?"auto":"noauto");
//     // nm_setting_option_set_boolean(NM_SETTING(sc), "autoconnect", FALSE);
//     // nm_connection_add_setting(rc, NM_SETTING(sc));

//   }
// }

int main(){

  g_print("\n");

  g_print("[build@%s]\n", __TIME__);
  g_print("\n");

  // g_assert_true(0==setenv("G_DEBUG", "fatal-warnings", 1));;
  NMClient *client=nm_client_new2();

  // const GPtrArray *devs=nm_client_get_all_devices(client); // nm_client_get_devices()
  // gboolean uniq_wifi_found=FALSE;
  // for(guint i=0; i<(devs->len); ++i){
  //   NMDevice *const d=(devs->pdata)[i];
  //   g_assert_true(nm_device_is_real(d));
  //   g_assert_true(!nm_device_get_firmware_missing(d));
  //   g_assert_true(!strcmp(nm_device_get_iface(d), nm_device_get_description(d)));
  //   // nm_device_set_autoconnect()
  //   if(NM_DEVICE_TYPE_WIFI==nm_device_get_device_type(d)){
  //     g_assert_true(!uniq_wifi_found); uniq_wifi_found=TRUE;
  //     g_assert_true(nm_device_get_managed(d));
  //     g_assert_true(!strcmp("wlan0", nm_device_get_iface(d)));
  //     // coldspot(nm_device_get_available_connections(d));
  //     GPtrArray *wificon=nm_device_filter_connections(d, nm_client_get_connections(client));
  //     coldspot(wificon);
  //     g_ptr_array_unref(wificon); wificon=NULL;
  //   }
  // }

  g_object_unref(client); client=NULL; return 0;

}
