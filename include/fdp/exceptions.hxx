#ifndef __FDP_EXCEPTIONS_HXX__
#define __FDP_EXCEPTIONS_HXX__

#include <stdexcept>
#include <string>

namespace FairDataPipeline {
class config_parsing_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  explicit config_parsing_error(const std::string& message);
};

class rest_apiquery_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  explicit rest_apiquery_error(const std::string& message);
};

class json_parse_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  explicit json_parse_error(const std::string& message);
};

class validation_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  explicit validation_error(const std::string& message);
};

class sync_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  explicit sync_error(const std::string& message);
};

class write_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  explicit write_error(const std::string& message);
};

}; // namespace FairDataPipeline

#endif
