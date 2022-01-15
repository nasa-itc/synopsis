#include <gtest/gtest.h>

#include <synopsis.hpp>


class TestASDS : public Synopsis::ASDS {

    public:

        int invocations;

        Synopsis::Status init(size_t bytes, void* memory) override {
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


// Demonstrate some basic assertions.
TEST(SynopsisTest, TestApplicationInterface) {
    // Test initialization
    Synopsis::Application app;
    Synopsis::Status status;

    TestASDS asds;
    status = app.add_asds("test_instrument", &asds);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    Synopsis::PassthroughASDS pt_asds;
    status = app.add_asds("test_instrument", "type", &pt_asds);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    size_t mem_req;
    mem_req = app.memory_requirement();
    EXPECT_EQ(128, mem_req);

    status = app.init(0, NULL);
    EXPECT_EQ(Synopsis::Status::FAILURE, status);

    void* mem = malloc(128);
    status = app.init(128, mem);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    Synopsis::DpMsg msg(
        "test_instrument", "test_type",
        "file::///data/file.dat",
        "file::///data/meta.dat",
        true
    );

    EXPECT_EQ(0, asds.invocations);
    status = app.accept_dp(msg);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
    EXPECT_EQ(1, asds.invocations);

    status = app.deinit();
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
    EXPECT_EQ(0, asds.invocations);
}

TEST(SynopsisTest, TestDpMsg) {

    Synopsis::DpMsg msg;
    EXPECT_EQ(msg.get_instrument_name(), "");
    EXPECT_EQ(msg.get_type(), "");
    EXPECT_EQ(msg.get_uri(), "");
    EXPECT_EQ(msg.get_metadata_uri(), "");
    EXPECT_EQ(msg.get_metadata_usage(), false);

    Synopsis::DpMsg msg2(
        "cntx", "flat",
        "file::///data/file.dat",
        "file::///data/meta.dat",
        true
    );
    EXPECT_EQ(msg2.get_instrument_name(), "cntx");
    EXPECT_EQ(msg2.get_type(), "flat");
    EXPECT_EQ(msg2.get_uri(), "file::///data/file.dat");
    EXPECT_EQ(msg2.get_metadata_uri(), "file::///data/meta.dat");
    EXPECT_EQ(msg2.get_metadata_usage(), true);

}

