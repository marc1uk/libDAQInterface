#include <iostream>
#include <DAQInterface.h>
#include <functional>

using namespace ToolFramework;


int main(){
  
  int verbose=1;
  
  std::string Interface_configfile = "./InterfaceConfig";
  
  DAQInterface DAQ_inter(Interface_configfile);
  std::string device_name = DAQ_inter.GetDeviceName(); //name of my device
  
  /*
  std::cout<<"Testing logging..."<<std::flush;
  DAQ_inter.SendLog("severity 2 message", 2, device_name);   //sending log message to database specifing severity and device name
  DAQ_inter.SendLog("important message", 0, device_name);
  DAQ_inter.SendLog("unimportant message");   // if not specified, the default severity level is 9 and the default name is the one passed to the DAQInterface constructor
  if(verbose) std::cout<<"Logs sent"<<std::endl;
  
  std::cout<<"Sending test alarm..."<<std::flush;
  DAQ_inter.SendAlarm("High current on channel 3"); // sending alarm message to database
  if(verbose) std::cout<<"Test alarm sent"<<std::endl;
  */
  
  std::cout<<"Sending SQLQueries with one bad..."<<std::endl;
  std::string sss;
  DAQ_inter.SQLQuery("SELECT time, message FROM logging ORDER BY time DESC LIMIT 1",sss);   // success
  //DAQ_inter.SQLQuery("SELECT potato, message FROM logging ORDER BY time DESC LIMIT 1",sss); // fail
  DAQ_inter.SendAlarm("High current on channel 7");   // successful but will fail if in same batch as above
  std::cout<<"sent"<<std::endl;
  
  std::cout<<"Getting configuration settings from database"<<std::endl;
  int vvv=-1; //version of configuration to get; -1 to get the latest
  DAQ_inter.GetDeviceConfig(sss, vvv); //get configuration from database
  
  // TODO add testing GetDeviceRunConfig
  
  std::cout<<"Creating new configuration Store with default settings"<<std::endl;
  Store configuration;
  configuration.Set("power_on", bool(1));   // Store::Set can be passed any non-class variable type
  configuration.Set("voltage_1", 0);         // it can accept constant values...
  configuration.Set("voltage_2", 33); // ... or local variables ...
  std::string config_json;
  configuration>>config_json; //output current configuration to json
  DAQ_inter.SendDeviceConfig(config_json, "John Doe", "My New Config"); //uplaod configuration to database
  std::cout<<"sent"<<std::endl;
  
  // TODO add testing Get/Send CalibrationData
  
  // TODO add testing Get/Send Rootplot
  
  std::cout<<"sending PlotlyPlot"<<std::endl;
  std::vector<float> plot_x(5);
  for (size_t i = 0; i < plot_x.size(); ++i) plot_x[i] = i;
  std::vector<float> plot_y(plot_x.size());
  for (auto& y : plot_y) y = rand();
  std::string plot_layout = "{"
    "\"title\":\"A random plot\","
    "\"xaxis\":{\"title\":\"x\"},"
    "\"yaxis\":{\"title\":\"y\"}"
  "}";
  std::vector<std::string> traces(2);
  Store store;
  store.Set("x", plot_x);
  store.Set("y", plot_y);
  store >> traces[0];
  for (auto& y : plot_y) y = rand();
  store.Set("x", plot_x);
  store.Set("y", plot_y);
  store >> traces[1];
  DAQ_inter.SendPlotlyPlot("test_plot", traces, plot_layout);
  
  // TODO add test GetPlotlyPlot
  
  std::cout<<"Testing single-record generic SQL queries"<<std::endl;
  bool qryok = DAQ_inter.SQLQuery("SELECT time, message FROM logging ORDER BY time DESC LIMIT 1",sss);
  std::cout<<"success: "<<qryok<<", response: '"<<sss<<"'"<<std::endl;
  
  std::cout<<"Testing multi-record generic SQL queries"<<std::endl;
  std::vector<std::string> resps;
  qryok = DAQ_inter.SQLQuery("SELECT time, message FROM logging ORDER BY time DESC LIMIT 5",resps);
  for(int i=0; i<std::min(resps.size(),size_t(5)); ++i) std::cout<<i<<": '"<<resps.at(i)<<"'"<<std::endl;
  std::cout<<"success: "<<qryok<<", got "<<resps.size()<<" records:"<<std::endl;
  
  return 0;
  
}
