// gcc -g -Wall -Wextra -std=gnu11 $(pkg-config --cflags libnm) -o bug bug.c $(pkg-config --libs libnm)
// scp ./bug user@wt88047usb:/tmp/bug && ssh root@wt88047usb /tmp/bug "22b985e1-e00e-4e0a-be92-8d9de9903968"

#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

#define FMT "a{sa{sv}}"
#define PARSE "{'connection': {'autoconnect': <true>}}"

void gv_print(GVariant *v){
  g_print("%s\n", g_variant_get_type_string(v));
  g_print("\n");
  g_print("%s\n", g_variant_print(v, /*type_annotate*/TRUE));
  g_print("\n");
}

void gv_check_print(GVariant *v){

  // check
  g_assert_true(g_variant_check_format_string(v, FMT, TRUE));
  g_assert_true(0==g_strcmp0(FMT, g_variant_get_type_string(v)));

  // print
  // g_print("%s\n", FMT);
  // g_print("\n");
  // g_print("%s\n", g_variant_print(v, TRUE));
  // g_print("\n");
  gv_print(v);

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
  g_print("\n");

}

GVariant *gv_new_autoconnect_true(){

  // [ { s => [ s => v ] } ]
  // [{s => [s => v]}]
  // [{s=>[s=>v]}]
  // "Strings"
  // "Variants"
  // "Arrays"
  // "Dictionaries"

  // g_variant_new_parsed() // https://docs.gtk.org/glib/gvariant-format-strings.html#examples_8

  GVariantBuilder *b1=NULL;
  GVariantBuilder *b0=NULL;
  GVariant *vv=NULL;

  // b1 def    "a{sv}"
  // b1 add     "{sv}"  <- "autoconnect" + gvariantboolean(TRUE)
  // b0 def "a{sa{sv}}"
  // b0 add  "{sa{sv}}" <- "connection" + b1
  // vv def "a{sa{sv}}" <- b0
  b1=g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));
  g_variant_builder_add(b1, "{sv}", "autoconnect", g_variant_new_boolean(TRUE));
  b0=g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
  g_variant_builder_add(b0, "{sa{sv}}", "connection", b1);
  vv=g_variant_new("a{sa{sv}}", b0); b0=NULL;

  gv_check_print(vv);
  g_assert_true(0==g_strcmp0(PARSE, g_variant_print(vv, /*type_annotate*/TRUE)));

  return vv;

}

int main(){

  GError *e=NULL;

  g_print("\n");
  
  NMClient *client=nm_client_new(NULL, NULL); g_assert_true(client);
  g_print("NetworkManager version: %s\n", nm_client_get_version(client));
  g_print("\n");

  // get connection "Hotspot"
  NMRemoteConnection *r_con=nm_client_get_connection_by_uuid(client, "22b985e1-e00e-4e0a-be92-8d9de9903968"); g_assert_true(r_con);
  NMConnection *con=NM_CONNECTION(r_con);
  NMSettingConnection *set_c=nm_connection_get_setting_connection(con); g_assert_true(set_c);

  // generate new settings with autoconnect replaced
  GVariant *old_v=nm_connection_to_dbus(con, NM_CONNECTION_SERIALIZE_ALL);
  gv_check_print(old_v);

  // replace settings
  g_assert_true(g_variant_type_equal(NM_VARIANT_TYPE_CONNECTION, g_variant_get_type(old_v))); // correct
  e=NULL; if(!nm_connection_replace_settings(con, old_v, &e)){
    g_critical("nm_connection_replace_settings() error: %s\n", e->message);
    exit(0);
  }

  

  // save to disk
  e=NULL; if(!nm_remote_connection_commit_changes(r_con, TRUE, NULL, &e)){
    g_critical("nm_remote_connection_commit_changes() error: %s\n", e->message);
    exit(0);
  }

  set_c=NULL;
  con=NULL;

  g_object_unref(client); client=NULL;

  g_print("bye\n");
  g_print("\n");
  return 0;
  
}

// GVariantDict *new_d=new_v;
// {
//   GVariant *data;
//   gint value = 1;
//   gint max = 3;
//   data = g_variant_new_parsed ("(%o, {'brightness': {'value': <%i>, 'max': <%i>}})",
//                                "/object/path", value, max);
//   gv_print(data);
//   {
//     GVariant *params;
//     GVariant *p_brightness;
//     gchar *obj;
//     gint p_max;
//     g_variant_get (data, "(o@a{?*})", &obj, &params);
//     gv_print(params);
//     g_print ("object_path: %s\n", obj);
//     p_brightness = g_variant_lookup_value (params, "brightness", G_VARIANT_TYPE_VARDICT);
//     gv_print(p_brightness);
//     {
//       GVariant *ref=NULL;
//       g_variant_lookup (p_brightness, "max", "@i", &ref);
//       g_assert_true(g_variant_is_floating(g_variant_new_int32(77)));
//       g_assert_true(!g_variant_is_floating(ref));
//       g_print ("max: %d\n", g_variant_get_int32(ref));
//     }
//     g_variant_lookup (p_brightness, "max", "i", &p_max);
//     g_print ("max: %d\n", p_max);
//     exit(0);
//   }
// }

// {
//   GVariant *ref=NULL;
//   gpointer *buf=g_malloc0(1024*1024);
//   g_assert_true(g_variant_lookup(new_v, "connection", "{sa{sv}}", buf, buf, buf, buf, buf, buf));
//   g_assert_true(ref);
//   gv_check_print(ref);
//   exit(0);
// }

// {
//   g_print("%s\n", g_variant_type_dup_string(NM_VARIANT_TYPE_SETTING));
//   g_print("%s\n", g_variant_type_dup_string(NM_VARIANT_TYPE_CONNECTION));
// }
// g_assert_true(NM_VARIANT_TYPE_CONNECTION==g_variant_get_type(new_settings)); // pitfall!
