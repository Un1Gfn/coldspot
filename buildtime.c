#include <glib.h> // g_print()

#include "buildtime.h"

void buildtime(){
  const char *const utc=__TIME__;
  g_print("[build @ %s UTC]\n", utc);
  // {
  //   struct tm tm={0};
  //   g_assert_true(utc+2+1+2+1+2==strptime(utc, "%T", &tm));
  //   //     strptime()    mktime        gmtime    strftime
  //   // str            tm        time_t        tm          time_t
  //   char local[9]={0};
  //   g_assert_true(8==strftime(local, 9, "%T", gmtime(&(time_t){mktime(&tm)})));
  //   g_print("%s\n", local);
  //   g_print("\n");
  //   exit(0);
  // }
}
