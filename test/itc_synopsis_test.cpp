#include "itc_synopsis_bridge.hpp"

// TODO:  MAKE USE OF STATUS VARIABLE
// TODO:  Make Branch, and issue, and push up
// TODO:  Create a test from the wrappers?

class TestASDS : public Synopsis::ASDS {

    public:

        int invocations;

        Synopsis::Status init(size_t bytes, void* memory, Synopsis::Logger *logger) override {
            this->_logger = logger;
            this->invocations = 0;
            return Synopsis::SUCCESS;
        }

        Synopsis::Status deinit() override {
            this->invocations = 0;
            return Synopsis::SUCCESS;
        }

        Synopsis::Status process_data_product(Synopsis::DpMsg msg) override {
            this->invocations++;
            return Synopsis::SUCCESS;
        }

        size_t memory_requirement(void) override {
            return 123;
        }
};

struct itc_asds 
{
    void *obj;
};

itc_asds_t *itc_asds_create()
{
    itc_asds_t *itc_asds;
    TestASDS *obj;

    itc_asds = (typeof(itc_asds))malloc(sizeof(*itc_asds));
    obj = new TestASDS;
    itc_asds->obj = obj;

    return itc_asds;
} 

struct itc_app
{
    void *obj;
};

itc_app_t *itc_app_create() //TODO:  Do we want to pass in the db/planner/logger/clock?
{
    itc_app_t *itc_app;

    Synopsis::SqliteASDPDB db(":memory:");
    Synopsis::StdLogger logger;
    Synopsis::LinuxClock clock;
    Synopsis::MaxMarginalRelevanceDownlinkPlanner planner;

    Synopsis::Application *obj;
    itc_app = (typeof(itc_app))malloc(sizeof(*itc_app));
    obj = new Synopsis::Application(&db, &planner, &logger, &clock);

    itc_app->obj = obj;

    return itc_app;
}

void itc_app_add(itc_app_t *itc_app, itc_asds_t *itc_asds, std::string instrument_name)
{
    Synopsis::Application *obj;

    if (itc_app == NULL)
        return;
    
    obj = static_cast<Synopsis::Application *>(itc_app->obj);
    obj->add_asds(instrument_name, static_cast<TestASDS *>(itc_asds->obj)); //TODO:  If issues happen, this could be a culprit
}

void itc_app_add(itc_app_t *itc_app, itc_asds_t *itc_asds, std::string instrument_name, std::string dp_type)
{
    Synopsis::Application *obj;

    if (itc_app == NULL)
        return;
    
    obj = static_cast<Synopsis::Application *>(itc_app->obj);
    obj->add_asds(instrument_name, dp_type, static_cast<TestASDS *>(itc_asds->obj)); //TODO:  If issues happen, this could be a culprit
}

size_t itc_app_memory_requirement(itc_app_t *itc_app)
{
    Synopsis::Application *obj;

    if (itc_app == NULL)
        return;
    
    obj =  static_cast<Synopsis::Application *>(itc_app->obj);
    return obj->memory_requirement();
}

void itc_app_init(itc_app_t *itc_app, size_t bytes, void *memory)
{
    Synopsis::Application *obj;

    if (itc_app == NULL)
        return;
    
    obj = static_cast<Synopsis::Application *>(itc_app->obj);
    obj->init(bytes, memory);
}

int itc_asds_invocations(itc_asds_t *itc_asds)
{
    TestASDS *obj;

    if (itc_asds == NULL)
        return;
    
    obj = static_cast<TestASDS *>(itc_asds->obj);
    int invocations = obj->invocations;
    return invocations;
}

struct itc_dpmsg
{
    void *obj;
};

itc_dpmsg_t *itc_dpmsg_create()
{
    itc_dpmsg_t *msg;
    Synopsis::DpMsg *obj;

    msg = (typeof(msg))malloc(sizeof(*msg));
    obj = new Synopsis::DpMsg();
    msg->obj = obj;

    return msg;
}

itc_dpmsg_t *itc_dpmsg_create(std::string instrument_name, std::string dp_type, std::string dp_uri, std::string meta_uri, bool meta_usage)
{
    itc_dpmsg_t *msg;
    Synopsis::DpMsg *obj;

    msg = (typeof(msg))malloc(sizeof(*msg));
    obj = new Synopsis::DpMsg(instrument_name, dp_type, dp_uri, meta_uri, meta_usage);
    msg->obj = obj;

    return msg;
}

void itc_app_accept_dp(itc_app_t *itc_app, itc_dpmsg_t *msg)
{
    Synopsis::Application *obj;
    Synopsis::DpMsg *msg_obj;

    if ((itc_app == NULL) || (msg == NULL))
        return;
    
    obj = static_cast<Synopsis::Application *>(itc_app->obj);
    msg_obj = static_cast<Synopsis::DpMsg *>(msg->obj);
    obj->accept_dp(static_cast<Synopsis::DpMsg>(*msg_obj));  //TODO:  COULD BE AN ISSUE HERE

}

