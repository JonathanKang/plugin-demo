#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum
{
    PD_PLUGIN_ACTION_UNKNOWN,
    PD_PLUGIN_ACTION_HELLO_WORLD,
    PD_PLUGIN_ACTION_LAST
} PdPluginAction;

#define PD_TYPE_PLUGIN_JOB (pd_plugin_job_get_type ())

G_DECLARE_FINAL_TYPE (PdPluginJob, pd_plugin_job, PD, PLUGIN_JOB, GObject)

PdPluginAction pd_plugin_job_get_action (PdPluginJob *plugin_job);

#define pd_plugin_job_newv(a,...) PD_PLUGIN_JOB(g_object_new (PD_TYPE_PLUGIN_JOB, "action", a, __VA_ARGS__))

G_END_DECLS
