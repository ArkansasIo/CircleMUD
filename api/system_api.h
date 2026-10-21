#ifndef CIRCLEMUD_SYSTEM_API_H
#define CIRCLEMUD_SYSTEM_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define CM_API_NAME "CircleMUD Application API"
#define CM_API_VERSION "0.1"
#define CM_APP_NAME "CircleMUD Game Engine"
#define CM_APP_ID "ARK-CIRCLE-GE"
#define CM_APP_VERSION "0.1.0"
#define CM_RELEASE_CHANNEL "pre-alpha"
#define CM_DEVELOPER "ArkansasIo"
#define CM_BUILD_ID "ARK-CIRCLE-GE-0.1.0-PA-0001"

typedef struct cm_system_info {
  const char *name;
  const char *id;
  const char *version;
  const char *channel;
  const char *developer;
  const char *build_id;
  const char *api_version;
} cm_system_info;

const cm_system_info *cm_system_get_info(void);

#ifdef __cplusplus
}
#endif

#endif
