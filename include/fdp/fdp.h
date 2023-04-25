#ifndef __FDP_C_API__
#define __FDP_C_API__


#ifdef __cplusplus

#include "fdp.hxx"

extern "C" {

#endif

/**
 * @brief Struct providing an interface to the pipeline.
 *
 * Defined in the implementation file, as it depends on C++ features. A pointer to this
 * struct should be passed to functions in the C API. Set up by the function fdp_init,
 * and finalised by fdp_finalise. Can also be generated from a C++ DataPipeline
 * using to_c_struct.
 */
struct FdpDataPipeline;
typedef struct FdpDataPipeline FdpDataPipeline;

/**
 * @brief Enumeration used to denote different error types.
 *
 * The underlying C++ API will raise a number of different exception types. These map to
 * integer error codes for C compatibility.
 */
enum FdpError {
    FDP_ERR_NONE = 0,
    FDP_ERR_CONFIG_PARSE = 1,
    FDP_ERR_REST_API_QUERY = 2,
    FDP_ERR_JSON_PARSE = 3,
    FDP_ERR_VALIDATION = 4,
    FDP_ERR_SYNC = 5,
    FDP_ERR_WRITE = 6,
    FDP_ERR_TOML = 7,
    FDP_ERR_OTHER = 8
};
typedef enum FdpError FdpError;


/**
 * @brief Initialise the pipeline.
 *
 * Should be called once before any calls to fdp_link_read or fdp_link_write. If called
 * more than once, returns FDP_ERR_OTHER.
 * 
 * @param data_pipeline     Pointer-to-pointer of a FdpDataPipeline object. The user
 *                          should declare a pointer to a FdpDataPipeline, and pass its
 *                          address to this function. This function will then initialise
 *                          the pipeline.
 * @param config_file_path  Path to the `config.yaml` file for this FDP run. Should
 *                          be at the location `${FDP_CONFIG_DIR}/config.yaml`.
 * @param script_file_path  Path to the script which initiates this FDP run. Should
 *                          be at the location `${FDP_CONFIG_DIR}/script.sh` (or
 *                          `${FDP_CONFIG_DIR}/script.bat` on Windows).
 * @param token             Token used to connect to FDP registry. May be set to `NULL`.
 *
 * @return Error code.
 */
FdpError fdp_init(
    FdpDataPipeline **data_pipeline,
    const char *config_file_path,
    const char *script_file_path,
    const char *token
);


/**
 * @brief Finalise the pipeline.
 *
 * Must be called after a call to fdp_init.
 * 
 * Record all data products and meta data to the registry. Update the code run with all
 * appropriate meta data.
 *
 * @param data_pipeline Pointer-to-pointer of a FdpDataPipeline object. This function
 *                      finalises the FdpDataPipeline, and sets its pointer to NULL.
 * 
 * @return Error code.
 */
FdpError fdp_finalise(FdpDataPipeline **data_pipeline);


/**
 * @brief Set a path to a given data product while recording it's meta data for the
 *        code run.
 *
 * Must be called after fdp_init and before fdp_finalise.
 * 
 * @param data_pipeline   Pointer to a FdpDataPipeline object.
 * @param data_product    Path to the input file.
 * @param data_store_path Path to the assigned data store location. The user should
 *                        allocate sufficient memory beforehand.
 * @return Error code
 */
FdpError fdp_link_read(
    FdpDataPipeline *data_pipeline,
    const char *data_product,
    char *data_store_path
);


/**
 * @brief Set a path to a given data product while recording it's meta data for the
 *        code run.
 *
 * Must be called after fdp_init and before fdp_finalise.
 * 
 * @param data_pipeline   Pointer to a FdpDataPipeline object.
 * @param data_product    Path to the output file.
 * @param data_store_path Path to the assigned data store location. The user should
 *                        allocate sufficient memory beforehand.
 * @return Error code
 */
FdpError fdp_link_write(
    FdpDataPipeline *data_pipeline,
    const char *data_product,
    char *data_store_path
);


/**
 * @brief Enumeration used to denote the different levels of logging.
 *
 * Each level of logging includes all log levels greater than it, so setting the log
 * level to `DEBUG` will include all log types except `TRACE`. These correspond to
 * the C++ logging levels `FairDataPipeline::logging::LOG_LEVEL`.
 */
enum FdpLogLevel {
    FDP_LOG_TRACE = 0,
    FDP_LOG_DEBUG = 1,
    FDP_LOG_INFO = 2,
    FDP_LOG_WARN = 3,
    FDP_LOG_ERROR = 4,
    FDP_LOG_CRITICAL = 5,
    FDP_LOG_OFF = 6
};
typedef enum FdpLogLevel FdpLogLevel;


/**
 * @brief Set the log level. Must call `fdp_init` first.
 *
 * @param log_level
 */
void fdp_set_log_level(FdpLogLevel log_level);


/**
 * @brief Get the current log level. Must call fdp_init first.
 *
 * @return Log level
 */
FdpLogLevel fdp_get_log_level();


/**
 * @brief Write a message to the log. This will be passed to the C++ logger,
 *        `FairDataPipeline::logger::get_logger()->level() << msg`, where `level`
 *        is one of `trace`, `debug`, `info`, `warn`, `error`, or `critical`.
 *
 * @param log_level The type of log message to write, e.g. FDP_LOG_INFO, FDP_LOG_ERROR.
 * @param msg       The message to be written to log.
 *
 * @return Error code. 1 if logging unsuccessful, 0 otherwise.
 */
int fdp_log(FdpLogLevel log_level, const char *msg);


#ifdef __cplusplus

} // close extern "C"

namespace FairDataPipeline {

/**
 * @brief Convert data pipeline from the C API to one in the C++ API.
 */
DataPipeline::sptr from_c_struct(FdpDataPipeline *data_pipeline);

/**
 * @brief Convert data pipeline from the C++ API to one in the C API.
 *
 * If the pipeline is set up using the C++ method DataPipeline::construct, this may be
 * used to generate a C-compatible struct. Note that this uses 'new' to allocate the
 * returned pointer, so the user should 'delete_c_struct` after use to avoid memory
 * leaks. It is not recommended to mix usage of the C and C++ APIs for init and finalise
 * functions.
 */
FdpDataPipeline* to_c_struct(DataPipeline::sptr data_pipeline);

/**
 * @brief Function to clean up FdpDataPipeline created by to_c_struct
 */
void delete_c_struct(FdpDataPipeline *data_pipeline);

} // close namespace FairDataPipeline

#endif


#endif // __FDP_C_API__
