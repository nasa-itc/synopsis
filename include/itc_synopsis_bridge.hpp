#ifndef __ITC_SYNOPSIS_BRIDGE_HPP__
#define __ITC_SYNOPSIS_BRIDGE_HPP__

#ifdef __cplusplus
extern "C" {
#endif
#include <cmath>
#include "synopsis.hpp"
#include "SqliteASDPDB.hpp"
#include "StdLogger.hpp"
#include "LinuxClock.hpp"
#include "MaxMarginalRelevanceDownlinkPlanner.hpp"
#include "Timer.hpp"
#include "RuleAST.hpp"
#include "StdLogger.hpp"

struct itc_asds;
typedef struct itc_asds itc_asds_t;
itc_asds_t *itc_asds_create();
int itc_asds_invocations(itc_asds_t *itc_asds);

struct itc_app;
typedef struct itc_app itc_app_t;
itc_app_t *itc_app_create();
void itc_app_add(itc_app_t *itc_app, itc_asds_t *itc_asds, std::string instrument_name);
void itc_app_add(itc_app_t *itc_app, itc_asds_t *itc_asds, std::string instrument_name, std::string dp_type);
size_t itc_app_memory_requirement(itc_app_t *itc_app);

struct itc_dpmsg;
typedef struct itc_dpmsg itc_dpmsg_t;
itc_dpmsg_t *itc_dpmsg_create();
itc_dpmsg_t *itc_dpmsg_create(std::string instrument_name, std::string dp_type, std::string dp_uri, std::string meta_uri, bool meta_usage);

#ifdef __cplusplus
}
#endif

#endif /* __ITC_SYNOPSIS_BRIDGE_HPP__*/