#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hour;
  int minute;
  int second;
  string time;

  hour = seconds/3600;
  minute = seconds/60;
  second = seconds%60;

  if (hour < 10) 
  {
    time = "0";
  }
  time += to_string(hour) + ":";
  
  if (minute < 10)
  {
    time += "0";
  }
  time += to_string(minute) + ":";
  
  if (second < 10){
    time += "0";
  }
  time += to_string(second);
  
  return time; 
 }

