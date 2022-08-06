// gcc -g -Wall -Wextra -std=gnu11 $(pkg-config --cflags libnm) -o bug bug.c $(pkg-config --libs libnm)
// scp ./bug user@wt88047usb:/tmp/bug && ssh root@wt88047usb /tmp/bug "22b985e1-e00e-4e0a-be92-8d9de9903968"

#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <unistd.h>

void func(
  __attribute__((unused)) NMSetting *setting,
  const char *key,
  __attribute__((unused)) const GValue *value,
  __attribute__((unused)) GParamFlags flags,
  __attribute__((unused)) gpointer user_data
){
  // g_print(".%s.\n", key);
  if(!g_strcmp0("autoconnect", key)){
    g_assert_true(!user_data);
    g_print("0x%X\n", flags);
    g_assert_true(G_VALUE_HOLDS_BOOLEAN(value));
    g_value_set_boolean(value, TRUE);
    g_print("%d\n", g_value_get_boolean(value));
  }
}

void force_auto(NMSetting *p){

  g_assert_true(!nm_setting_option_get_boolean(p, "autoconnect", &(gboolean){0}));
  g_assert_true(!nm_setting_option_get_boolean(p, "connection.autoconnect", &(gboolean){0}));
  g_assert_true(!nm_setting_option_get_all_names(p, &(guint){0}));
  g_assert_true(!nm_setting_option_get(p, "autoconnect"));
  g_assert_true(!nm_setting_option_get(p, "connection.autoconnect"));
  g_assert_true(!nm_setting_option_get_uint32(p, "autoconnect", &(guint32){0}));
  g_assert_true(!nm_setting_option_get_uint32(p, "connection.autoconnect", &(guint32){0}));

  g_assert_true(!nm_setting_get_dbus_property_type(p, "lskdjfj92kefms"));
  g_assert_true(!nm_setting_get_dbus_property_type(p, "connection.autoconnect"));
  const GVariantType *vt=nm_setting_get_dbus_property_type(p, "autoconnect"); g_assert_true(vt);
  gchar *vts=g_variant_type_dup_string(vt);
  g_print("marshall '%s'\n", vts); g_free(vts); vts=NULL;

  // g_print("%s\n", nm_setting_to_string(p));
  nm_setting_enumerate_values(p, &func, NULL);
  // g_print("%s\n", nm_setting_to_string(p));
  nm_setting_enumerate_values(p, &func, NULL);

}

int main(){
  // g_assert(0==getuid()&&argc==2&&argv[1]);
  
  NMClient *client=nm_client_new(NULL, NULL); g_assert_true(client);
  g_print("NetworkManager version: %s\n", nm_client_get_version(client));

  // Hotspot
  NMRemoteConnection *con=nm_client_get_connection_by_uuid(client, "22b985e1-e00e-4e0a-be92-8d9de9903968"); g_assert_true(con);
  NMSettingConnection *set=nm_connection_get_setting_connection(NM_CONNECTION(con)); g_assert_true(set);

  force_auto(NM_SETTING(set));
  // nm_setting_option_set(NM_SETTING(set), "autoconnect", NULL);
  // force_auto(set);

  GError *e=NULL;
  if(!nm_remote_connection_commit_changes(con,TRUE,NULL,&e)){
    g_critical("err: %s\n", e->message);
    exit(0);
  }

  set=NULL;
  con=NULL;

  g_object_unref(client); client=NULL;

  g_print("bye\n");
  return 0;
  
}
