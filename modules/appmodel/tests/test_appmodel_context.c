#include <criterion/criterion.h>
#include "appmodel.h"

static AppModelContext *ac;

static void
startup(void)
{
  ac = appmodel_context_new();
}

static void
teardown(void)
{
  appmodel_context_free(ac);
  ac = NULL;
}

Test(appmodel_context, register_application_makes_the_app_available)
{
  appmodel_context_register_application(ac, application_new("foobar", "*"));
  appmodel_context_lookup_application(ac, "foobar", "*");
}

static void
_foreach_app(Application *app, Application *base_app, gpointer user_data)
{
  GString *result = (GString *) user_data;

  g_string_append(result, app->name);
}

Test(appmodel_context, iter_applications_enumerates_apps_without_asterisk)
{
  GString *result = g_string_sized_new(128);

  appmodel_context_register_application(ac, application_new("foo", "*"));
  appmodel_context_register_application(ac, application_new("foo", "port514"));
  appmodel_context_register_application(ac, application_new("bar", "*"));
  appmodel_context_register_application(ac, application_new("bar", "port514"));
  appmodel_context_register_application(ac, application_new("baz", "*"));
  appmodel_context_register_application(ac, application_new("baz", "port514"));
  appmodel_context_iter_applications(ac, _foreach_app, result);
  cr_assert_str_eq(result->str, "foobarbaz");
  g_string_free(result, TRUE);
}

Test(appmodel_context, iter_applications_enumerates_apps_in_the_order_of_registration)
{
  GString *result = g_string_sized_new(128);

  appmodel_context_register_application(ac, application_new("baz", "*"));
  appmodel_context_register_application(ac, application_new("baz", "port514"));
  appmodel_context_register_application(ac, application_new("bar", "*"));
  appmodel_context_register_application(ac, application_new("bar", "port514"));
  appmodel_context_register_application(ac, application_new("foo", "*"));
  appmodel_context_register_application(ac, application_new("foo", "port514"));
  appmodel_context_iter_applications(ac, _foreach_app, result);
  cr_assert_str_eq(result->str, "bazbarfoo");
  g_string_free(result, TRUE);
}

TestSuite(appmodel_context, .init = startup, .fini = teardown);
