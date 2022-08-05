#include <assert.h>
#include <glib.h>
#include <NetworkManager.h>

int main(){

  NMClient *client=nm_client_new(NULL, NULL);
  assert(client);

  g_print("NetworkManager version: %s\n", nm_client_get_version(client));

  g_object_unref(client); client=NULL;
  return 0;

}
