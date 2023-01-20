#include "ExampleSlowControl.h"

ExampleSlowControl::ExampleSlowControl():Tool(){}


bool ExampleSlowControl::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
  if(!m_variables.Get("Port",m_port)) m_port=5555;

  m_util=new DAQUtilities(m_data->context);
  
  sock = new  zmq::socket_t(*(m_data->context), ZMQ_DEALER);

  std::stringstream tmp;
  tmp<<"tcp://*:"<<m_port;

  sock->bind(tmp.str().c_str());

  if (!m_util->AddService("ExampleSlowControl",m_port,false)) return false;
  
  return true;
}


bool ExampleSlowControl::Execute(){

  return true;
}


bool ExampleSlowControl::Finalise(){

  bool ret=m_util->RemoveService("ExampleSlowControl");

  delete sock;
  sock=0;

  if(!ret) return false;

  return true;
}