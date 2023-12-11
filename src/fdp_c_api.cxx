#include <algorithm>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <utility>

#include <ghc/filesystem.hpp>

#include "fdp/exceptions.hxx"
#include "fdp/fdp.h"
#include "fdp/fdp.hxx"
#include "fdp/objects/metadata.hxx"
#include "fdp/utilities/logging.hxx"

namespace FDP = FairDataPipeline;

// Define FdpDataPipeline struct and conversion routines

struct FdpDataPipeline {
  FDP::DataPipeline::sptr _pipeline;
};

FDP::DataPipeline::sptr FDP::from_c_struct(FdpDataPipeline *data_pipeline) {
  return data_pipeline->_pipeline;
}

FdpDataPipeline *FDP::to_c_struct(FDP::DataPipeline::sptr data_pipeline) {
  return new FdpDataPipeline{data_pipeline};
}

void FDP::delete_c_struct(FdpDataPipeline *data_pipeline) {
  delete data_pipeline;
}

/**
 * @brief Utility method, calls exception-raising function and returns error
 * codes
 *
 * Used to permit calls to C++ functions that may raise exceptions within a C
 * environment. If any exceptions are thrown, these are caught and converted to
 * an error code using the enum type FdpError.
 *
 * Many functions in the C++ API operate on a shared pointer to a
 * FairDataPipeline::DataPipeline object. To call a function on this shared
 * pointer using exception_to_err_code, use a lambda that captures the shared
 * pointer.
 *
 * @param function The C++ function to call. Must not have a void return type.
 *
 * @param ret Parameter in which to store the result of the function call.
 *
 * @param args Args to pass to the function.
 *
 * @return Error code
 *
 * @see exception_to_err_code_void
 */
template <typename Function, typename Return, typename... Args>
FdpError exception_to_err_code(Function &&function, Return &ret,
                               Args &&...args) {
  try {
    ret = std::forward<Function>(function)(std::forward<Args>(args)...);
    return FDP_ERR_NONE;
  } catch (const FDP::config_parsing_error &) {
    return FDP_ERR_CONFIG_PARSE;
  } catch (const FDP::rest_apiquery_error &) {
    return FDP_ERR_REST_API_QUERY;
  } catch (const FDP::json_parse_error &) {
    return FDP_ERR_JSON_PARSE;
  } catch (const FDP::validation_error &) {
    return FDP_ERR_VALIDATION;
  } catch (const FDP::sync_error &) {
    return FDP_ERR_SYNC;
  } catch (const FDP::write_error &) {
    return FDP_ERR_WRITE;
  } catch (const FDP::toml_error &) {
    return FDP_ERR_TOML;
  } catch (...) {
    return FDP_ERR_OTHER;
  }
}

/**
 * @brief Companion to exception_to_err_code for non-returning functions.
 */
template <typename Function, typename... Args>
FdpError exception_to_err_code_void(Function &&function, Args &&...args) {
  int dummy;
  return exception_to_err_code(
      [&function, &args...]() -> int {
        std::forward<Function>(function)(std::forward<Args>(args)...);
        return 0;
      },
      dummy);
}

/**
 * @brief Get error name from error code
 */
std::string error_name(FdpError err) {
  std::string result;
  switch (err) {
  FDP_ERR_NONE:
    result = "None";
    break;
  FDP_ERR_CONFIG_PARSE:
    result = "Config Parse";
    break;
  FDP_ERR_REST_API_QUERY:
    result = "REST API Query";
    break;
  FDP_ERR_JSON_PARSE:
    result = "JSON Parse";
    break;
  FDP_ERR_VALIDATION:
    result = "Validation";
    break;
  FDP_ERR_SYNC:
    result = "Sync";
    break;
  FDP_ERR_WRITE:
    result = "Write";
    break;
  FDP_ERR_TOML:
    result = "TOML";
    break;
  default:
    result = "Other";
  }
  return result;
}

// =================
// init and finalise
// =================

