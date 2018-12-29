///@file PaassExceptions.hpp
///@brief Class to handle errors occurring during the execution of the code
/// This file will break with the One-Class-Per-File rule. Breaking these into separate headers would generate a
/// significant amount of file clutter.
///@author S. V. Paulauskas and K. A. Miernik
///@date November 22, 2012

#ifndef __PAASS_EXCEPTIONS_HPP__
#define __PAASS_EXCEPTIONS_HPP__

#include <stdexcept>

///A generic PAASS exception.
class PaassException : public std::exception {
public:
    ///@brief Constructor that sets the value of what_
    ///@param[in] what : The string that we want to assign to what
    explicit PaassException (const std::string &what) noexcept : what_(what) {}

    virtual const char* what() const noexcept { return what_.c_str(); }
private:
    std::string what_;
};

///@brief An exception type that is used when we just need to send the user a message not actually kill the code. 
class PaassWarning : public PaassException {
public:
    ///@brief Constructor that sets the value of what
    ///@param[in] what : The string that we want to assign to what
    explicit PaassWarning (const std::string &what) noexcept : PaassException(what) {}
};

///@brief Read/write exception for all file operation related tasks. 
class IOException : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    IOException(const std::string &msg) : PaassException(msg) {}
};

///@brief Errors that occur during initialization of an object.
class InitializationException : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    InitializationException(const std::string &msg) : PaassException(msg) {}
};

///@brief Errors that occur when booting modules
class BootException : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    BootException(const std::string &msg) : PaassException(msg) {}
};

///@brief Errors that occur when trying to synchronize objects.
class SynchronizationException : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    SynchronizationException(const std::string &msg) : PaassException(msg) {}
};

///@brief Errors in the setup of the TreeCorrelator. Including, but not limited to creating new places, accessing places etc.
class TreeCorrelatorException : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    TreeCorrelatorException(const std::string &msg) : PaassException(msg) {}
};

///@brief Error related to converting between different variable types
class ConversionException : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    ConversionException(const std::string &msg) : PaassException(msg) {}
};

///@brief Errors related to access, creation, or closing of histograms
class HistogramException : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    HistogramException(const std::string &msg) : PaassException(msg) {}
};

///@brief Errors related to partially implemented features that may or may not work properly.
class NotImplemented : public PaassException {
public:
    /// Constructor taking the error message as the argument
    ///@param [in] msg : the error message to output
    NotImplemented(const std::string &msg) : PaassException(msg) {}
};

#endif
