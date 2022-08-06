// gcc -g -Wall -Wextra -std=gnu11 $(pkg-config --cflags libnm) -o bug bug.c $(pkg-config --libs libnm)
// scp ./bug user@wt88047usb:/tmp/bug && ssh root@wt88047usb /tmp/bug "22b985e1-e00e-4e0a-be92-8d9de9903968"

#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

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

int main(){
  // g_assert(0==getuid()&&argc==2&&argv[1]);
  
  NMClient *client=nm_client_new(NULL, NULL); g_assert_true(client);
  g_print("NetworkManager version: %s\n", nm_client_get_version(client));

  // Hotspot
  NMRemoteConnection *r_con=nm_client_get_connection_by_uuid(client, "22b985e1-e00e-4e0a-be92-8d9de9903968"); g_assert_true(r_con);
  NMConnection *con=NM_CONNECTION(r_con);

  NMSettingConnection *set_c=nm_connection_get_setting_connection(con); g_assert_true(set_c);

  GVariant *v=nm_connection_to_dbus(con, NM_CONNECTION_SERIALIZE_ALL);
  gsize sz=g_variant_get_size(v);
  g_print("%lu\n", sz);
  gchar data[sz+1]; bzero(data, sz+1);
  g_variant_store(v, data);
  g_assert_true(!isprint('\b'));
  g_assert_true(!isprint('\v'));
  g_assert_true(!isprint('\t'));
  for(gsize i=0; i<sz; ++i){
    gchar ch=data[i];
    // g_print("[%lu] ", i);
    putchar(isprint(ch)?ch:'.');
  }
  g_print("\n");

  // g_assert_true(NM_VARIANT_TYPE_CONNECTION==g_variant_get_type(v));
  // GVariant *new_settings=g_variant_new(
    // NM_VARIANT_TYPE_SETTING === G_VARIANT_TYPE_VARDICT
    // map { string => arr [ map { string => variant }

    // NM_VARIANT_TYPE_CONNECTION
    // arr [ map { string => arr [ map { string => variant } ] ]
    // "a{sa{sv}}",
  // );
  // GError *e=NULL;
  // if(!nm_connection_replace_settings(con, new_settings, &e)){
  //   g_critical("err: %s\n", e->message);
  //   exit(0);
  // }

  // alternative
  // nm_connection_replace_settings()

  // GError *e=NULL;
  // if(!nm_remote_connection_commit_changes(r_con,TRUE,NULL,&e)){
  //   g_critical("err: %s\n", e->message);
  //   exit(0);
  // }

  set_c=NULL;
  con=NULL;

  g_object_unref(client); client=NULL;

  g_print("bye\n");
  return 0;
  
}
