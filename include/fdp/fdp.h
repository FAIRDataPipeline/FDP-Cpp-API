#ifndef __FDP_C_API__
#define __FDP_C_API__


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Enumeration used to denote different error types.
 *
 * The underlying C++ API will raise a number of different exception types. These map to
 * integer error codes for C compatibility.
 */
enum FDP_ERR_T {
    FDP_ERR_NONE = 0,
    FDP_ERR_CONFIG_PARSE = 1,
    FDP_ERR_REST_API_QUERY = 2,
    FDP_ERR_JSON_PARSE = 3,
    FDP_ERR_VALIDATION = 4,
    FDP_ERR_SYNC = 5,
    FDP_ERR_WRITE = 6,
    FDP_ERR_TOML = 6,
    FDP_ERR_OTHER = 7
};
typedef enum FDP_ERR_T FDP_ERR_T;


/**
 * @brief Initialise the pipeline.
 *
 * Should be called once before any calls to fdp_link_read or fdp_link_write. If called
 * more than once, returns FDP_ERR_OTHER.
 * 
 * @param config_file_path  Path to the `config.yaml` file for this FDP run. Should
 *                          be at the location `${FDP_CONFIG_DIR}/config.yaml`.
 * @param script_file_path  Path to the script which initiates this FDP run. Should
 *                          be at the location `${FDP_CONFIG_DIR}/script.sh` (or
 *                          `${FDP_CONFIG_DIR}/script.bat` on Windows).
 * @param token             Token used to connect to FDP registry. May be set to `NULL`.
 *
 * @return Error code.
 */
FDP_ERR_T fdp_init(
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
 * @return Error code.
 */
FDP_ERR_T fdp_finalise();


/**
 * @brief Set a path to a given data product while recording it's meta data for the
 *        code run.
 *
 * Must be called after fdp_init and before fdp_finalise.
 * 
 * @param data_product    Path to the input file.
 * @param data_store_path Path to the assigned data store location. The user should
 *                        allocate sufficient memory beforehand.
 * @return Error code
 */
FDP_ERR_T fdp_link_read(const char *data_product, char *data_store_path);


/**
 * @brief Set a path to a given data product while recording it's meta data for the
 *        code run.
 *
 * Must be called after fdp_init and before fdp_finalise.
 * 
 * @param data_product    Path to the output file.
 * @param data_store_path Path to the assigned data store location. The user should
 *                        allocate sufficient memory beforehand.
 * @return Error code
 */
FDP_ERR_T fdp_link_write(const char *data_product, char *data_store_path);


/**
 * @brief Enumeration used to denote the different levels of logging.
 *
 * Each level of logging includes all log levels greater than it, so setting the log
 * level to `DEBUG` will include all log types except `TRACE`. These correspond to
 * the C++ logging levels `FairDataPipeline::logging::LOG_LEVEL`.
 */
enum FDP_LOG_LEVEL {
    FDP_LOG_TRACE = 0,
    FDP_LOG_DEBUG = 1,
    FDP_LOG_INFO = 2,
    FDP_LOG_WARN = 3,
    FDP_LOG_ERROR = 4,
    FDP_LOG_CRITICAL = 5,
    FDP_LOG_OFF = 6
};
typedef enum FDP_LOG_LEVEL FDP_LOG_LEVEL;


/**
 * @brief Set the log level. Must call `fdp_init` first.
 *
 * @param log_level
 */
void fdp_set_log_level(FDP_LOG_LEVEL log_level);


/**
 * @brief Get the current log level. Must call fdp_init first.
 *
 * @return Log level
 */
FDP_LOG_LEVEL fdp_get_log_level();


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
int fdp_log(FDP_LOG_LEVEL log_level, const char* msg);


#ifdef __cplusplus
} // close extern "C"
#endif


#endif // __FDP_C_API__
