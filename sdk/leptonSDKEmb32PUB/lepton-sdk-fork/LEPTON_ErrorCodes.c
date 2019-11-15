#include "LEPTON_ErrorCodes.h"

const char *bad_arg_pointer_error     =  "BAD_ARG_POINTER_ERROR";
const char *checksum_error            =  "CHECKSUM_ERROR";
const char *comm_checksum_error       =  "COMM_CHECKSUM_ERROR";
const char *comm_close_error          =  "ERROR_CLOSING_COMM";
const char *comm_count_error          =  "COMM_COUNT_ERROR";
const char *comm_create_error         =  "ERROR_CREATING_COMM";
const char *comm_invalid_port         =  "COMM_INVALID_PORT_ERROR";
const char *comm_no_dev               =  "COMM_NO_DEV";
const char *comm_port_not_open        =  "COMM_PORT_NOT_OPEN";
const char *comm_range_error          =  "COMM_RANGE_ERROR";
const char *comm_read_error           =  "COMM_ERROR_READING_COMM";
const char *comm_start_error          =  "ERROR_STARTING_COMM";
const char *comm_write_error          =  "COMM_ERROR_WRITING_COMM";
const char *data_size_error           =  "DATA_SIZE_ERROR";
const char *div_zero                  =  "DIV_ZERO_ERROR";
const char *error                     =  "ERROR";
const char *function_not_supported    =  "FUNCTION_NOT_SUPPORTED";
const char *i2c_bus_arbitration_lost  =  "ERROR_I2C_ARBITRATION_LOST";
const char *i2c_bus_buffer_overflow   =  "ERROR_I2C_BUFFER_OVERFLOW";
const char *i2c_bus_error             =  "ERROR_I2C_BUS_ERROR";
const char *i2c_bus_nack_received     =  "ERROR_I2C_NACK_RECEIVED";
const char *i2c_bus_not_fail          =  "ERROR_I2C_FAIL";
const char *i2c_bus_not_ready         =  "ERROR_I2C_BUS_NOT_READY";
const char *not_an_error_code         =  "NOT A LEPTON ERROR CODE";
const char *not_ready                 =  "NOT_READY";
const char *ok                        =  "OK";
const char *operation_canceled        =  "OPERATION_CANCELED";
const char *otp_not_programmed        =  "OTP_NOT_PROGRAMMED_ERROR";
const char *otp_read_error            =  "OTP_READ_ERROR";
const char *otp_write_error           =  "OTP_WRITE_ERROR";
const char *range_error               =  "RANGE_ERROR";
const char *timeout_error             =  "TIMEOUT_ERROR";
const char *undefined_error           =  "UNDEFINED_ERROR_CODE";
const char *undefined_function_error  =  "UNDEFINED_FUNCTION_ERROR";

const char *error_as_string(LEP_RESULT errortype) {
    switch(errortype) {
    case LEP_OK:
        return ok;
    case LEP_ERROR:
        return error;
    case LEP_NOT_READY:
        return not_ready;
    case LEP_RANGE_ERROR:
        return range_error;
    case LEP_CHECKSUM_ERROR:
        return checksum_error;
    case LEP_BAD_ARG_POINTER_ERROR:
        return bad_arg_pointer_error;
    case LEP_DATA_SIZE_ERROR:
        return data_size_error;
    case LEP_UNDEFINED_FUNCTION_ERROR:
        return undefined_function_error;
    case LEP_FUNCTION_NOT_SUPPORTED:
        return function_not_supported;
    case LEP_OTP_WRITE_ERROR:
        return otp_write_error;
    case LEP_OTP_READ_ERROR:
        return otp_read_error;
    case LEP_OTP_NOT_PROGRAMMED_ERROR:
        return otp_not_programmed;
    case LEP_ERROR_I2C_BUS_NOT_READY:
        return i2c_bus_not_ready;
    case LEP_ERROR_I2C_BUFFER_OVERFLOW:
        return i2c_bus_buffer_overflow;
    case LEP_ERROR_I2C_ARBITRATION_LOST:
        return i2c_bus_arbitration_lost;
    case LEP_ERROR_I2C_BUS_ERROR:
        return i2c_bus_error;
    case LEP_ERROR_I2C_NACK_RECEIVED:
        return i2c_bus_nack_received;
    case LEP_ERROR_I2C_FAIL:
        return i2c_bus_not_fail;
    case LEP_DIV_ZERO_ERROR:
        return div_zero;
    case LEP_COMM_PORT_NOT_OPEN:
        return comm_port_not_open;
    case LEP_COMM_INVALID_PORT_ERROR:
        return comm_invalid_port;
    case LEP_COMM_RANGE_ERROR:
        return comm_range_error;
    case LEP_ERROR_CREATING_COMM:
        return comm_create_error;
    case LEP_ERROR_STARTING_COMM:
        return comm_start_error;
    case LEP_ERROR_CLOSING_COMM:
        return comm_close_error;
    case LEP_COMM_CHECKSUM_ERROR:
        return comm_checksum_error;
    case LEP_COMM_NO_DEV:
        return comm_no_dev;
    case LEP_TIMEOUT_ERROR:
        return timeout_error;
    case LEP_COMM_ERROR_WRITING_COMM:
        return comm_write_error;
    case LEP_COMM_ERROR_READING_COMM:
        return comm_read_error;
    case LEP_COMM_COUNT_ERROR:
        return comm_count_error;
    case LEP_OPERATION_CANCELED:
        return operation_canceled;
    case LEP_UNDEFINED_ERROR_CODE:
        return undefined_error;
    default:
        return not_an_error_code;

    }
}
