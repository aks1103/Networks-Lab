#include<iostream>

#include <ns3/applications-module.h>


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("FifthScriptExample");

ofstream f1("congestionReport.txt" , ofstream::out)
ofstream f2("droppedReport.txt" , ofstream::out)
ofstream f3("transferedReport.txt" , ofstream::out)


//---------------------------Class MyApp Starts-------------

class MyApp : public Application
  {
  public:
  
    MyApp ();
    virtual ~MyApp();
  
    void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, 
      uint32_t nPackets, DataRate dataRate);
  
  private:
    virtual void StartApplication (void);
    virtual void StopApplication (void);
  
    void ScheduleTx (void);
    void SendPacket (void);
  
    Ptr<Socket>     m_socket;
    Address         m_peer;
    uint32_t        m_packetSize;
    uint32_t        m_nPackets;
    DataRate        m_dataRate;
    EventId         m_sendEvent;
    bool            m_running;
    uint32_t        m_packetsSent;
  };


MyApp::MyApp ()
: m_socket (0),
  m_peer (),
  m_packetSize (0),
  m_nPackets (0),
  m_dataRate (0),
  m_sendEvent (),
  m_running (false),
  m_packetsSent (0)
{
}

MyApp::~MyApp()
{
m_socket = 0;
}

void
MyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate){
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
  m_dataRate = dataRate;
}

void
MyApp::StartApplication (void)
{
	m_running = true;
	m_packetsSent = 0;
	m_socket->Bind ();
	m_socket->Connect (m_peer);
	SendPacket ();
}


void
MyApp::StopApplication (void)
{
	m_running = false;

	if (m_sendEvent.IsRunning ())
	  {
	    Simulator::Cancel (m_sendEvent);
	  }

	if (m_socket)
	  {
	    m_socket->Close ();
	  }
}


void
MyApp::SendPacket (void)
{
	Ptr<Packet> packet = Create<Packet> (m_packetSize);
	m_socket->Send (packet);

	if (++m_packetsSent < m_nPackets)
	  {
	    ScheduleTx ();
	  }
}


void
MyApp::ScheduleTx (void)
{
	if (m_running)
	  {
	    Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
	    m_sendEvent = Simulator::Schedule (tNext, &MyApp::SendPacket, this);
	  }
}
//---------------------------Class MyApp Ends---------------





// to log the change in congestion window
// get trace callbacks from TCP indicating the congestion window has been updated. The next piece of code implements the corresponding trace sink: 
static void
CwndChange (uint32_t oldCwnd, uint32_t newCwnd)
{
	NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
}


// 
static void
TraceCwnd (Ptr<OutputStreamWrapper> stream)
{
    Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeBoundCallback (&CwndChange,stream));
}

 // new trace sink to show where packets are dropped. We are going to add an error model to this code also, so we wanted to demonstrate this working. 
static void
RxDrop (Ptr<const Packet> p)
{
	NS_LOG_UNCOND ("RxDrop at " << Simulator::Now ().GetSeconds ());
}


