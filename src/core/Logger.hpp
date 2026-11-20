#pragma once

#include <iostream>
#include <string>
#include <ctime>


template<typename... Args>
static std::string BuildMessage(Args&&... args)
{
    std::ostringstream ss;
    (ss << ... << args);
    return ss.str();
}

class Logger
{
private:
    static std::ofstream logFile;
    static std::string GetTimestamp(){
        std::time_t now = std::time(nullptr);
        std::tm localTime = *std::localtime(&now);

        std::stringstream ss;
        ss << std::put_time(&localTime, "%H:%M:%S");
        
        return ss.str();
    }
    static void Log(const std::string& level,
                    const std::string& message,
                    std::ostream& output)
    {
        std::string logMessage =
            "[" + GetTimestamp() + "] [" + level + "] " + message;

        // Console output
        output << logMessage << std::endl;

        // File output
        if (logFile.is_open())
        {
            logFile << logMessage << std::endl;
            logFile.flush();
        }
    }
public:

    static void Init(const std::string& filename = "application.log")
    {
        logFile.open(filename, std::ios::out | std::ios::app);

        if (!logFile.is_open())
        {
            std::cerr << "Failed to open log file!" << std::endl;
        }
    }

    static void Shutdown()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    template<typename... Args>
    static void Info(Args&&... args)
    {
        Log("INFO", BuildMessage(std::forward<Args>(args)...), std::cout);
    }

    template<typename... Args>
    static void Warning(Args&&... args)
    {
        Log("WARNING", BuildMessage(std::forward<Args>(args)...), std::cout);
    }

    template<typename... Args>
    static void Error(Args&&... args)
    {
        Log("ERROR", BuildMessage(std::forward<Args>(args)...), std::cerr);
    }

    inline static bool debugEnable = true;
    template<typename... Args>
    static void Debug(Args&&... args)
    {
        if(debugEnable)
            Log("DEBUG", BuildMessage(std::forward<Args>(args)...), std::cout);
    }

};

// Static member definition
inline std::ofstream Logger::logFile;
