#ifndef TESTDIR
#define TESTDIR ""
#endif

#include "fdp/utilities/data_io.hxx"
#include "gtest/gtest.h"

using namespace FairDataPipeline;

class IOTest : public ::testing::Test {
protected:
  void SetUp() override {}
  std::string getHomeDirectory() {
    std::string HomeDirectory;
#ifdef _WIN32
    HomeDirectory = getenv("HOMEDRIVE");
    HomeDirectory += getenv("HOMEPATH");
#else
    HomeDirectory = getenv("HOME");
#endif
    return HomeDirectory;
  }
  ghc::filesystem::path read_csv_config =
      ghc::filesystem::path(TESTDIR) / "data" / "read_csv.yaml";
  std::string api_url_ = "http://127.0.0.1:8000/api";

  FairDataPipeline::API::sptr api_ = FairDataPipeline::API::construct(api_url_);

  std::string token = read_token(ghc::filesystem::path(getHomeDirectory()) /
                                 ".fair" / "registry" / "token");

  std::string component = "Test_Component";
  double p_estimate = 52.95;
  std::string distribution = "Weibull";

  void TearDown() override {}
};

TEST_F(IOTest, TestEstimate) {
    ghc::filesystem::path p_estimate_toml = write_point_estimate<double>(p_estimate, component, ghc::filesystem::path(TESTDIR) / "data" / "test_estimate.toml");

    double p_estimate_ = read_point_estimate_from_toml(p_estimate_toml);

    EXPECT_EQ(p_estimate, p_estimate_);

}

TEST_F(IOTest, TestDistribution) {
    ghc::filesystem::path distribution_toml = write_distribution(distribution, component, ghc::filesystem::path(TESTDIR) / "data" / "test_distribution.toml");

    std::string distribution_ = read_distribution_from_toml(distribution_toml);

    EXPECT_EQ(distribution, distribution_);

}

TEST_F(IOTest, TestBoth) {
    ghc::filesystem::path both_toml = write_distribution(distribution, component, ghc::filesystem::path(TESTDIR) / "data" / "test_both.toml");

    write_point_estimate<double>(p_estimate, component, both_toml);

    std::string distribution_ = read_distribution_from_toml(both_toml);
    double p_estimate_ = read_point_estimate_from_toml(both_toml);

    EXPECT_EQ(distribution, distribution_);
    EXPECT_EQ(p_estimate, p_estimate_);

}