FdpError fdp_init(FdpDataPipeline **data_pipeline, const char *config_file_path,
                  const char *script_file_path, const char *token) {
  std::string token_str = (token == nullptr ? "" : token);
  FDP::DataPipeline::sptr cpp_data_pipeline;
  FdpError err = exception_to_err_code(
      FDP::DataPipeline::construct, cpp_data_pipeline,
      std::string(config_file_path), std::string(script_file_path), token_str);
  if (err) {
    // Trust that the C++ API logged the error before throwing
    *data_pipeline = nullptr;
    return err;
  }
  *data_pipeline = FDP::to_c_struct(cpp_data_pipeline);
  return err;
}

FdpError fdp_finalise(FdpDataPipeline **data_pipeline) {
  if (*data_pipeline == nullptr || (*data_pipeline)->_pipeline == nullptr) {
    FDP::logger::get_logger()->error()
        << "Pipeline not initialiased in call to fdp_finalise";
    return FDP_ERR_OTHER;
  }
  FdpError err = exception_to_err_code_void(
      [](FDP::DataPipeline::sptr pipeline) { pipeline->finalise(); },
      (*data_pipeline)->_pipeline);
  if (err) {
    // Trust that the C++ API logged the error before throwing
    return err;
  }
  FDP::delete_c_struct(*data_pipeline);
  *data_pipeline = nullptr;
  return err;
}

template <typename LinkFunction>
FdpError _fdp_link(LinkFunction &&link_function,
                   const std::string &link_function_name,
                   FdpDataPipeline *data_pipeline, const char *path,
                   char *output, size_t output_len) {
  // Ensure pipeline is initialised
  if (data_pipeline == nullptr || data_pipeline->_pipeline == nullptr) {
    FDP::logger::get_logger()->error()
        << " Data pipeline not initialised in call to " << link_function_name;
    return FDP_ERR_OTHER;
  }
  // Ensure input and output paths are valid
  if (path == nullptr) {
    FDP::logger::get_logger()->error()
        << "Input path is NULL in call to " << link_function_name;
    return FDP_ERR_OTHER;
  }
  if (output == nullptr) {
    FDP::logger::get_logger()->error()
        << "Output path is NULL in call to " << link_function_name;
    return FDP_ERR_OTHER;
  }
  // Check output len is valid
  if (output_len == 0) {
    FDP::logger::get_logger()->error()
        << "output_len is zero in call to " << link_function_name;
    return FDP_ERR_OTHER;
  }
  // Check input is null terminated, max 4096 chars, including terminator
  // TODO Should we check MAX_PATH/PATH_MAX here?
  bool path_null_terminated = false;
  std::size_t path_len = 0;
  for (std::size_t ii = 0; ii < 4096; ++ii) {
    if (path[ii] == '\0') {
      path_null_terminated = true;
      path_len = ii;
      break;
    }
  }
  if (!path_null_terminated) {
    FDP::logger::get_logger()->error()
        << "Input path is not null-terminated or is longer than 4095 chars in "
           "call to "
        << link_function_name;
    return FDP_ERR_OTHER;
  }
  // Check input and output don't overlap
  auto x1 = reinterpret_cast<std::uintptr_t>(path);
  auto x2 = x1 + path_len;
  auto y1 = reinterpret_cast<std::uintptr_t>(output);
  auto y2 = y1 + output_len;
  if (std::max(x1, y1) <= std::min(x2, y2)) {
    FDP::logger::get_logger()->error()
        << "Input and output paths overlap in call to " << link_function_name;
    return FDP_ERR_OTHER;
  }
  // Use C++ strings over C strings to interface with the C++ pipeline
  std::string input_path = path;
  std::string output_path;
  // Call either link_read or link_write on the pipeline, sets output_path
  FdpError err =
      exception_to_err_code(std::forward<LinkFunction>(link_function),
                            output_path, data_pipeline->_pipeline, input_path);
  if (err) {
    // Trust that the C++ API logged the error before throwing
    return err;
  }
  // Don't copy if output_path won't fit in output buffer
  // Use >= instead of > to account for null terminator
  if (output_path.size() >= output_len) {
    FDP::logger::get_logger()->error()
        << "Output path won't fit in buffer in call to " << link_function_name;
    return FDP_ERR_OTHER;
  }
  strncpy(output, output_path.c_str(), output_len);
  return FDP_ERR_NONE;
}

