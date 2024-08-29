#include <iostream>
#include <zmq.hpp>
#include <ServiceDiscovery.h>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace ToolFramework;

int main(){

  zmq::context_t context(1);

  ServiceDiscovery SD(false,true, 55555 , "239.192.1.1", 5000, &context, boost::uuids::random_generator()(), "Win_Mac_translation", 5, 60);


    boost::uuids::uuid m_UUID=boost::uuids::random_generator()();
    long msg_id=0;

    zmq::socket_t Ireceive (context, ZMQ_DEALER);
    Ireceive.connect("inproc://ServiceDiscovery");


    zmq::socket_t publish_sock (context, ZMQ_PUB);
    publish_sock.connect("tcp://127.0.0.1:666");

    boost::posix_time::time_duration period(0,0,1,0);
    boost::posix_time::ptime last=  boost::posix_time::microsec_clock::universal_time();


    ///////////////////////////// MM ///////////////////

    zmq::socket_t publish_sock_MM (context, ZMQ_PUB);
    publish_sock.connect("tcp://127.0.0.1:667");

    struct sockaddr_in addr;
    int addrlen, sock, cnt;
    struct ip_mreq mreq;
    char message[512];
    
    // set up socket //
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    int a =1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int));
    //fcntl(sock, F_SETFL, O_NONBLOCK); 
    if (sock < 0) {
      perror("socket");
      exit(1);
    }
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(5554U);
    addrlen = sizeof(addr);

 // receive //
  if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {        
    perror("bind");
    printf("Failed to bind to multicast listen socket");
    exit(1);
  }    
  mreq.imr_multiaddr.s_addr = inet_addr("239.192.1.1");         
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);         
  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq)) < 0) {
    perror("setsockopt mreq");
    printf("Failed to goin multicast group listen thread");
    exit(1);
  }         
    
  zmq::pollitem_t items [] = {
    { NULL, sock, ZMQ_POLLIN, 0 }
  };

    ///////////////////////////////////////////////////


  while(true){

    zmq::poll (&items [0], 1, 100);

    if ((items [0].revents & ZMQ_POLLIN)) {


      cnt = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) &addr, (socklen_t*) &addrlen);
      if ((cnt > 0) ) {
	

	zmq::message_t MM_message(sizeof(message));
	
	memcpy( MM_message.data(), message, sizeof(message));
	//	snprintf ((char *) MM_message.data(), sizeof(message) , "%d" , message) ;	
	publish_sock_MM.send(MM_message);

      }
    }
      
    boost::posix_time::time_duration lapse(period-(boost::posix_time::microsec_clock::universal_time() - last));


    if(lapse.is_negative()){

      last= boost::posix_time::microsec_clock::universal_time();
      
      zmq::message_t send(4);
      snprintf ((char *) send.data(), 4 , "%s" ,"All") ;
      
      Ireceive.send(send);
      
      zmq::message_t receive;
      Ireceive.recv(&receive);
      std::istringstream iss(static_cast<char*>(receive.data()));
      
      int size;
      iss>>size;
      
      for(int i=0;i<size;i++){
	
	Store *service = new Store;
	
	zmq::message_t servicem;
	Ireceive.recv(&servicem);
	
	publish_sock.send(servicem);
	
	//      std::istringstream ss(static_cast<char*>(servicem.data()));
	//std::cout<<ss.str()<<std::endl;
	
	
	/*      service->JsonParser(ss.str());
		
		std::string type;
		std::string uuid;
		std::string ip;
		std::string remote_port;
		service->Get("msg_value",type);
		service->Get("uuid",uuid);
		service->Get("ip",ip);
		if(port=="") service->Get("remote_port",remote_port);
		else remote_port=port;      
		std::string tmp=ip + ":" + remote_port;
		
		//if(type == ServiceName && connections.count(uuid)==0){
		if(type == ServiceName && connections.count(tmp)==0){
		connections[tmp]=service;
		//std::string ip;
		//std::string port;
		//service->Get("ip",ip);
		//service->Get("remote_port",port);
		tmp="tcp://"+ tmp;
		sock->connect(tmp.c_str());
		}
		else{
		delete service;
		service=0;
		}
		
		
		}
		
		return connections.size();
	*/  
      }	 
    }
					   
    
  }
    
  
  
  return 0;
  
}
