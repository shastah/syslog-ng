#include <criterion/criterion.h>
#include "appmodel.h"
#include "application.h"
#include "plugin.h"
#include "apphook.h"
#include "cfg-grammar.h"
#include "config_parse_lib.h"

Application *
_parse_application(const char *appmodel, const gchar *name, const gchar *specialization)
{
  AppModelContext *ac;

  cfg_load_module(configuration, "appmodel");
  cr_assert(parse_config(appmodel, LL_CONTEXT_ROOT, NULL, NULL),
            "Parsing the given configuration failed: %s", appmodel);
  ac = appmodel_get_context(configuration);
  return appmodel_context_lookup_application(ac, name, specialization);
}

Test(appmodel, empty_application_can_be_parsed_properly)
{
  Application *app;

  app = _parse_application("application foobar[*] {};", "foobar", "*");
  cr_assert(app != NULL);
  cr_assert_str_eq(app->name, "foobar");
  cr_assert_str_eq(app->specialization, "*");
}

Test(appmodel, name_is_parsed_into_name_member)
{
  Application *app;

  app = _parse_application("application name[*] {};", "name", "*");
  cr_assert(app != NULL);
}

Test(appmodel, specialization_in_brackets_is_parsed_into_specialization)
{
  Application *app;

  app = _parse_application("application name[port514] {};", "name", "port514");
  cr_assert(app != NULL);
  cr_assert_str_eq(app->specialization, "port514");
}

Test(appmodel, filter_expressions_can_be_specified_with_a_filter_keyword)
{
  Application *app;

  app = _parse_application(
"application name[port514] {"
"  filter { program(\"kernel\"); };"
"  parser { kv-parser(); };"
"};",
  "name", "port514");
  cr_assert(app != NULL);
  cr_assert_str_eq(app->specialization, "port514");
  cr_assert_str_eq(app->filter_expr, " program(\"kernel\"); ");
}

Test(appmodel, parser_expressions_can_be_specified_with_a_parser_keyword)
{
  Application *app;

  app = _parse_application(
"application name[port514] {"
"  parser { kv-parser(); };"
"  filter { program(\"kernel\"); };"
"};",
  "name", "port514");
  cr_assert(app != NULL);
  cr_assert_str_eq(app->specialization, "port514");
  cr_assert_str_eq(app->parser_expr, " kv-parser(); ");
}

static void
setup(void)
{
  app_startup();
  configuration = cfg_new_snippet();
}

void
teardown(void)
{
  cfg_free(configuration);
  app_shutdown();
}

TestSuite(appmodel, .init = setup, .fini = teardown);
