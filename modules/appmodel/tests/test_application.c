#include <criterion/criterion.h>
#include "application.h"

Test(application, empty_application_can_be_created_and_freed)
{
  Application *app;

  app = application_new("foobar", "*");
  application_free(app);
}

Test(application, filter_can_be_set_and_queried)
{
  Application *app;
  const gchar *filter_expr = "'1' eq '1'";
  const gchar *filter_expr2 = "'2' eq '2'";

  app = application_new("foobar", "*");
  application_set_filter(app, filter_expr);
  cr_assert_str_eq(app->filter_expr, filter_expr);

  application_set_filter(app, filter_expr2);
  cr_assert_str_eq(app->filter_expr, filter_expr2);
  application_free(app);
}

Test(application, parser_can_be_set_and_queried)
{
  Application *app;
  const gchar *parser_expr = "kv-parser();";
  const gchar *parser_expr2 = "csv-parser();";

  app = application_new("foobar", "*");
  application_set_parser(app, parser_expr);
  cr_assert_str_eq(app->parser_expr, parser_expr);

  application_set_parser(app, parser_expr2);
  cr_assert_str_eq(app->parser_expr, parser_expr2);
  application_free(app);
}
