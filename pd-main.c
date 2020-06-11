#include <glib.h>

#include "pd-plugin.h"
#include "pd-plugin-job.h"
#include "pd-plugin-loader.h"

static void
plugin_finished_cb (GObject *object,
                    GAsyncResult *result,
                    gpointer user_data)
{
    GMainLoop *main_loop = user_data;

    g_main_loop_quit (main_loop);
}

int
main (int argc, char **argv)
{
    GMainLoop *main_loop;
    PdPluginJob *plugin_job;
    PdPluginLoader *plugin_loader;

    main_loop = g_main_loop_new (NULL, FALSE);
    plugin_job = pd_plugin_job_newv (PD_PLUGIN_ACTION_HELLO_WORLD, NULL);

    plugin_loader = pd_plugin_loader_new ();
    pd_plugin_loader_setup (plugin_loader);
    pd_plugin_loader_call_func_async (plugin_loader, plugin_job, NULL,
                                      plugin_finished_cb, main_loop);

    g_main_loop_run (main_loop);

    g_main_loop_unref (main_loop);
    g_object_unref (plugin_job);
    g_object_unref (plugin_loader);

    return 0;
}
