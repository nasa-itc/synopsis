#include <gtest/gtest.h>
#include <synopsis.h>


// Demonstrate some basic assertions.
TEST(SynopsisTest, TestHelloWorld) {

  // Expect equality.
  EXPECT_EQ(12345, synopsis_hello_world());


  // Test initialization
  Synopsis::Application app;
  Synopsis::Application::Status status;
  status = app.init();
  EXPECT_EQ(Synopsis::Application::Status::SUCCESS, status);
}

