#include "system_api.h"

static const cm_system_info system_info = {
  CM_APP_NAME,
  CM_APP_ID,
  CM_APP_VERSION,
  CM_RELEASE_CHANNEL,
  CM_DEVELOPER,
  CM_BUILD_ID,
  CM_API_VERSION
};

const cm_system_info *cm_system_get_info(void)
{
  return &system_info;
}
