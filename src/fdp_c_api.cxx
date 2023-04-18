#include <cstring>
#include <string>
#include <map>
#include <utility>

#include "fdp/fdp.h"
#include "fdp/fdp.hxx"
#include "fdp/exceptions.hxx"
#include "fdp/utilities/logging.hxx"


namespace FDP = FairDataPipeline;

/** 
 * @brief Utility method, calls exception-raising function and returns error codes
 *
 * Used to permit calls to C++ functions that may raise exceptions within a C
 * environment. If any exceptions are thrown, these are caught and converted to an error
 * code using the enum type FDP_ERR_T.
 *
 * Many functions in the C++ API operate on a shared pointer to a
 * FairDataPipeline::DataPipeline object. To call a function on this shared pointer
 * using exception_to_err_code, use a lambda that captures the shared pointer.
 *
 * @param function The C++ function to call. Must not have a void return type.
 * @param ret      Parameter in which to store the result of the function call.
 * @param args     Args to pass to the function.
 * 
 * @return Error code
 * 
 * @see exception_to_err_code_void
 */
template<typename Function, typename Return, typename... Args>
FDP_ERR_T exception_to_err_code(Function&& function, Return& ret, Args&&... args){
    try{
        ret = std::forward<Function>(function)(std::forward<Args>(args)...);
        return FDP_ERR_NONE;
    }
    catch(const FDP::config_parsing_error&){
        return FDP_ERR_CONFIG_PARSE;
    }
    catch(const FDP::rest_apiquery_error&){
        return FDP_ERR_REST_API_QUERY;
    }
    catch(const FDP::json_parse_error&){
        return FDP_ERR_JSON_PARSE;
    }
    catch(const FDP::validation_error&){
        return FDP_ERR_VALIDATION;
    }
    catch(const FDP::sync_error&){
        return FDP_ERR_SYNC;
    }
    catch(const FDP::write_error&){
        return FDP_ERR_WRITE;
    }
    catch(const FDP::toml_error&){
        return FDP_ERR_TOML;
    }
    catch(...){
        return FDP_ERR_OTHER;
    }
}

/** 
 * @brief Companion to exception_to_err_code for non-returning functions.
 */
template<typename Function, typename... Args>
FDP_ERR_T exception_to_err_code_void(Function&& function, Args&&... args){
    int dummy;
    return exception_to_err_code(
        [&function](Args&&... args) -> int {
            std::forward<Function>(function)(std::forward<Args>(args)...);
            return 0;
        },
        dummy,
        std::forward<Args>(args)...
    );
}


// =================
// init and finalise
// =================


/**
 * @brief Global instance of FairDataPipeline::DataPipeline
 *
 * This should be initialised by fdp_init and deleted by fdp_finalise. The functions
 * fdp_link_read and fdp_link_write make use of this, so will have undefined behaviour
 * if called before init and after finalise.
 */
FDP::DataPipeline::sptr _datapipeline;


FDP_ERR_T fdp_init(
    const char *config_file_path,
    const char *script_file_path,
    const char *token
){
    std::string token_str = (token == nullptr ? "" : token);

    if(_datapipeline == nullptr){
        return exception_to_err_code(
            FDP::DataPipeline::construct,
            _datapipeline,
            std::string(config_file_path),
            std::string(script_file_path),
            token_str
        );
    }
    return FDP_ERR_OTHER;
}


FDP_ERR_T fdp_finalise(){
    if(_datapipeline == nullptr) return FDP_ERR_OTHER;
    return exception_to_err_code_void(
        [=](){
            _datapipeline->finalise();
        }
    );
}


template<typename LinkFunction>
FDP_ERR_T _fdp_link(LinkFunction&& link_function, const char* path, char* output){
    if(_datapipeline == nullptr) return FDP_ERR_OTHER;
    std::string input_path = path;
    std::string output_path;
    FDP_ERR_T err = exception_to_err_code(
        std::forward<LinkFunction>(link_function),
        output_path,
        input_path
    );
    if(err){
        return err;
    } 
    strcpy(output, output_path.c_str());
    return FDP_ERR_NONE;
}


FDP_ERR_T fdp_link_read(const char* path, char* output){
    return _fdp_link(
        [=](std::string& path) -> std::string {
            return _datapipeline->link_read(path);
        },
        path,
        output
    );
}


FDP_ERR_T fdp_link_write(const char* path, char* output){
    return _fdp_link(
        [=](std::string& path) -> std::string {
            return _datapipeline->link_write(path);
        },
        path,
        output
    );
}

// =======
// logging
// =======


/**
 * @brief Map converting C API logging enums to the C++ API
 */
std::map<FDP_LOG_LEVEL, FDP::logging::LOG_LEVEL> to_cpp_enum = {
    {FDP_LOG_TRACE, FDP::logging::TRACE},
    {FDP_LOG_DEBUG, FDP::logging::DEBUG},
    {FDP_LOG_INFO, FDP::logging::INFO},
    {FDP_LOG_WARN, FDP::logging::WARN},
    {FDP_LOG_ERROR, FDP::logging::ERROR},
    {FDP_LOG_CRITICAL, FDP::logging::CRITICAL},
    {FDP_LOG_OFF, FDP::logging::OFF}
};


/**
 * @brief Map converting C++ API logging enums to the C API
 */
std::map<FDP::logging::LOG_LEVEL, FDP_LOG_LEVEL> to_c_enum = {
    {FDP::logging::TRACE, FDP_LOG_TRACE},
    {FDP::logging::DEBUG, FDP_LOG_DEBUG},
    {FDP::logging::INFO, FDP_LOG_INFO},
    {FDP::logging::WARN, FDP_LOG_WARN},
    {FDP::logging::ERROR, FDP_LOG_ERROR},
    {FDP::logging::CRITICAL, FDP_LOG_CRITICAL},
    {FDP::logging::OFF, FDP_LOG_OFF}
};


void fdp_set_log_level(FDP_LOG_LEVEL log_level){
    FDP::logger::get_logger()->set_level(to_cpp_enum[log_level]);
}


FDP_LOG_LEVEL fdp_get_log_level(){
    return to_c_enum[FDP::logger::get_logger()->get_level()];
}


int fdp_log(FDP_LOG_LEVEL log_level, const char* msg){
    switch(log_level){
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
