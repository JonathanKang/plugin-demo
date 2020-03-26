#ifndef PD_PLUGIN_LOADER_H_
#define PD_PLUGIN_LOADER_H_

#include <glib-object.h>

#define PD_TYPE_PLUGIN_LOADER (pd_plugin_loader_get_type ())
G_DECLARE_FINAL_TYPE (PdPluginLoader, pd_plugin_loader, PD, PLUGIN_LOADER, GObject)

void pd_plugin_loader_call_func (PdPluginLoader *plugin_loader,
                                 const gchar *func_name);
void pd_plugin_loader_setup (PdPluginLoader *plugin_loader);
PdPluginLoader *pd_plugin_loader_new (void);

#endif
