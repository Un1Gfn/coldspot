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


void gv_test(GVariant *v){
  g_print("%s\n", g_variant_get_type_string(v));
  g_print("%s\n", g_variant_print(v, /*type_annotate*/TRUE));
  g_print("\n");
}

GVariant *gv_new(){

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

  gv_test(vv);
  g_assert_true(0==g_strcmp0(PARSE, g_variant_print(vv, /*type_annotate*/TRUE)));

  return vv;

}

void gv_print(GVariant *v){

  // g_print("%s\n", );
  g_assert_true(g_variant_check_format_string(v, FMT, TRUE));
  g_assert_true(0==g_strcmp0(FMT, g_variant_get_type_string(v)));
  g_print("%s\n", FMT);
  g_print("\n");

  g_print("%s\n", g_variant_print(v, TRUE));
  g_print("\n");

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

int main(){

  {
    gv_new();
    return 0;
  }

  g_print("\n");
  
  NMClient *client=nm_client_new(NULL, NULL); g_assert_true(client);
  g_print("NetworkManager version: %s\n", nm_client_get_version(client));

  // Hotspot
  NMRemoteConnection *r_con=nm_client_get_connection_by_uuid(client, "22b985e1-e00e-4e0a-be92-8d9de9903968"); g_assert_true(r_con);
  NMConnection *con=NM_CONNECTION(r_con);

  NMSettingConnection *set_c=nm_connection_get_setting_connection(con); g_assert_true(set_c);

  GVariant *v=nm_connection_to_dbus(con, NM_CONNECTION_SERIALIZE_ALL);
  gv_print(v);

  gv_new();

  // g_assert_true(NM_VARIANT_TYPE_CONNECTION==g_variant_get_type(v));
  // GVariant *new_settings=g_variant_new(
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
  g_print("\n");
  return 0;
  
}