int main(int argc, char const *argv[])
{
	cout<<"Assignment 4 : Networks Lab";


	// properties of consitituents as per the question
	int cbr_agents = 5;
	char cbr_speed[] = "300Kbps"
	float starttime[cbr_agents] = {0.2 , 0.4, 0.6, 0.8, 1.0};
	float endtime[cbr_agents] = {-1, -1, 1.2, 1.4, 1.6 };
	int simulation_time = 1.8; // simulation time 1800 ms

	char link_bandwidth[] = "1Mbps";
	char link_delay[] = "10ms";
	char queue_type = "ns3::DropTailQueue";

	// setting the default protocol type
	Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpNewReno::GetTypeId ()));

	// Node container
	// keep track of a set of node pointers.
	// Typically ns-3 helpers operate on more than 
	// one node at a time. For example a device helper
	// may want to install devices on a large number of 
	// similar nodes. The helper Install methods usually 
	// take a NodeContainer as a parameter. NodeContainers hold the multiple 
	// Ptr<Node> which are used to refer to the nodes. 
	NodeContainer nodes;
	nodes.Create(2);
	NS_LOG_INFO ("2 nodes created");



	// PointToPointHelper
	//Build a set of PointToPointNetDevice objects. 
	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue (link_bandwidth));
	p2p.SetChannelAttribute("Delay", StringValue (link_delay));
	p2p.SetQueue();

	NS_LOG_INFO ("Point2point channel created with following properties");
	NS_LOG_INFO ("DataRate : 1Mbps");
	NS_LOG_INFO ("Delay : 10ms");
	NS_LOG_INFO ("Queue Type : DropTailQueue");

	// olds a vector of ns3::NetDevice pointers
	// Typically ns-3 NetDevices are installed on nodes using a net device helper. The helper Install method takes a NodeContainer which holds some number of Ptr<Node>. For each of the Nodes in the NodeContainer the helper will instantiate a net device, add a MAC address and a queue to the device and install it to the node. For each of the devices, the helper also adds the device into a Container for later use by the caller. This is that container used to hold the Ptr<NetDevice> which are instantiated by the device helper. 

	NetDeviceContainer devices; 
	devices = p2p.Install(nodes)
	NS_LOG_INFO ("Instantiating a net device, add a MAC address and a queue to the device and install it to the nodes.");



	// doubt
	Ptr<RateErrorModel> em = CreateObjectWithAttributes<RateErrorModel> (
    "RanVar", RandomVariableValue (UniformVariable (0., 1.)),
    "ErrorRate", DoubleValue (0.00001));
  devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
	NS_LOG_INFO ("Error model instantiated");
	




  	// InternetStackHelper
	// aggregate IP/TCP/UDP functionality to existing Nodes.
	// This helper enables pcap and ascii tracing of events in the internet stack associated with a node
	InternetStackHelper stack;
	stack.Install(nodes)
	NS_LOG_INFO ("Internet Stacks installed on the nodes");




	Ipv4AddressHelper address;
  	address.SetBase ("10.1.1.0", "255.255.255.252");
  	Ipv4InterfaceContainer interfaces = address.Assign (devices);
	NS_LOG_INFO ("Assigning IP address to the nodes");


  	//----------------CREATE APPLICATIONs-------------------

  	//----------------CREATE FTP AGENT-------------------


	uint16_t sinkPort = 8080;
	Address sinkAddress (InetSocketAddress(interfaces.GetAddress (1), sinkPort));
	PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", 
	InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
	ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (1));
	sinkApps.Start (Seconds (0.));
	sinkApps.Stop (Seconds(simulation_time));



	// bulksend 
	BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress (ipv4Container.GetAddress (0), port));
    source.SetAttribute ("MaxBytes",0);
    ApplicationContainer sourceApps = source.Install (nodes.Get (0));
    sourceApps.Start (Seconds (0.0));
	sourceApps.Stop (Seconds (simulation_time));



uint16_t cbr_sink_port = 8000;

for(int i=0;i<5;i++, cbr_sink_port++){


		// creating a sink app at node 1 --- destination node

		Address cbr_sink_address (InetSocketAddress (interfaces.GetAddress (1), cbr_sink_port));
		PacketSinkHelper packetSinkHelper2 ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), cbr_sink_port));
		ApplicationContainer cbr_sink_app = packetSinkHelper2.Install (nodes.Get (1)); 
		
		cbr_sink_app.Start(Seconds(starttime[i]));
		if(endtime[i]==-1) endtime[i]=simulation_time;
		cbr_sink_app.Stop (Seconds(endtime[i]));
	
	
		// creating a source app at node 0 --- source node
		// udp is used for cbr agent as mentioned in official tutorial
		Ptr<Socket> ns3UdpSocket = Socket::CreateSocket (nodes.Get (0), UdpSocketFactory::GetTypeId ()); 
	
		Ptr<MyApp> cbr_agent = CreateObject<MyApp> ();
		cbr_agent->Setup (ns3UdpSocket, cbr_sink_address, 1040, 100000, DataRate ("300Kbps"));
		nodes.Get(0)->AddApplication (cbr_agent);
		cbr_agent->SetStartTime(Seconds(starttime[i]));
		cbr_agent->SetStopTime(Seconds(endtime[i]));


	}




  	// start the simulator

  	// set the simulation time
  	Simulator::Stop (Seconds(simulation_time));
  	// run to start the simulator 
  	// once the simulator is started it can be configured . Time for trace collection
    Simulator::Run ();

    // end the simulator.Once done
    Simulator::Destroy ();




    // close and free all resources
    f1.close();
    f2.close();
    f3.close();

	return 0;
}