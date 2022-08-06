#include <stdio.h>
#include <glib-object.h>
// #include <glib.h> // not enough

#include "hier.h"

// void typedemo(gpointer o){
//   // NM_CONNECTION(obj)
//   // G_TYPE_CHECK_INSTANCE_CAST()
//   // type_check_instance_cast()
//   #define X(r1, r2, r3, r4, r5, r6, F, O) {
//     /*GObject*/            g_assert_true(r1==F(o, G_TYPE_OBJECT));
//     /*NMObject*/           g_assert_true(r2==F(o, NM_TYPE_OBJECT));
//     /*NMConnection*/       g_assert_true(r3==F(o, NM_TYPE_CONNECTION));
//     /*NMRemoteConnection*/ g_assert_true(r4==F(o, NM_TYPE_REMOTE_CONNECTION));
//     /*NMDevice*/           g_assert_true(r5==F(o, NM_TYPE_DEVICE));
//     /*NMSetting*/          g_assert_true(r6==F(o, NM_TYPE_SETTING));
//   }
//   // https://docs.gtk.org/gobject/#function_macros
//   g_assert_true(NM_TYPE_REMOTE_CONNECTION==G_OBJECT_TYPE(o));
//   g_assert_true(!strcmp("NMRemoteConnection",G_OBJECT_TYPE_NAME(o)));
//   g_assert_true(G_TYPE_CHECK_INSTANCE(o));
//   X(1, 1, 1, 1, 0, 0, G_TYPE_CHECK_INSTANCE_TYPE, o);
//   X(1, 0, 0, 0, 0, 0, G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE, o);
//   // https://docs.gtk.org/gobject/#functions
//   X(1, 1, 1, 1, 0, 0, g_type_check_instance_is_a, o);
//   X(1, 0, 0, 0, 0, 0, g_type_check_instance_is_fundamentally_a, o);
//   #undef X
// }

void relationship(gpointer instance, const S *s){
  g_assert_true(G_TYPE_CHECK_INSTANCE(instance));
  g_assert_true(g_type_check_instance(instance));

  // print chain
  // g_type_parent()

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
