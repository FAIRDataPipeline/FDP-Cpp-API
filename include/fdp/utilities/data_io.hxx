/*! **************************************************************************
 * @file FairDataPipeline/registry/config.hxx
 * @author K. Zarebski (UKAEA)
 * @date 2021-05-06
 * @brief File containing classes for interacting with the local file system
 *
 * The classes within this file contain methods for interacting with the
 * local file system. These include the reading of configurations, and
 * the reading/writing of data to locally.
 ****************************************************************************/
#ifndef __FDP_DATA_IO_HXX__
#define __FDP_DATA_IO_HXX__

#include "toml.hpp"
#include <cstdlib>
#include <ghc/filesystem.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <ghc/filesystem.hpp>

#include <iostream>

#include "fdp/objects/config.hxx"
#include "fdp/objects/metadata.hxx"
#include "fdp/utilities/logging.hxx"
#include "fdp/utilities/semver.hxx"
#include "fdp/exceptions.hxx"

namespace FairDataPipeline {

/*! ***************************************************************************
 * @brief read the value of a point estimate from a given TOML file
 * @author K. Zarebski (UKAEA)
 *
 * @param var_address file path for the input TOML file
 * @return the extracted point estimate value
 *
 * @paragraph testcases Test Case
 *    `test/test_filesystem.cxx`: TestTOMLPERead
 *
 *    This unit test checks that a point estimate can be read from a TOML file
 *    successfully
 *    @snippet `test/test_filesystem.cxx TestTOMLPERead
 *
 *****************************************************************************/
double read_point_estimate_from_toml(const ghc::filesystem::path file_path);
double read_point_estimate_from_toml(const ghc::filesystem::path file_path, const std::string &component);
toml::value read_parameter_from_toml(const ghc::filesystem::path file_path,
                                     const std::string &parameter);
toml::value read_parameter_from_toml(const ghc::filesystem::path file_path,
                                     const std::string &parameter,
                                     const std::string &component);
toml::value read_data_from_toml(const ghc::filesystem::path file_path);
toml::value read_component_from_toml(const ghc::filesystem::path file_path, const std::string &component);

ghc::filesystem::path write_toml_data(ghc::filesystem::path file_path, 
                                      const std::string &component,
                                      const toml::value &data);

/**
 * @brief Create an estimate
 * 
 * @tparam T 
 * @param value 
 * @param file_path
 * @return ghc::filesystem::path 
 */
template <typename T>
ghc::filesystem::path write_toml_parameter(T &value,
                                      const std::string &parameter,
                                      const std::string &component,
                                      const ghc::filesystem::path file_path
                                      ) {

  const toml::value data_{
      {component, {{"type", parameter}, {"value", value}}}};
  return write_toml_data(file_path, component, data_);
  
}


template <typename T>
ghc::filesystem::path
write_point_estimate(T &value, const std::string &component,
                     const ghc::filesystem::path file_path) {
  return write_toml_parameter(value, "point-estimate",
                              component, file_path);
}

std::string get_first_component(const ghc::filesystem::path &file_path);

bool component_exists(const ghc::filesystem::path &file_path, const std::string &component);

}; // namespace FairDataPipeline

#endif
