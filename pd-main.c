#include <glib.h>

#include "pd-plugin.h"
#include "pd-plugin-loader.h"

int
main (int argc, char **argv)
{
    PdPluginLoader *plugin_loader;

    plugin_loader = pd_plugin_loader_new ();
    pd_plugin_loader_setup (plugin_loader);
    pd_plugin_loader_call_func (plugin_loader, "pd_plugin_print");
    g_object_unref (plugin_loader);

    return 0;
}
