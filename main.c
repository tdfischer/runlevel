#include <graviton/client/cloud.h>
#include <graviton/server/quickserver.h>

#include <stdlib.h>
#include <string.h>

static void
cb_set (GravitonServiceInterface *iface, gpointer user_data)
{
  graviton_service_interface_call_noref (iface,
                                                   "set",
                                                   NULL,
                                                   "runlevel",
                                                   g_variant_new_int64 ((long int)user_data),
                                                   NULL);
}

static void
cb_get (GravitonServiceInterface *iface, gpointer user_data)
{
  GVariant *val = graviton_service_interface_call (iface,
                                                   "get",
                                                   NULL,
                                                   NULL);
  g_print ("%d\n", (int)g_variant_get_int64 (val));
  g_variant_unref (val);
}

static void
cb_changed (GravitonServiceInterface *iface, gchar* event_name, GVariant *val, gpointer user_data)
{
  int i = g_variant_get_int64 (val);
  g_print ("%d", i);
  g_variant_unref (val);
}

static void
cb_watch (GravitonServiceInterface *iface, gpointer user_data)
{
  GMainLoop *loop = g_main_loop_new (NULL, 0);
  graviton_service_interface_subscribe_events (iface, "tick", NULL);
  g_signal_connect (iface, "event::runlevel-changed", G_CALLBACK (cb_changed), NULL);
  g_main_loop_run (loop);
}

guint runlevel = 0;

static GVariant *
cb_set_runlevel (GravitonService *svc, GHashTable *args, GError **error, gpointer data)
{
  GVariant *arg = g_hash_table_lookup (args, "runlevel");
  if (arg) {
    runlevel = g_variant_get_int64 (arg);
    graviton_service_emit_event (svc, "runlevel-changed", arg);
  }
  return NULL;
}

static GVariant *
cb_get_runlevel (GravitonService *svc, GHashTable *args, GError **error, gpointer data)
{
  return g_variant_new_uint64 (runlevel);
}

void
do_serve ()
{
  GravitonQuickserver* server = graviton_quickserver_new();

  graviton_quickserver_add_method (server, "noisebridge:net:runlevel", "set", cb_set_runlevel, NULL, NULL);
  graviton_quickserver_add_method (server, "noisebridge:net:runlevel", "get", cb_get_runlevel, NULL, NULL);

  graviton_quickserver_run (server);
}

int main(int argc, char** argv)
{
  if (strcmp (argv[1], "serve") == 0) {
    do_serve();
    return 0;
  }

  GravitonCloud *cloud = graviton_cloud_new_default_cloud ();

  if (strcmp (argv[1], "set") == 0) {
    graviton_cloud_foreach_service (cloud, "noisebridge:net:runlevel", cb_set, (void*)(long long)atoi(argv[2]));
  } else if (strcmp (argv[1], "get") == 0) {
    graviton_cloud_foreach_service (cloud, "noisebridge:net:runlevel", cb_get, NULL);
  } else if (strcmp (argv[1], "watch") == 0) {
    //TODO
    graviton_cloud_foreach_service (cloud, "noisebridge:net:runlevel", cb_watch, NULL);
  }

  return 0;
}

