#include "pd-enums.h"
#include "pd-plugin-job.h"

struct _PdPluginJob
{
		GObject parent_instance;

    PdPluginAction action;
};

G_DEFINE_TYPE (PdPluginJob, pd_plugin_job, G_TYPE_OBJECT)

enum {
      PROP_0,
      PROP_ACTION,
      N_PROPS
};

static GParamSpec *properties [N_PROPS];

const gchar *
pd_plugin_job_action_to_function_name (PdPluginAction action)
{
    if (action == PD_PLUGIN_ACTION_HELLO_WORLD)
    {
        return "pd_plugin_print";
    }

    return NULL;
}

PdPluginAction pd_plugin_job_get_action (PdPluginJob *self)
{
    return self->action;
}

static void
pd_plugin_job_finalize (GObject *object)
{
    PdPluginJob *self = (PdPluginJob *)object;

    G_OBJECT_CLASS (pd_plugin_job_parent_class)->finalize (object);
}

static void
pd_plugin_job_get_property (GObject    *object,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    PdPluginJob *self = PD_PLUGIN_JOB (object);

		switch (prop_id)
    {
    case PROP_ACTION:
        g_value_set_enum (value, self->action);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
pd_plugin_job_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    PdPluginJob *self = PD_PLUGIN_JOB (object);

		switch (prop_id)
    {
    case PROP_ACTION:
        self->action = g_value_get_enum (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
pd_plugin_job_class_init (PdPluginJobClass *klass)
{
		GObjectClass *object_class = G_OBJECT_CLASS (klass);

		object_class->finalize = pd_plugin_job_finalize;
		object_class->get_property = pd_plugin_job_get_property;
		object_class->set_property = pd_plugin_job_set_property;

    properties[PROP_ACTION] = g_param_spec_enum ("action", NULL, NULL,
                                                 PD_TYPE_PLUGIN_ACTION,
                                                 PD_PLUGIN_ACTION_UNKNOWN,
                                                 G_PARAM_READWRITE);
    g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
pd_plugin_job_init (PdPluginJob *self)
{
}
