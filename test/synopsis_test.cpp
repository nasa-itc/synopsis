#include <gtest/gtest.h>

#include <synopsis.hpp>


class TestASDS : public Synopsis::ASDS {

    public:

        Synopsis::Status init(void) override {
            return Synopsis::SUCCESS;
        }

        Synopsis::Status process_data_product(Synopsis::DpMsg msg) override {
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
    status = app.add_module(&asds);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    Synopsis::PassthroughASDS pt_asds;
    status = app.add_module(&pt_asds);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    size_t mem_req;
    mem_req = app.memory_requirement();
    EXPECT_EQ(123, mem_req);

    status = app.init();
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);
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

