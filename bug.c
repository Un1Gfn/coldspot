// gcc -g -Wall -Wextra -std=gnu11 $(pkg-config --cflags libnm) -o bug bug.c $(pkg-config --libs libnm)
// scp ./bug user@wt88047usb:/tmp/bug && ssh root@wt88047usb /tmp/bug "22b985e1-e00e-4e0a-be92-8d9de9903968"

#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <unistd.h>

// void func(
//   __attribute__((unused)) NMSetting *setting,
//   const char *key,
//   __attribute__((unused)) const GValue *value,
//   __attribute__((unused)) GParamFlags flags,
//   __attribute__((unused)) gpointer user_data
// ){
//   // g_print(".%s.\n", key);
//   if(!g_strcmp0("autoconnect", key)){
//     g_assert_true(!user_data);
//     g_print("0x%X\n", flags);
//     g_assert_true(G_VALUE_HOLDS_BOOLEAN(value));
//     g_value_set_boolean(value, TRUE);
//     g_print("%d\n", g_value_get_boolean(value));
//   }
// }

NMSetting *reconstruct_auto(NMSettingConnection *p){

  // g_assert_true(!nm_setting_option_get_boolean(p, "autoconnect", &(gboolean){0}));
  // g_assert_true(!nm_setting_option_get_boolean(p, "connection.autoconnect", &(gboolean){0}));
  // g_assert_true(!nm_setting_option_get_all_names(p, &(guint){0}));
  // g_assert_true(!nm_setting_option_get(p, "autoconnect"));
  // g_assert_true(!nm_setting_option_get(p, "connection.autoconnect"));
  // g_assert_true(!nm_setting_option_get_uint32(p, "autoconnect", &(guint32){0}));
  // g_assert_true(!nm_setting_option_get_uint32(p, "connection.autoconnect", &(guint32){0}));

  NMSetting *news=nm_setting_connection_new();
  g_assert_true(news);

  // if variant is a floating reference, it will be consumed
  // #define g_variant_new_string2(X) {const gchar* s=X; g_assert_true(s&&s[0]); g_variant_new_string(s);}
  g_print("%s\n", nm_setting_connection_get_id(p));
  nm_setting_option_set(news, "id",             g_variant_new_string(nm_setting_connection_get_id(p)));
  nm_setting_option_set(news, "uuid",           g_variant_new_string(nm_setting_connection_get_uuid(p)));
  nm_setting_option_set(news, "type",           g_variant_new_string(nm_setting_connection_get_connection_type(p)));
  nm_setting_option_set(news, "autoconnect",    g_variant_new_boolean(TRUE));
  nm_setting_option_set(news, "interface-name", g_variant_new_string(nm_setting_connection_get_interface_name(p)));
  nm_setting_option_set(news, "timestamp",      g_variant_new_uint64(nm_setting_connection_get_timestamp(p)));

  // nm_setting_enumerate_values(news, &func, NULL);
  g_print("%s\n", nm_setting_to_string(news));

  return news;

}

int main(){
  // g_assert(0==getuid()&&argc==2&&argv[1]);
  
  NMClient *client=nm_client_new(NULL, NULL); g_assert_true(client);
  g_print("NetworkManager version: %s\n", nm_client_get_version(client));

  // Hotspot
  NMRemoteConnection *r_con=nm_client_get_connection_by_uuid(client, "22b985e1-e00e-4e0a-be92-8d9de9903968"); g_assert_true(r_con);
  NMConnection *con=NM_CONNECTION(r_con);

  NMSettingConnection *set_c=nm_connection_get_setting_connection(NM_CONNECTION(con)); g_assert_true(set_c);

  NMSetting *news=reconstruct_auto(set_c);

  GError *e=NULL;
  if(!nm_setting_verify(news, con, &e)){
    g_critical("err: %s\n", e->message);
    exit(0);
  }
  nm_connection_add_setting(con, news);

  // alternative
  // nm_connection_replace_settings()

  // GError *e=NULL;
  if(!nm_remote_connection_commit_changes(r_con,TRUE,NULL,&e)){
    g_critical("err: %s\n", e->message);
    exit(0);
  }

  set_c=NULL;
  con=NULL;

  g_object_unref(client); client=NULL;

  g_print("bye\n");
  return 0;
  
}
