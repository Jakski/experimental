#include "json.h"

bool print_json_key(const char *text, const char *key)
{
  json_t *root, *field;
  json_error_t error;
  bool ret = true;

  root = json_loads(text, 0, &error);
  if(!root)
  {
    fprintf(stderr, "Error on line %d: %s\n", error.line, error.text);
    ret = false;
    goto exit;
  }
  field = json_object_get(root, key);
  if(!json_is_string(field))
  {
    fprintf(stderr, "Field \"%s\" isn't a string!\n", key);
    ret = false;
    goto exit;
  }
  const char *field_text = json_string_value(field);
  if(!field_text)
  {
    fprintf(stderr, "Field \"%s\" isn't JSON string\n", key);
    ret = false;
    goto exit;
  }

  printf("Found: %s\n", field_text);

exit:
  json_decref(root);
  return ret;
}
