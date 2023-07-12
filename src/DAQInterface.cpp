#include <DAQInterface.h>

DAQInterface::DAQInterface(){

  sc_vars.InitThreadedReceiver(&m_context, 88888, 100, false);
  
  m_pgclient.SetUp(&m_context);
  
}
 
DAQInterface::~DAQInterface(){

  m_pgclient.Finalise();
  sc_vars.Clear();
  delete mp_SD;
  mp_SD=0;
  
}

bool DAQInterface::Init(std::string name, std::string pg_client_configfile, std::string db_name){
  
  m_name=name; 
  m_dbname=db_name;
  
  mp_SD = new ServiceDiscovery(true, false, 88888, "239.192.1.1", 5000, &m_context, boost::uuids::random_generator()(), name, 5, 60);
   
  if(!m_pgclient.Initialise(pg_client_configfile)){
    
    std::cout<<"error initialising pgclient"<<std::endl;
    return false;
  }
   
  // after Initilising the pgclient needs ~15 seconds for the middleman to connect
  std::this_thread::sleep_for(std::chrono::seconds(15));
  // hopefully the middleman has found us by now
  
  return true;
}


bool DAQInterface::SQLQuery(std::string dbname, std::string query_string, std::string &result, int &timeout, std::string& err){
 
  return m_pgclient.SendQuery(dbname, query_string, &result, &timeout, &err);
    
  // responses are returned as JSON maps, where each key represents a fieldname
  // and the corresponding value the field value. if multiple rows are returned,
  // this applies to each vector entry. Use a json parser to parse it into a BoostStore.
  //std::cout<<"PGHelper getting toolconfig by parsing json '"+result+"'"<<std::endl;
  //  BoostStore store;
  // parser.Parse(result, store);
  
  //  store.set stuff;

}


bool DAQInterface::SendLog(std::string message, int severity, std::string source){

  if(source=="") source=m_name;
  std::string err="";
  int timeout=300;
  std::string result;
  std::string query_string="insert into logging (time, source, severity, message) values (now(), '" + source + "', "+std::to_string(severity)+", '" + message + "');";

  if(!SQLQuery(m_dbname , query_string, result, timeout, err)){
    
    std::cout<<"log error: "<<err<<std::endl;
    
    return false;
  }
  

  return true;

}

bool DAQInterface::SendAlarm(std::string message, std::string type, std::string source){
  
  if(source=="") source=m_name;
  std::string err="";
  int timeout=300;
  std::string result;
  std::string query_string="insert into alarms (time, source, type, alarm) values (now(), '" + source + "', '" + type + "', '" + message + "');";
  
  if(!SQLQuery(m_dbname , query_string, result, timeout, err)){
    std::cerr<<"SendAlarm error: "<<err<<std::endl;
    return false;
  }
  
  return true;

}

bool DAQInterface::SendMonitoringData(std::string json_data, std::string source){

  if(source=="") source=m_name;
  std::string err="";
  int timeout=300;
  std::string result;
  std::string query_string="insert into monitoring (time, source, data) values (now(), '" + source + "', '" + json_data + "');";

  if(!SQLQuery(m_dbname , query_string, result, timeout, err)){
    std::cerr<<"SendMonitoringData error: "<<err<<std::endl;
    return false;
  }
  
  
  return true;

}


bool DAQInterface::SendConfig(std::string json_data, std::string author, std::string device){


  if(device=="") device=m_name;

  std::string result;
  int timeout=300;
  std::string err="";
  std::string query= "insert into device_config (time, device, version, author, data) values (now(), '"+ device + "', '0', '" + author + "', '" + json_data + "');";
  
if(!SQLQuery(m_dbname, query, result, timeout, err)){
    std::cerr<<"SendConfig error: "<<err<<std::endl;  
    return false;
  }

  return true;

}

bool DAQInterface::GetConfig(std::string& json_data, int version, std::string device){
  
  if(device=="") device=m_name;

  int timeout=300;
  std::string err="";
  std::string query= "select data from device_config where device='"+ device + "' and version=" + std::to_string(version) +";";
  
  if(!SQLQuery(m_dbname, query, json_data, timeout, err)){
    std::cerr<<"GetConfig error: "<<err<<std::endl;
    return false;
  }

  json_data.replace(0,9,"");
  json_data.replace(json_data.end()-2, json_data.end(),""); 

  return true;

}

SlowControlCollection* DAQInterface::GetSlowControlCollection(){

  return &sc_vars;

}

SlowControlElement* DAQInterface::GetSlowControlVariable(std::string key){

  return  sc_vars[key];
  
}

bool DAQInterface::AddSlowControlVariable(std::string name, SlowControlElementType type, std::function<std::string(std::string)> function){
  
return  sc_vars.Add(name, type, function);
  
}

bool DAQInterface::RemoveSlowControlVariable(std::string name){

  return sc_vars.Remove(name);  
  
}

void DAQInterface::ClearSlowControlVariables(){

  sc_vars.Clear();

}
bool DAQInterface::TriggerSubscribe(std::string trigger, std::function<void(std::string)> function){

return  sc_vars.TriggerSubscribe(trigger, function);

}
bool DAQInterface::TriggerSend(std::string trigger){

  return sc_vars.TriggerSend(trigger);

}
std::string DAQInterface::PrintSlowControlVariables(){

  return sc_vars.Print();

}

std::string DAQInterface::GetDeviceName(){

  return m_name;

}




