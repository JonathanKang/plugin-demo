#include "config.h"

#include "pd-plugin.h"
#include "pd-plugin-loader.h"

struct _PdPluginLoader
{
    /*< private >*/
    GObject parent_instance;
};

typedef struct
{
    GPtrArray *plugins;
} PdPluginLoaderPrivate;

typedef void (*PluginFunc) (PdPlugin *plugin);

G_DEFINE_TYPE_WITH_PRIVATE (PdPluginLoader, pd_plugin_loader, G_TYPE_OBJECT)

static void
pd_plugin_loader_call_func (GTask *task,
                            gpointer object,
                            gpointer task_data,
                            GCancellable *cancellable)
{
    guint i;
    PdPluginAction action;
    PdPluginJob *plugin_job = PD_PLUGIN_JOB (task_data);
    PdPluginLoader *self = PD_PLUGIN_LOADER (object);
    PdPluginLoaderPrivate *priv;

    priv = pd_plugin_loader_get_instance_private (self);
    action = pd_plugin_job_get_action (plugin_job);

    /* Iterate all the plugins, check whether it supports this function,
     * and run it if it does. */
    for (i = 0; i < priv->plugins->len; i++)
    {
        gpointer symbol;
        PdPlugin *plugin;

        plugin = g_ptr_array_index (priv->plugins, i);
        symbol = pd_plugin_is_function_supported (plugin,
                                                  pd_plugin_job_action_to_function_name (action));
        if (symbol != NULL)
        {
            PluginFunc plugin_func;

            /* Function is supported by this plugin, run it. */
            plugin_func = symbol;
            plugin_func (plugin);
        }
        else
        {
            g_warning ("Function not found.");
        }
    }

    return;
}

void
pd_plugin_loader_call_func_async (PdPluginLoader *self,
                                  PdPluginJob *plugin_job,
                                  GCancellable *cancellable,
                                  GAsyncReadyCallback callback,
                                  gpointer user_data)
{
    g_autoptr (GTask) task = NULL;

    task = g_task_new (self, cancellable, callback, user_data);
    g_task_set_task_data (task, plugin_job, NULL);

    g_task_run_in_thread (task, pd_plugin_loader_call_func);
}

void
pd_plugin_loader_setup (PdPluginLoader *self)
{
    const gchar *filename;
    g_autofree gchar *plugin_dir = NULL;
    g_autoptr(GError) error = NULL;
    GDir *plugin_directory;
    PdPluginLoaderPrivate *priv;

    priv = pd_plugin_loader_get_instance_private (self);

    plugin_dir = g_build_filename (LIBDIR, "pd-plugins", NULL);
    plugin_directory = g_dir_open (plugin_dir, 0, &error);
    while ((filename = g_dir_read_name (plugin_directory)) != NULL)
    {
        /* Read this library file and setup related plugin. */
        if (g_strrstr (filename, ".so") != NULL)
        {
            g_autofree gchar *full_fn = NULL;
            PdPlugin *plugin;

            full_fn = g_build_filename (plugin_dir, filename, NULL);
            plugin = pd_plugin_new ();
            pd_plugin_setup (plugin, full_fn);

            g_ptr_array_add (priv->plugins, plugin);
        }
    }
}

static void
pd_plugin_loader_finalize (GObject *object)
{
    PdPluginLoader *self;
    PdPluginLoaderPrivate *priv;

    self = PD_PLUGIN_LOADER (object);
    priv = pd_plugin_loader_get_instance_private (self);

    g_ptr_array_free (priv->plugins, TRUE);

    G_OBJECT_CLASS (pd_plugin_loader_parent_class)->finalize (object);
}

static void
pd_plugin_loader_class_init (PdPluginLoaderClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = pd_plugin_loader_finalize;
}

static void
pd_plugin_loader_init (PdPluginLoader *self)
{
    PdPluginLoaderPrivate *priv;

    priv = pd_plugin_loader_get_instance_private (self);

    priv->plugins = g_ptr_array_new_with_free_func ((GDestroyNotify) g_object_unref);
}

PdPluginLoader *
pd_plugin_loader_new (void)
{
    return g_object_new (PD_TYPE_PLUGIN_LOADER, NULL);
}
