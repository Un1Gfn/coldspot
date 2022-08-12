#include <NetworkManager.h>

#include "nm2.h"

static inline NMClient *nm2_init(){
  NMClient *c=nm_client_new(/*GCancellable*/NULL, /*GError*/NULL); g_assert_true(c);
  _Static_assert(NM_CHECK_VERSION(1,38,2), ""); g_assert_true(0==g_strcmp0("1.38.2", nm_client_get_version(c)));
  g_assert_true(!nm_client_get_startup(c));
  g_assert_true(nm_client_get_nm_running(c));
  g_assert_true((NM_RADIO_FLAG_WLAN_AVAILABLE|NM_RADIO_FLAG_WWAN_AVAILABLE)==nm_client_get_radio_flags(c));
  g_assert_true(nm_client_networking_get_enabled(c));
  return c;
}

// Returns: (transfer full) (element-type NMConnection): array of all wifi connections
static inline GPtrArray *nm2_getwificons(NMClient *c){
  GPtrArray *r=NULL;
  const GPtrArray *devs=nm_client_get_all_devices(c); // nm_client_get_devices()
  gboolean uniq_wifidev_found=FALSE;
  for(guint i=0; i<(devs->len); ++i){
    NMDevice *const d=(devs->pdata)[i];
    g_assert_true(nm_device_is_real(d)); g_assert_true(!nm_device_get_firmware_missing(d)); g_assert_true(0==g_strcmp0(nm_device_get_iface(d), nm_device_get_description(d)));
    if(NM_DEVICE_TYPE_WIFI==nm_device_get_device_type(d)){
      g_assert_true(!uniq_wifidev_found); uniq_wifidev_found=TRUE;
      g_assert_true(nm_device_get_managed(d)); g_assert_true(0==g_strcmp0("wlan0", nm_device_get_iface(d)));
      r=nm_device_filter_connections(d, nm_client_get_connections(c)); g_assert_true(r && 1<=(r->len));
    }
  }
  return r;
}

static inline GVariant *copy_change(GVariant *old, const gboolean bool_autoconnect){

  GVariantIter *section_it=NULL;
  gchar        *section_name=NULL;
  GVariant     *section=NULL;
  GVariantIter *option_it=NULL;
  gchar        *option_key=NULL;
  GVariant     *option_value=NULL;

  GVariantBuilder *b_opt2sec=NULL;
  GVariantBuilder *b_sec2set=NULL;
  GVariant *new_setting=NULL;

  g_assert_true(g_variant_type_equal(NM_VARIANT_TYPE_CONNECTION, g_variant_get_type(old)));

  b_opt2sec=g_variant_builder_new(NM_VARIANT_TYPE_SETTING);
  b_sec2set=g_variant_builder_new(NM_VARIANT_TYPE_CONNECTION);

  section_it=g_variant_iter_new(old); g_assert_true(section_it); while(g_variant_iter_loop(section_it, "{s@a{sv}}", &section_name, &section)){

    g_assert_true(section_name);
    g_assert_true(section);

    if(0==g_strcmp0("connection", section_name)){

      // section "connection"

      gboolean opt_autoconnect_found=FALSE;
      option_it=g_variant_iter_new(section); g_assert_true(option_it); while(g_variant_iter_loop(option_it, "{sv}", &option_key, &option_value)){
        g_assert_true(option_key);
        g_assert_true(option_value && !g_variant_is_floating(option_value));
        if(0==g_strcmp0("autoconnect", option_key)){
          g_assert_true(!opt_autoconnect_found); opt_autoconnect_found=TRUE;
          // connection.autoconnect (replace)
          option_value=g_variant_ref_sink(g_variant_new_boolean(bool_autoconnect));
        }else{
          g_variant_ref(option_value); // g_variant_ref_sink(option_value);
        }
        // add option to section
        g_variant_builder_add(b_opt2sec, "{sv}", option_key, option_value);
      } // end loop over options in old section "connection"
      if(!opt_autoconnect_found){
        // connection.autoconnect (add)
        g_variant_builder_add(b_opt2sec, "{sv}", "autoconnect", g_variant_ref_sink(g_variant_new_boolean(bool_autoconnect)));
      }

      g_variant_iter_free(option_it); option_it=NULL;

      // colloect options and export to section "connection" (modified)
      // add section "connection" (modified) to b_sec2set
      g_variant_builder_add(b_sec2set, "{sa{sv}}", "connection", b_opt2sec); b_opt2sec=NULL;

    }else{

      g_variant_ref(section);
      // add section other than "connection" (as-is) to b_sec2set
      g_variant_builder_add(b_sec2set, "{s@a{sv}}", section_name, section);

    }

  } // end loop over sections in old setting

  g_variant_iter_free(section_it); section_it=NULL;
  new_setting=g_variant_new("a{sa{sv}}", b_sec2set); b_sec2set=NULL;

  return new_setting;

}

