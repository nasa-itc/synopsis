#include <gtest/gtest.h>

#include <synopsis.hpp>


// Demonstrate some basic assertions.
TEST(SynopsisTest, TestApplicationInterface) {
    // Test initialization
    Synopsis::Application app;
    Synopsis::Application::Status status;
    status = app.init();
    EXPECT_EQ(Synopsis::Application::Status::SUCCESS, status);
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

