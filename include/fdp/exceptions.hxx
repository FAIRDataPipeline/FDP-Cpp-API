#ifndef __FDP_EXCEPTIONS_HXX__
#define __FDP_EXCEPTIONS_HXX__

#include <stdexcept>
#include <string>

namespace FairDataPipeline {
class config_parsing_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class rest_apiquery_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class json_parse_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class validation_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class sync_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class write_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class toml_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

}; // namespace FairDataPipeline

#endif