static inline void nm2_autoconnect_toggle(__attribute__((unused)) NMClient *c, NMConnection *con, const gboolean auto_ap_manual_else){

    g_assert_true(nm_connection_is_type(con, NM_SETTING_WIRELESS_SETTING_NAME));
    g_assert_true(nm_connection_verify(con, NULL));
    g_assert_true(0==g_strcmp0("wlan0", nm_connection_get_interface_name(con)));
    g_assert_true(0==g_strcmp0("802-11-wireless", nm_connection_get_connection_type(con)));

    // non-remote
    // {
    // }

    // // remote
    {
      NMRemoteConnection *rcon=nm_client_get_connection_by_uuid(c, nm_connection_get_uuid(con)); g_assert_true(rcon);
      g_assert_true(nm_remote_connection_get_visible(rcon));
      g_assert_true(!nm_remote_connection_get_unsaved(rcon));
      con=NM_CONNECTION(rcon);
      GVariant *cur_v=nm_connection_to_dbus(con, NM_CONNECTION_SERIALIZE_ALL);
      GVariant *new_v=copy_change(cur_v, !auto_ap_manual_else == !(0==g_strcmp0("Hotspot", nm_connection_get_id(con))));
      GError *e=NULL;

      // replace new settings
      e=NULL; if(!nm_connection_replace_settings(con, new_v, &e)){
        g_critical("nm_connection_replace_settings() error: %s\n", e->message);
        exit(0);
      }

      // save to disk
      e=NULL; if(!nm_remote_connection_commit_changes(rcon, TRUE, NULL, &e)){
        g_critical("nm_remote_connection_commit_changes() error: %s\n", e->message);
        exit(0);
      }

    }

}

static inline void nm2_wireless_disable(NMClient **const cc){
  g_assert_true(nm_client_wireless_hardware_get_enabled(*cc));
  while(nm_client_wireless_get_enabled(*cc)){
    g_print("disabling...\n");
    // g_print(":; nmcli radio wifi off\n"); g_print("\n");
    nm_client_wireless_set_enabled(*cc, FALSE);
    sleep(1);
    g_object_unref(*cc); *cc=NULL; *cc=nm2_init();
  }
}

static inline void nm2_wireless_enable(NMClient *const *const cc){
  g_assert_true(nm_client_wireless_hardware_get_enabled(*cc));
  g_assert_true(!nm_client_wireless_get_enabled(*cc));
  g_print("enabling (async)...\n");
  nm_client_wireless_set_enabled(*cc, TRUE);
  // g_assert_true(nm_client_wireless_get_enabled(client));
}

void nm2_demo1_gvtrel(){

  inline void ts_t_equ(const gchar *const ts, const GVariantType *const t){
    g_assert_true(g_variant_type_equal(G_VARIANT_TYPE(ts), t));
    gchar *ts2=g_variant_type_dup_string(t); g_assert_true(0==g_strcmp0(ts, ts2)); g_free(ts2); ts2=NULL;
  }

  g_assert_true(g_variant_type_string_is_valid("a{sa{sv}}"));
  ts_t_equ("a{sa{sv}}", NM_VARIANT_TYPE_CONNECTION);

  g_assert_true(!g_variant_type_is_definite(G_VARIANT_TYPE("a{?*}")));
  g_assert_true(g_variant_type_is_subtype_of(NM_VARIANT_TYPE_CONNECTION, G_VARIANT_TYPE("a{?*}")));
  g_assert_true(g_variant_type_is_subtype_of(NM_VARIANT_TYPE_CONNECTION, NM_VARIANT_TYPE_CONNECTION));

  g_assert_true(g_variant_type_is_array(NM_VARIANT_TYPE_CONNECTION));
  g_assert_true(!g_variant_type_is_basic(NM_VARIANT_TYPE_CONNECTION));
  g_assert_true(g_variant_type_is_container(NM_VARIANT_TYPE_CONNECTION));
  g_assert_true(g_variant_type_is_definite(NM_VARIANT_TYPE_CONNECTION));
  g_assert_true(!g_variant_type_is_maybe(NM_VARIANT_TYPE_CONNECTION));
  g_assert_true(!g_variant_type_is_tuple(NM_VARIANT_TYPE_CONNECTION));
  g_assert_true(!g_variant_type_is_variant(NM_VARIANT_TYPE_CONNECTION));

  const GVariantType *const e=g_variant_type_element(NM_VARIANT_TYPE_CONNECTION); g_assert_true(e);
  ts_t_equ("{sa{sv}}", e);

  g_assert_true(g_variant_type_is_dict_entry(e));
  g_assert_true(g_variant_type_equal(NM_VARIANT_TYPE_SETTING, g_variant_type_value(e)));

  const GVariantType *const i=g_variant_type_first(e); g_assert_true(i);
  ts_t_equ("s", i); g_assert_true(g_variant_type_is_basic(i));
  g_assert_true(g_variant_type_equal(NM_VARIANT_TYPE_SETTING, g_variant_type_next(i)));

  ts_t_equ("a{sv}", NM_VARIANT_TYPE_SETTING);

}

void nm2(const gboolean auto_ap_manual_else){

  NMClient *client=nm2_init();

  nm2_wireless_disable(&client); g_print("\n");

  GPtrArray *wificons=nm2_getwificons(client);
  for(guint i=0; i<(wificons->len); ++i)
    nm2_autoconnect_toggle(client, wificons->pdata[i], auto_ap_manual_else);
  g_ptr_array_unref(wificons); wificons=NULL;

  nm2_wireless_enable(&client); g_print("\n");

  g_object_unref(client); client=NULL;

}
