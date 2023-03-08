#include "itc_synopsis_bridge.hpp"

// TODO:  MAKE USE OF STATUS VARIABLE
// TODO:  Make Branch, and issue, and push up
// TODO:  Create a test from the wrappers?

//create an asds
//create App
//add ASDS to APP
//init db
//insert dp
//update_priority_bin
//accept_dp
//APP_deinit

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

static TestASDS itc_asds;
Synopsis::PassthroughASDS pt_asds;
Synopsis::SqliteASDPDB db(":memory:");
Synopsis::StdLogger logger;
Synopsis::LinuxClock clock2;
Synopsis::MaxMarginalRelevanceDownlinkPlanner planner;
Synopsis::Application app(&db, &planner, &logger, &clock2);
void* memory = malloc(128);

void itc_setup_testasds(){
    Synopsis::Status status;
    status = app.add_asds("test_instrument", &itc_asds);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

void itc_setup_ptasds(){
    Synopsis::Status status;
    status = app.add_asds("test_instrument", "type", &pt_asds);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

size_t itc_app_get_memory_requiremennt(){
    size_t mem_req_bytes = 0;
    mem_req_bytes = app.memory_requirement();        
    printf("REQ Bytes: %ld\n", mem_req_bytes);
    return mem_req_bytes;
}

void itc_app_init(size_t bytes, void* memory){ // Remember to free memory
    Synopsis::Status status;
    status = app.init(bytes, memory);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

void itc_app_deinit(){
    Synopsis::Status status;
    status = app.deinit();
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

int itc_app_get_invocations(){
    int invocations = itc_asds.invocations;
    printf("INVOCATIONS: %d\n", invocations);
    return invocations;
}

struct itc_dpmsg {
    void *obj;
};

itc_dpmsg_t* itc_create_dpmsg(std::string instrument_name, std::string dp_type, std::string dp_uri, std::string meta_uri, bool meta_usage){
    itc_dpmsg_t *msg;
    Synopsis::DpMsg *dpobj;

    msg = (typeof(msg))malloc(sizeof(*msg));
    dpobj = new Synopsis::DpMsg(instrument_name, dp_type, dp_uri, meta_uri, meta_usage);

    msg->obj = dpobj;

    return msg;
}

void itc_app_accept_dpmsg(){
    Synopsis::Status status;
    Synopsis::DpMsg msg(
        "test_instrument", "test_type",
        "file::///data/file.dat",
        "file::///data/meta.dat",
        true
    );

    status = app.accept_dp(msg);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

void itc_app_accept_dpmsg(itc_dpmsg_t* msg){
    Synopsis::Status status;
    Synopsis::DpMsg *tempmsg = static_cast<Synopsis::DpMsg *>(msg->obj);
    Synopsis::DpMsg real_msg(tempmsg->get_instrument_name(), tempmsg->get_type(), tempmsg->get_uri(), tempmsg->get_metadata_uri(), tempmsg->get_metadata_usage());
    status = app.accept_dp(real_msg);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

TEST(SynopsisTest, TestCWrapper)
{
    size_t mem_req_bytes = 0;

    itc_setup_testasds();
    itc_setup_ptasds();
    mem_req_bytes = itc_app_get_memory_requiremennt();

    itc_app_init(mem_req_bytes, memory);
    EXPECT_EQ(0, itc_app_get_invocations());

    itc_app_accept_dpmsg();
    EXPECT_EQ(1, itc_app_get_invocations());
    
    itc_app_accept_dpmsg();
    EXPECT_EQ(2, itc_app_get_invocations());

    itc_dpmsg_t *temp_dpmsg = itc_create_dpmsg("test_instrument", "test_type", "file::///data/file.dat", "file::///data/meta.dat", true);
    itc_app_accept_dpmsg(temp_dpmsg);
    EXPECT_EQ(3, itc_app_get_invocations());

    itc_app_deinit();
    EXPECT_EQ(0, itc_app_get_invocations());
}