FdpError fdp_link_read(FdpDataPipeline *data_pipeline, const char *path,
                       char *output, size_t output_len) {
  return _fdp_link(
      [](FDP::DataPipeline::sptr pipeline, std::string &path) -> std::string {
        return pipeline->link_read(path);
      },
      "fdp_link_read", data_pipeline, path, output, output_len);
}

FdpError fdp_link_write(FdpDataPipeline *data_pipeline, const char *path,
                        char *output, size_t output_len) {
  return _fdp_link(
      [](FDP::DataPipeline::sptr pipeline, std::string &path) -> std::string {
        return pipeline->link_write(path);
      },
      "fdp_link_write", data_pipeline, path, output, output_len);
}

// =======
// logging
// =======

/**
 * @brief Map converting C API logging enums to the C++ API
 */
std::map<FdpLogLevel, FDP::logging::LOG_LEVEL> to_cpp_enum = {
    {FDP_LOG_TRACE, FDP::logging::TRACE},
    {FDP_LOG_DEBUG, FDP::logging::DEBUG},
    {FDP_LOG_INFO, FDP::logging::INFO},
    {FDP_LOG_WARN, FDP::logging::WARN},
    {FDP_LOG_ERROR, FDP::logging::ERROR},
    {FDP_LOG_CRITICAL, FDP::logging::CRITICAL},
    {FDP_LOG_OFF, FDP::logging::OFF}};

/**
 * @brief Map converting C++ API logging enums to the C API
 */
std::map<FDP::logging::LOG_LEVEL, FdpLogLevel> to_c_enum = {
    {FDP::logging::TRACE, FDP_LOG_TRACE},
    {FDP::logging::DEBUG, FDP_LOG_DEBUG},
    {FDP::logging::INFO, FDP_LOG_INFO},
    {FDP::logging::WARN, FDP_LOG_WARN},
    {FDP::logging::ERROR, FDP_LOG_ERROR},
    {FDP::logging::CRITICAL, FDP_LOG_CRITICAL},
    {FDP::logging::OFF, FDP_LOG_OFF}};

void fdp_set_log_level(FdpLogLevel log_level) {
  FDP::logger::get_logger()->set_level(to_cpp_enum[log_level]);
}

FdpLogLevel fdp_get_log_level() {
  return to_c_enum[FDP::logger::get_logger()->get_level()];
}

int fdp_log(FdpLogLevel log_level, const char *msg) {
  switch (log_level) {
  case FDP_LOG_TRACE:
    FDP::logger::get_logger()->trace() << msg;
    break;
  case FDP_LOG_DEBUG:
    FDP::logger::get_logger()->debug() << msg;
    break;
  case FDP_LOG_INFO:
    FDP::logger::get_logger()->info() << msg;
    break;
  case FDP_LOG_WARN:
    FDP::logger::get_logger()->warn() << msg;
    break;
  case FDP_LOG_ERROR:
    FDP::logger::get_logger()->error() << msg;
    break;
  case FDP_LOG_CRITICAL:
    FDP::logger::get_logger()->critical() << msg;
    break;
  default:
    return 1;
  }
  return 0;
}

// =========
// utilities
// =========

FdpError fdp_read_token(const char *path, char *token, size_t token_len) {
  std::string token_str;
  FdpError err = exception_to_err_code(FDP::read_token, token_str,
                                       ghc::filesystem::path(path));
  if (err) {
    FDP::logger::get_logger()->error() << "Failed to read token file";
    return err;
  }
  if (token_str.size() >= token_len) {
    FDP::logger::get_logger()->error()
        << "Token array is too small to store token";
    return FDP_ERR_OTHER;
  }
  strncpy(token, token_str.c_str(), token_len);
  return err;
}
