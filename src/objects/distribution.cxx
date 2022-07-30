#include "fdp/objects/distribution.hxx"
#include "fdp/utilities/data_io.hxx"
#include "fdp/exceptions.hxx"

#include <vector>

namespace FairDataPipeline {
    Distribution::Distribution(std::string file_name){
        ghc::filesystem::path file_name_(file_name);
        _component = get_first_component(ghc::filesystem::path(file_name_));
        read_from_toml(file_name_, _component);
    }
    Distribution::Distribution(std::string file_name, std::string component){
        ghc::filesystem::path file_name_(file_name);
        this->_component = component;
        read_from_toml(file_name_, _component);
    }

    void Distribution::read_from_toml(ghc::filesystem::path file_path, std::string component){
        auto data_ = read_component_from_toml(file_path, component);
        if(!data_.contains("type")){
            throw toml_error("Error Toml file: " + file_path.string() + " does not contain a type tag" );
        }

        if(data_.at("type").as_string() != "distribution"){
            throw toml_error("Error component: " + component + " does not contain a distribution");
        }

        std::vector<std::string> required_keys_{"distribution", "mu", "sigma"};

        for(auto &key : required_keys_)
        {
            if(!data_.contains(key)){
                throw toml_error("Error component: " + component + " does not contain a " + key);
            }
        }

        _name = data_.at("distribution").as_string();
        

        if(data_.at("mu").is_floating()){
            _mu = data_.at("mu").as_floating();
        }
        else if(data_.is_integer()){
            _mu = static_cast<double>(data_.at("mu").as_integer());
        }
        else{
            throw toml_error("Error mu value is not a number");
        }

        if(data_.at("sigma").is_floating()){
            _sigma = data_.at("sigma").as_floating();
        }
        else if(data_.is_integer()){
            _sigma = static_cast<double>(data_.at("sigma").as_integer());
        }
        else{
            throw toml_error("Error sigma value is not a number");
        }


    }

    std::string Distribution::write_to_toml(std::string &file_name){
        return write_to_toml(_component, file_name);
    }

    std::string Distribution::write_to_toml(std::string &component, std::string &file_name){
        const toml::value data_{
      {component, {{"type", "distribution"},
       {"distribution", _name},
       {"mu", _mu},
       {"sigma", _sigma}}}};
    
    return write_toml_data(ghc::filesystem::path(file_name), component, data_).string();

    }

    bool Distribution::isEqual(const Distribution& dist) const{
        return this->_component == dist._component &&
        this->_mu == dist._mu &&
        this->_name == dist._name &&
        this->_sigma == dist._sigma;
    }

};