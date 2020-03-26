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

G_DEFINE_TYPE_WITH_PRIVATE (PdPlugin, pd_plugin, G_TYPE_OBJECT)

gpointer
pd_plugin_is_function_supported (PdPlugin *self, const gchar *func_name)
{
    gpointer symbol = NULL;
    PdPluginPrivate *priv;

    priv = pd_plugin_get_instance_private (self);

    g_module_symbol (priv->module, func_name, &symbol);
    if (symbol == NULL)
    {
        return NULL;
    }
    else
    {
        g_hash_table_insert (priv->vfuncs, g_strdup (func_name), symbol);
        return symbol;
    }
}


void
pd_plugin_setup (PdPlugin *self,
                 const gchar *filename)
{
    PdPluginPrivate *priv;

    priv = pd_plugin_get_instance_private (self);
    priv->name = g_strdup (filename + 7);
    priv->module = g_module_open (filename, G_MODULE_BIND_LAZY);

    /* Load all the vfuncs in the hash table. There is no way to do this. */
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
