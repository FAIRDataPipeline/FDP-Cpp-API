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
            Distribution(std::string &name, double mu, double sigma):
                _name(name),
                _mu(mu),
                _sigma(sigma),
                _component("0"){};
            Distribution(std::string &name, double mu, double sigma, std::string &component):
                _name(name),
                _mu(mu),
                _sigma(sigma),
                _component(component){};
            Distribution(std::string file_name);
            Distribution(std::string file_name, std::string component);
            std::string get_name(){return _name;}
            double get_mu(){return _mu;}
            double get_sigma(){return _sigma;}
            std::string get_component(){return _component;}

            std::string write_to_toml(std::string &file_name);
            std::string write_to_toml(std::string &component, std::string &file_name);

        private:
            std::string _name;
            double _mu;
            double _sigma;
            std::string _component;
        
        protected:
            void read_from_toml(ghc::filesystem::path file_path, std::string component);
            virtual bool isEqual(const Distribution& dist) const;
            friend bool operator==(const Distribution& lhs, const Distribution& rhs){
                return typeid(lhs) == typeid(rhs) && lhs.isEqual(rhs);
            }
    };

}; // namespace FairDataPipeline

#endif