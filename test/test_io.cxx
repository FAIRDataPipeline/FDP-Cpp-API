#ifndef TESTDIR
#define TESTDIR ""
#endif

#include "fdp/utilities/data_io.hxx"
#include "fdp/objects/distribution.hxx"
#include "fdp/exceptions.hxx"
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
  std::string component_2 = "Test_Component_2";
  double p_estimate = 52.95;
  double p_estimate_2 = 60.5;
  std::string distribution = "Weibull";
  double mu = 0.5;
  double sigma = 0.25;

  ghc::filesystem::path test_distribution_toml =
      ghc::filesystem::path(TESTDIR) / "data" / "temp" /
      "test_distribution.toml";
  ghc::filesystem::path test_distribution_3_toml =
      ghc::filesystem::path(TESTDIR) / "data" / "temp" /
      "test_distribution_3.toml";

  void TearDown() override {
    ghc::filesystem::remove_all(ghc::filesystem::path(TESTDIR) / "data" /
                                "temp");
  }
};

TEST_F(IOTest, TestEstimate) {
    ghc::filesystem::path p_estimate_toml = write_point_estimate<double>(p_estimate, component, ghc::filesystem::path(TESTDIR) / "data" / "temp" / "test_estimate.toml");

    double p_estimate_ = read_point_estimate_from_toml(p_estimate_toml);

    EXPECT_EQ(p_estimate, p_estimate_);

}

TEST_F(IOTest, TestDistribution) {
    Distribution distribution_1(distribution, mu, sigma, component);
    EXPECT_EQ(distribution_1.get_name(), distribution);
    EXPECT_DOUBLE_EQ(distribution_1.get_mu(), mu);
    EXPECT_DOUBLE_EQ(distribution_1.get_sigma(), sigma);
    EXPECT_EQ(distribution_1.get_component(), component);
}

TEST_F(IOTest, TestDistributionReadWrite) {
    Distribution distribution_1(distribution, mu, sigma, component);
    std::string file_name(test_distribution_toml.string());
    distribution_1.write_to_toml(file_name);
    Distribution distribution_2(file_name, component);

    EXPECT_EQ(distribution_1, distribution_2);
}

TEST_F(IOTest, TestDistributionReadWriteNoComponent) {
    Distribution distribution_3(distribution, mu, sigma);
    std::string file_name(test_distribution_3_toml.string());
    distribution_3.write_to_toml(file_name);
    Distribution distribution_4(file_name);

    EXPECT_EQ(distribution_3, distribution_4);
}

TEST_F(IOTest, TestTwoComponents) {
     ghc::filesystem::path two_p_estimates_toml = write_point_estimate<double>(p_estimate, component, ghc::filesystem::path(TESTDIR) / "data" / "temp" / "test_two_estimates.toml");

    write_point_estimate(p_estimate_2, component_2, two_p_estimates_toml);

    double p_estimate_ = read_point_estimate_from_toml(two_p_estimates_toml);
    double p_estimate_2_ = read_point_estimate_from_toml(two_p_estimates_toml, component_2);

    EXPECT_EQ(p_estimate, p_estimate_);
    EXPECT_EQ(p_estimate_2, p_estimate_2_);

}

TEST_F(IOTest, TestComponentExists) {
    try{
        ghc::filesystem::path both_toml = write_point_estimate(p_estimate, component, ghc::filesystem::path(TESTDIR) / "data" / "temp" / "test_both_fail.toml");

        write_point_estimate<double>(p_estimate, component, both_toml);
        FAIL();
    }
    catch(const toml_error& exception) {

    }
    catch(...){
        FAIL();
    }   

}
