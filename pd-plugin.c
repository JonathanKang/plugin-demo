#include <glib-object.h>
#include <gmodule.h>

#include "pd-plugin.h"

struct _PdPlugin
{
    /*< private >*/
    GObject parent_instance;
};

typedef struct
{
    gchar *name;
    GHashTable *vfuncs;
    GModule *module;
} PdPluginPrivate;

typedef void (*PluginFunc) (PdPlugin *plugin);

G_DEFINE_TYPE_WITH_PRIVATE (PdPlugin, pd_plugin, G_TYPE_OBJECT)

void
pd_plugin_setup (PdPlugin *self,
                 const gchar *filename)
{
    gpointer symbol = NULL;
    PluginFunc plugin_func;
    PdPluginPrivate *priv;

    priv = pd_plugin_get_instance_private (self);
    priv->name = g_strdup (filename + 7);
    priv->module = g_module_open (filename, G_MODULE_BIND_LAZY);
    g_module_symbol (priv->module, "pd_plugin_print", &symbol);

    plugin_func = symbol;
    plugin_func (self);
}

static void
pd_plugin_finalize (GObject *object)
{
    PdPlugin *self;
    PdPluginPrivate *priv;

    self = PD_PLUGIN (object);
    priv = pd_plugin_get_instance_private (self);

    g_free (priv->name);
    g_hash_table_unref (priv->vfuncs);
    if (priv->module != NULL)
    {
        g_module_close (priv->module);
    }
}

static void
pd_plugin_class_init (PdPluginClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = pd_plugin_finalize;
}

static void
pd_plugin_init (PdPlugin *self)
{
    PdPluginPrivate *priv;

    priv = pd_plugin_get_instance_private (self);

    priv->name = NULL;
    priv->vfuncs = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
}

PdPlugin *
pd_plugin_new (void)
{
    return g_object_new (PD_TYPE_PLUGIN, NULL);
}
