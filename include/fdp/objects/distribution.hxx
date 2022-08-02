#ifndef __FDP_DISTRIBUTION_HXX__
#define __FDP_DISTRIBUTION_HXX__

#include <string>
#include <ghc/filesystem.hpp>

namespace FairDataPipeline {
    /**
     * @brief Class for API objects
     * 
     */
    class Distribution {

        public:
            Distribution(const std::string &name, double mu, double sigma):
                _name(name),
                _mu(mu),
                _sigma(sigma),
                _component("0"){};
            Distribution(const std::string &name, double mu, double sigma, const std::string &component):
                _name(name),
                _mu(mu),
                _sigma(sigma),
                _component(component){};
            explicit Distribution(const std::string &file_name);
            Distribution(const std::string &file_name, const std::string &component);
            virtual ~Distribution() = default;
            std::string get_name() const {return _name;}
            double get_mu() const {return _mu;}
            double get_sigma() const {return _sigma;}
            std::string get_component() const {return _component;}

            std::string write_to_toml(std::string &file_name);
            std::string write_to_toml(std::string &component, std::string &file_name);

        private:
            std::string _name;
            double _mu;
            double _sigma;
            std::string _component;
        
        protected:
            void read_from_toml(const ghc::filesystem::path &file_path, const std::string &component);
            virtual bool isEqual(const Distribution& dist) const;
            friend bool operator==(const Distribution& lhs, const Distribution& rhs){
                return typeid(lhs) == typeid(rhs) && lhs.isEqual(rhs);
            }
    };

}; // namespace FairDataPipeline

#endif