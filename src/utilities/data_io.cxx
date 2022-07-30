#include "fdp/utilities/data_io.hxx"

namespace FairDataPipeline {

double read_point_estimate_from_toml(const ghc::filesystem::path file_path, const std::string &component) {
  toml::value value = read_parameter_from_toml(file_path, "point-estimate", component);
  return (value.is_floating())
             ? value.as_floating()
             : static_cast<double>(
                   value.as_integer());
}

double read_point_estimate_from_toml(const ghc::filesystem::path file_path) {
  return read_point_estimate_from_toml(file_path, get_first_component(file_path));
}

std::string read_distribution_from_toml(const ghc::filesystem::path file_path, const std::string &component){
  return read_parameter_from_toml(file_path, "distribution", component).as_string();
}

std::string read_distribution_from_toml(const ghc::filesystem::path file_path){
  return read_parameter_from_toml(file_path, "distribution").as_string();
}

toml::value read_parameter_from_toml(const ghc::filesystem::path file_path, const std::string &parameter) {
  return read_parameter_from_toml(file_path, parameter, get_first_component(file_path));
}

toml::value read_data_from_toml(const ghc::filesystem::path file_path){
  if (!ghc::filesystem::exists(file_path)) {
      throw std::runtime_error("File '" + file_path.string() +
                              "' could not be opened as it does not exist");
    }

  auto toml_data_ = toml::parse(file_path.string());
  return toml_data_;

}

toml::value read_component_from_toml(const ghc::filesystem::path file_path, const std::string &component){
  toml::value component_ = read_data_from_toml(file_path.string()).at(component);
  return component_;
}


toml::value read_parameter_from_toml(const ghc::filesystem::path file_path, const std::string &parameter, const std::string &component) {
  
  toml::value toml_data_ = read_data_from_toml(file_path);

  if (!toml::get<toml::table>(toml_data_).at(component).contains("type")) {
    throw toml_error("Expected 'type' tag but none found");
  }

  if (static_cast<std::string>(
          toml_data_.at(component).at("type").as_string()) !=
      parameter) {
    throw toml_error(
        "Expected "+ parameter +" for type but got '" +
        static_cast<std::string>(toml_data_.at("type").as_string()) + "'");
  }

  return toml_data_.at(component).at("value");

}

ghc::filesystem::path write_toml_data(ghc::filesystem::path file_path, 
                                      const std::string &component,
                                      const toml::value &data){
  auto the_logger = logger::get_logger();

  if(component_exists(file_path, component)){
    throw toml_error("Component " + component + " already exists in " + file_path.string());
  }

  std::ofstream toml_out_;

  if (!ghc::filesystem::exists(file_path.parent_path())) {
    ghc::filesystem::create_directories(file_path.parent_path());
  }

  toml_out_.open(file_path.string(), std::ios_base::app);

  if (!toml_out_) {
    throw std::runtime_error("Failed to open TOML file for writing");
  }

  //toml_out_ << toml::format(data_);
  toml_out_ << data;

  toml_out_.close();

  the_logger->debug() 
      <<  "FileSystem:WriteToml: Wrote " << data << " to '" << file_path.string() << "'";

  return file_path;
}

std::string get_first_component(ghc::filesystem::path file_path){
  if (!ghc::filesystem::exists(file_path)) {
    throw toml_error("File '" + file_path.string() +
                             "' could not be opened as it does not exist");
  }
  const auto toml_data_ = toml::parse(file_path.string());

  return toml_data_.as_table().begin()->first;
}

bool component_exists(ghc::filesystem::path file_path, const std::string &component){
  if(!ghc::filesystem::exists(file_path))
  {
    return false;
  }
  const auto toml_data_ = toml::parse(file_path.string());
  if(!toml_data_.contains(component)){
    return false;
  }
  else{
    const auto& table = toml::find(toml_data_, component);
    return table.is_table();
  }
  
}
}; // namespace FairDataPipeline
