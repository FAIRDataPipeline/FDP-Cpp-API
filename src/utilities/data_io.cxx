#include "fdp/utilities/data_io.hxx"

namespace FairDataPipeline {

double read_point_estimate_from_toml(const ghc::filesystem::path var_address) {
  toml::value value = read_parameter_from_toml(var_address, "point-estimate");
  return (value.is_floating())
             ? value.as_floating()
             : static_cast<double>(
                   value.as_integer());
}

std::string read_distribution_from_toml(const ghc::filesystem::path var_address){
  return read_parameter_from_toml(var_address, "distribution").as_string();
}


toml::value read_parameter_from_toml(const ghc::filesystem::path var_address, const std::string &parameter) {
  if (!ghc::filesystem::exists(var_address)) {
    throw std::runtime_error("File '" + var_address.string() +
                             "' could not be opened as it does not exist");
  }

  const auto toml_data_ = toml::parse(var_address.string());

  const std::string first_key_ = get_first_key_(toml_data_);

  if (!toml::get<toml::table>(toml_data_).at(first_key_).contains("type")) {
    throw std::runtime_error("Expected 'type' tag but none found");
  }

  if (static_cast<std::string>(
          toml_data_.at(first_key_).at("type").as_string()) !=
      parameter) {
    throw std::runtime_error(
        "Expected "+ parameter +" for type but got '" +
        static_cast<std::string>(toml_data_.at("type").as_string()) + "'");
  }

  return toml_data_.at(first_key_).at("value");

}

ghc::filesystem::path
write_distribution(const std::string &value, const std::string &component,
                     const ghc::filesystem::path file_path) {
  return write_toml_parameter(value, "distribution",
                              component, file_path);
}

std::string get_first_key_(const toml::value data_table) {
  return data_table.as_table().begin()->first;
}
}; // namespace FairDataPipeline
