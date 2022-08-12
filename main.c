#include <elogind/sd-login.h>
#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <stdio.h>
#include <unistd.h> // sleep()

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

static inline void buildtime(){
  const char *const utc=__TIME__;
  g_print("[build @ %s UTC]\n", utc);
  // {
  //   struct tm tm={0};
  //   g_assert_true(utc+2+1+2+1+2==strptime(utc, "%T", &tm));
  //   //     strptime()    mktime        gmtime    strftime
  //   // str            tm        time_t        tm          time_t
  //   char local[9]={0};
  //   g_assert_true(8==strftime(local, 9, "%T", gmtime(&(time_t){mktime(&tm)})));
  //   g_print("%s\n", local);
  //   g_print("\n");
  //   exit(0);
  // }
}

int main(const int argc, const char *const argv[]){

  g_print("\n");

  g_assert_true(2==argc);
  g_assert_true(argv[1]&&argv[1][0]);

  // g_assert_true root or active session
  // ...

  buildtime();
  g_print("\n");

  NMClient *client=nm2_init();

  nm2_wireless_disable(&client);

  GPtrArray *wificons=nm2_getwificons(client);
  for(guint i=0; i<(wificons->len); ++i)
    nm2_autoconnect_toggle(client, wificons->pdata[i], (0==g_strcmp0("ap", argv[1])));
  g_ptr_array_unref(wificons); wificons=NULL;

  g_assert_true(!nm_client_wireless_get_enabled(client));
  nm_client_wireless_set_enabled(client, TRUE); // async! // g_assert_true(nm_client_wireless_get_enabled(client));

  g_object_unref(client); client=NULL; return 0;

}
