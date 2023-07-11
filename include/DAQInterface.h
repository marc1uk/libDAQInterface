#ifndef DAQ_INTERFACE_H
#define DAQ_INTERFACE_H

#include <ServiceDiscovery.h>
#include <zmq.hpp>
#include <iostream>
#include <string>
#include <SlowControlCollection.h>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/progress.hpp>
#include <PGClient.h>
#include <functional>

///removing header to pass into store
//   config_json.replace(0,9,"");
//    config_json.replace(config_json.end()-2, config_json.end(),"");


class DAQInterface{

 private: 

  zmq::context_t m_context;
  ServiceDiscovery* mp_SD;
  PGClient m_pgclient;
  std::string m_dbname;
  std::string m_name;

 public:

  DAQInterface();
  ~DAQInterface();
  bool Init(std::string name, std::string pg_client_configfile, std::string db_name);
  bool SQLQuery(std::string dbname, std::string query_string, std::string &result, int &timeout, std::string& err);
  bool SendLog(std::string message, int severity=2, std::string source="");
  bool SendAlarm(std::string message, std::string type, std::string source="");
  bool SendMonitoringData(std::string json_data, std::string source="");
  bool SendConfig(std::string json_data, std::string author, std::string device="");
  bool GetConfig(std::string &json_data, int version, std::string device="");

  SlowControlCollection* GetSlowControlCollection();
  SlowControlElement* GetSlowControlVariable(std::string key);
  bool AddSlowControlVariable(std::string name, SlowControlElementType type, std::function<std::string(std::string)> function=nullptr);
  bool RemoveSlowControlVariable(std::string name);
  void ClearSlowControlVariables();

  bool TriggerSubscribe(std::string trigger, std::function<void(std::string)> function);
  bool TriggerSend(std::string trigger);
  std::string PrintSlowControlVariables();
  std::string GetDeviceName();

  template<typename T> T GetSlowControlValue(std::string name){
    return sc_vars[name]->GetValue<T>();
  }

  SlowControlCollection sc_vars;
  
};

#endif
