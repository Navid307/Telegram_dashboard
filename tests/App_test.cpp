#include "gtest/gtest.h"

#include "App.hpp"

extern "C" {
void hello_from_rust();
}

TEST(App, dummy) { EXPECT_EQ(1, 1); }

TEST(App, execute_command_echo) {
  App app;
  std::string res = app.invokeSystemCommand("echo hello");
  ASSERT_EQ(res, "hello\n");
}

TEST(App, command_handler) {
  App app;
  app.commandHandler("Rust");
  ASSERT_TRUE(1);
}

TEST(App, command_handler_all) {
  App app;
  app.startApp();
  ASSERT_TRUE(1);
}
