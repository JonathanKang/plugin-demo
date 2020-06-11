#ifndef PD_PLUGIN_LOADER_H_
#define PD_PLUGIN_LOADER_H_

#include <gio/gio.h>
#include <glib-object.h>

#include "pd-plugin-job.h"

#define PD_TYPE_PLUGIN_LOADER (pd_plugin_loader_get_type ())
G_DECLARE_FINAL_TYPE (PdPluginLoader, pd_plugin_loader, PD, PLUGIN_LOADER, GObject)

void pd_plugin_loader_call_func_async (PdPluginLoader *plugin_loader,
                                       PdPluginJob *plugin_job,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data);
void pd_plugin_loader_setup (PdPluginLoader *plugin_loader);
PdPluginLoader *pd_plugin_loader_new (void);

#endif
