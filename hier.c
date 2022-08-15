#include <stdio.h>
#include <glib-object.h>
// #include <glib.h> // not enough

#include "hier.h"

/**
 * relationship(rc, (const S[]){
 *   {"OO", G_TYPE_OBJECT},
 *   {" O", NM_TYPE_OBJECT},
 *   {" O", NM_TYPE_CONNECTION},
 *   {" O", NM_TYPE_REMOTE_CONNECTION},
 *   {"  ", NM_TYPE_DEVICE},
 *   {"  ", NM_TYPE_SETTING},
 *   {"  ", NM_TYPE_SETTING_CONNECTION},
 *   {NULL, G_TYPE_NONE}
 * });
 * relationship(nm_connection_get_setting_connection(c), (const S[]){
 *   {"OO", G_TYPE_OBJECT},
 *   {"  ", NM_TYPE_OBJECT},
 *   {"  ", NM_TYPE_CONNECTION},
 *   {"  ", NM_TYPE_REMOTE_CONNECTION},
 *   {"  ", NM_TYPE_DEVICE},
 *   {" O", NM_TYPE_SETTING},
 *   {" O", NM_TYPE_SETTING_CONNECTION},
 *   {NULL, G_TYPE_NONE}
 * });
 **/
void relationship(gpointer instance, const S *s){
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

// doesn't work if child is not instantiated
// static void children_down_tree_dfs(const guint indent, const GType t){
//   // if(indent>0)
//   //   return;
//   printf("%*s%s\n", indent*2, "", g_type_name(t));
//   nm_setting_connection_new();
//   guint n_children=0;
//   GType *child=g_type_children(t, &n_children);
//   printf("%u\n", n_children);
//   // while(child){
//   //   children_down_tree_dfs(indent+1, *child);
//   //   ++child;
//   // }
//   // puts("");
// }

static inline void parents_up_chain(const GType t){
  guint d=g_type_depth(t);
  g_assert_true(1<=d);
  --d;
  printf("[%d] ", d);
  GType parents[d]; bzero(parents, d*sizeof(GType));
  GType p=t;
  for(gint i=d-1; i>=0; --i){
    p=g_type_parent(p);
    parents[i]=p;
  }
  for(guint i=0; i<d; ++i){
    printf("%s -> ", g_type_name(parents[i]));
  }
  puts(g_type_name(t));
}

void type_info(const GType t){
  g_assert_true(G_TYPE_NONE!=t);
  parents_up_chain(t);
  // children_down_tree_dfs(0, t);
  // g_type_children()
  // puts();
  // G_TYPE_FUNDAMENTAL()
}

// int main(){

//   // register before looking up
//   // NMSettingConnection *garbage=nm_setting_connection_new();
//   // puts(g_type_name(g_type_parent(g_type_from_name("NMSettingConnection"))));
//   // g_object_unref(garbage);

//   // type_info(G_TYPE_OBJECT);
//   // type_info(NM_TYPE_OBJECT);
//   // type_info(NM_TYPE_DEVICE);
//   // type_info(NM_TYPE_REMOTE_CONNECTION);
//   // type_info(NM_TYPE_SETTING);
//   // type_info(NM_TYPE_SETTING_CONNECTION);
//   // type_info(NM_TYPE_CONNECTION);

//   nm();

//   return 0;

// }
