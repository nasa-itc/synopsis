#ifndef __ITC_SYNOPSIS_BRIDGE_HPP__
#define __ITC_SYNOPSIS_BRIDGE_HPP__

#include <gtest/gtest.h>
#include <cmath>
#include "synopsis.hpp"
#include "SqliteASDPDB.hpp"
#include "StdLogger.hpp"
#include "LinuxClock.hpp"
#include "MaxMarginalRelevanceDownlinkPlanner.hpp"
#include "Timer.hpp"
#include "RuleAST.hpp"
#include "StdLogger.hpp"

#ifdef __cplusplus
extern "C" {
#endif

struct itc_dpmsg;
typedef struct itc_dpmsg itc_dpmsg_t;
itc_dpmsg_t* itc_create_dpmsg(std::string instrument_name, std::string dp_type,
                std::string dp_uri, std::string meta_uri, bool meta_usage);
void itc_setup_testasds();
void itc_setup_ptasds();
size_t itc_app_get_memory_requiremennt();
void itc_app_init(size_t bytes, void* memory);
int itc_app_get_invocations();
void itc_app_accept_dpmsg();

#ifdef __cplusplus
}
#endif

#endif /* __ITC_SYNOPSIS_BRIDGE_HPP__*/