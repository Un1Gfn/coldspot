#include <elogind/sd-login.h>
#include <glib-object.h>
#include <glib.h>
#include <NetworkManager.h>
#include <stdio.h>
#include <unistd.h> // sleep()

#include "buildtime.h"
#include "nm2.h"

static inline gboolean active_session(){
  char *session=NULL;
  if(-ENODATA==sd_pid_get_session(getpid(), &session)){
    g_assert_true(!session);
    return FALSE;
  }
  g_assert_true(session && (2==strlen(session)));
  g_free(session); session=NULL;
  return TRUE;
}

int main(const int argc, const char *const argv[]){

  g_print("\n");

  buildtime();
  g_print("\n");

  g_assert_true(2==argc && argv[1]);

  nm2_demo1_gvtrel();

  // permission check
  const uid_t u=getuid(); g_assert_true(
    (0==u) ||
    ((10000==u) && active_session())
  );

  nm2((0==g_strcmp0("ap", argv[1])));

  g_print("bye\n");
  g_print("\n");
  return 0;

}
