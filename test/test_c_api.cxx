#ifndef TESTDIR
#define TESTDIR ""
#endif

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include "fdp/fdp.h"
#include "fdp/objects/metadata.hxx" // read_token

#include <ghc/filesystem.hpp>
#include "gtest/gtest.h"

namespace fs = ghc::filesystem;
namespace fdp = FairDataPipeline;


std::string home_dir(){
  std::string home;
#ifdef _WIN32
  home = getenv("HOMEDRIVE");
  home += getenv("HOMEPATH");
#else
  home = getenv("HOME");
#endif
  return home;
}


TEST(CTest, link_read_write){
  fdp_set_log_level(FDP_LOG_DEBUG);

  // Initialise
  fs::path config = fs::path(TESTDIR) / "data" / "write_csv.yaml";
  fs::path script = fs::path(TESTDIR) / "test_script.sh";
  std::string token = fdp::read_token(
    fs::path(home_dir()) / ".fair" / "registry" / "token"
  );
  ASSERT_EQ(fdp_init(config.c_str(), script.c_str(), token.c_str()), FDP_ERR_NONE);
  char buf[512];

  // Test link write
  buf[0] = '\0'; // Ensure strlen of output buffer is 0
  EXPECT_EQ(fdp_link_write("test/csv", buf), FDP_ERR_NONE);
  EXPECT_GT(strlen(buf), 1);

  // Write to new path
  std::ofstream fstream(buf);
  fstream << "Test";
  fstream.close();

  // Finalise and re-initialise
  ASSERT_EQ(fdp_finalise(), FDP_ERR_NONE);
  config = fs::path(TESTDIR) / "data" / "read_csv.yaml";
  ASSERT_EQ(fdp_init(config.c_str(), script.c_str(), token.c_str()), FDP_ERR_NONE);

  // Test link read
  buf[0] = '\0'; // Ensure strlen of output buffer is 0
  EXPECT_EQ(fdp_link_read("test/csv", buf), FDP_ERR_NONE);
  EXPECT_GT(strlen(buf), 1);

  // Finalise again
  EXPECT_EQ(fdp_finalise(), FDP_ERR_NONE);
}


TEST(CTest, log_levels){
    fdp_set_log_level(FDP_LOG_INFO);
    EXPECT_EQ(fdp_get_log_level(), FDP_LOG_INFO);
    fdp_set_log_level(FDP_LOG_DEBUG);
    EXPECT_EQ(fdp_get_log_level(), FDP_LOG_DEBUG);
}
