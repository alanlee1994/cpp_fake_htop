#include "processor.h"
#include "linux_parser.h"

#include <unistd.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> cpu_utilization = LinuxParser::CpuUtilization();
    std::vector<std::string> cpu_utilization_last = cpu_utilization;
    float idle = stof(cpu_utilization[LinuxParser::kIdle_]) + stof(cpu_utilization[LinuxParser::kIOwait_]);
    float previdle = stof(cpu_utilization_last[LinuxParser::kIdle_]) + stof(cpu_utilization_last[LinuxParser::kIOwait_]);

    float nonidle = stof(cpu_utilization[LinuxParser::kUser_]) + stof(cpu_utilization[LinuxParser::kNice_]) + stof(cpu_utilization[LinuxParser::kSystem_]) +
                         stof(cpu_utilization[LinuxParser::kIRQ_]) + stof(cpu_utilization[LinuxParser::kSoftIRQ_]) + stof(cpu_utilization[LinuxParser::kSteal_]);
    float prev_nonidle = stof(cpu_utilization_last[LinuxParser::kUser_]) + stof(cpu_utilization_last[LinuxParser::kNice_]) + stof(cpu_utilization_last[LinuxParser::kSystem_]) +
                              stof(cpu_utilization_last[LinuxParser::kIRQ_]) + stof(cpu_utilization_last[LinuxParser::kSoftIRQ_]) + stof(cpu_utilization_last[LinuxParser::kSteal_]);

    float total_cpu = idle + nonidle;
    float prev_total_cpu = previdle + prev_nonidle;
    float totald = total_cpu - prev_total_cpu;
    totald = totald < 0.0001f ? 0.0001f : totald;
    const float idled = idle - previdle;
    const float percent = (totald - idled) / totald;
    cpu_utilization_last = cpu_utilization;
    return percent;
}