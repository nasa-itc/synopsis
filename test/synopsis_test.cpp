#include <gtest/gtest.h>

#include <synopsis.hpp>
#include <SqliteASDPDB.hpp>


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
    Synopsis::SqliteASDPDB db(":memory:");

    Synopsis::Application app(&db);
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


TEST(SynopsisTest, TestASDPDB) {

    Synopsis::Status status;
    std::vector<int> asdp_ids;

    Synopsis::DpDbMsg msg(
        -1, "test_instr", "test_type", "file:///data/file.dat",
        101, 0.12345, 7, Synopsis::DownlinkState::UNTRANSMITTED,
        {
            {"test_int", Synopsis::DpMetadataValue(123)},
            {"test_float", Synopsis::DpMetadataValue(123.456)},
            {"test_string", Synopsis::DpMetadataValue("test")}
        }
    );

    Synopsis::DpDbMsg msg2;

    Synopsis::SqliteASDPDB db(":memory:");

    status = db.init(0, NULL);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    status = db.insert_data_product(msg);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    // Check that a nonzero ID was assigned
    EXPECT_GT(msg.get_dp_id(), 0);

    asdp_ids = db.list_data_product_ids();
    EXPECT_EQ(asdp_ids.size(), 1);

    status = db.get_data_product(asdp_ids[0], msg2);
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

    // Check all fields equal to inserted value
    EXPECT_EQ(msg.get_dp_id(), msg2.get_dp_id());
    EXPECT_EQ(msg.get_instrument_name(), msg2.get_instrument_name());
    EXPECT_EQ(msg.get_type(), msg2.get_type());
    EXPECT_EQ(msg.get_uri(), msg2.get_uri());
    EXPECT_EQ(msg.get_dp_size(), msg2.get_dp_size());
    EXPECT_EQ(msg.get_science_utility_estimate(),
        msg2.get_science_utility_estimate());
    EXPECT_EQ(msg.get_priority_bin(), msg2.get_priority_bin());
    EXPECT_EQ(msg.get_downlink_state(), msg2.get_downlink_state());

    // Check equality of all metadata retrieved
    auto meta1 = msg.get_metadata();
    auto meta2 = msg.get_metadata();
    EXPECT_EQ(meta1.size(), meta2.size());
    for (auto const& pair : meta1) {
        std::string key = pair.first;
        Synopsis::DpMetadataValue value1 = pair.second;
        Synopsis::DpMetadataValue value2 = meta2[key];
        EXPECT_EQ(value1.get_type(), value2.get_type());
        EXPECT_EQ(value1.get_int_value(), value2.get_int_value());
        EXPECT_EQ(value1.get_float_value(), value2.get_float_value());
        EXPECT_EQ(value1.get_string_value(), value2.get_string_value());
    }

    status = db.get_data_product(-1, msg2);
    EXPECT_EQ(Synopsis::Status::FAILURE, status);

    status = db.deinit();
    EXPECT_EQ(Synopsis::Status::SUCCESS, status);

}
