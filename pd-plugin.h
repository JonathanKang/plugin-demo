#ifndef PD_PLUGIN_H_
#define PD_PLUGIN_H_

#include <glib-object.h>

#define PD_TYPE_PLUGIN (pd_plugin_get_type ())
G_DECLARE_FINAL_TYPE (PdPlugin, pd_plugin, PD, PLUGIN, GObject)

gpointer pd_plugin_is_function_supported (PdPlugin *plugin, const gchar *func_name);
void pd_plugin_setup (PdPlugin *plugin, const gchar *filename);
PdPlugin *pd_plugin_new (void);

#endif
