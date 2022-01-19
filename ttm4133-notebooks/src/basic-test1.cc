#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor-module.h"
#include <iomanip>
#include <string>
// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");
double simTime = 10000; // milliseconds
double udpAppStartTime = 400; //milliseconds
Time binSize = Seconds (0.1);


void
flowstats(Ptr<FlowMonitor> monitor, FlowMonitorHelper *flowmon)
{
  // Print per-flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon->GetClassifier ());
//  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();


  double averageFlowThroughput = 0.0;
  double averageFlowDelay = 0.0;


  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::stringstream protoStream;
      std::string channel;
      protoStream << (uint16_t) t.protocol;
      if (t.protocol == 6)
        {
          protoStream.str ("TCP");
        }
      if (t.protocol == 17)
        {
          protoStream.str ("UDP");
        }
      if(t.sourceAddress == "10.1.1.1")
        {
          channel = "PC";
        }
      else
        {
          channel = "CP";
        }

      if (i->second.rxPackets > 0)
        {
          // Measure the duration of the flow from receiver's perspective
          double rxDuration = i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ();
//          double rxDuration = (simTime - udpAppStartTime);


          averageFlowThroughput += (i->second.rxBytes * 8.0) / rxDuration / 1024 / 1024;
          averageFlowDelay += 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets;



    	  std::cout << std::fixed << std::setw (10) << Simulator::Now().GetSeconds();
    	  std::cout << std::setw(7) << channel;
    	  std::cout << std::setw(20) << (i->second.rxBytes * 8.0) / rxDuration / 1024 / 1024;
    	  std::cout << std::setw(20) << 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets << std::endl;

        }
      else
        {
    	  uint16_t temp = 0;
    	  std::cout << std::fixed << std::setw (10) << Simulator::Now().GetSeconds();
    	  std::cout << std::setw(7) << channel;
    	  std::cout << std::setw(20) << temp;
    	  std::cout << std::setw(20) << temp << std::endl;
        }
    }
  Simulator::Schedule (binSize, &flowstats, monitor, flowmon);

}

int
main (int argc, char *argv[])
{
  bool verbose = false;
  uint32_t nCsma = 3;
  uint32_t lambda = 20000;

  uint16_t udppksize = 3096, p2pdelay = 5, csmadelay = 30;
  uint32_t p2prate = 10000, csmarate = 5000;
  bool enableflowstats = true;

//  Ptr<UniformRandomVariable> p2pdelay = CreateObject<UniformRandomVariable> ();
//  p2pdelay->SetAttribute ("Min", DoubleValue (1));
//  p2pdelay->SetAttribute ("Max", DoubleValue (5));
//  Ptr<UniformRandomVariable> csmadelay = CreateObject<UniformRandomVariable> ();
//  csmadelay->SetAttribute ("Min", DoubleValue (1));
//  csmadelay->SetAttribute ("Max", DoubleValue (10));
//  Ptr<UniformRandomVariable> udppksize = CreateObject<UniformRandomVariable> ();
//  udppksize->SetAttribute ("Min", DoubleValue (1024));
//  udppksize->SetAttribute ("Max", DoubleValue (2048));

  CommandLine cmd (__FILE__);
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("lambda", "packet generation rate", lambda);
  cmd.AddValue ("udppksize", "udp packet size", udppksize);
  cmd.AddValue ("p2pdelay", "p2p link delay", p2pdelay);
  cmd.AddValue ("csmadelay", "csma delay", csmadelay);
  cmd.AddValue ("p2prate", "p2p line rate", p2prate);
  cmd.AddValue ("csmarate", "csma rate", csmarate);

  cmd.Parse (argc,argv);


  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma);

  std::string rate_p = std::to_string(p2prate) + "Mbps";
  std::string rate_c = std::to_string(csmarate) + "Mbps";
//   std::cout<< rate_p << " " << rate_c << std::endl;


  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue(rate_p));
  pointToPoint.SetChannelAttribute ("Delay", TimeValue (MilliSeconds(p2pdelay)));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);


  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue (rate_c));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds(csmadelay)));


//  std::cout << p2pdelay->GetInteger() << " " << csmadelay->GetInteger() << " " << udppksize->GetInteger() << std::endl;
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (p2pNodes.Get (0));
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (MilliSeconds (udpAppStartTime));
  serverApps.Stop (MilliSeconds (simTime));

  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100000));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0 / lambda)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (udppksize));

  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (MilliSeconds (udpAppStartTime));
  clientApps.Stop (MilliSeconds (simTime));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

//  pointToPoint.EnablePcapAll ("w_results/pcaps/second");
//  csma.EnablePcap ("second", csmaDevices.Get (1), true);
  FlowMonitorHelper flowmonHelper;
  NodeContainer endpointNodes;
  endpointNodes.Add (csmaNodes);
  endpointNodes.Add (p2pNodes);
  Ptr<ns3::FlowMonitor> monitor = flowmonHelper.Install (endpointNodes);
  monitor->SetAttribute ("DelayBinWidth", DoubleValue (0.01));
  monitor->SetAttribute ("JitterBinWidth", DoubleValue (0.01));

  Simulator::Stop (MilliSeconds (simTime));

  if(enableflowstats)
    {
      std::cout << "#";
      std::cout << std::setw(9) << "Time (s)";
      std::cout << std::setw(9) << "Flow";
      std::cout << std::setw(20) << "Tput (Mbps)";
      std::cout << std::setw(20) << "Latency (ms)" << std::endl;
      Simulator::Schedule (Seconds (0), &flowstats, monitor, &flowmonHelper);
    }

  Simulator::Run ();

  flowstats(monitor, &flowmonHelper);


  /*
   * To check what was installed in the memory, i.e., BWPs of eNb Device, and its configuration.
   * Example is: Node 1 -> Device 0 -> BandwidthPartMap -> {0,1} BWPs -> NrGnbPhy -> NrPhyMacCommong-> Numerology, Bandwidth, ...
  GtkConfigStore config;
  config.ConfigureAttributes ();
   */



//  pointToPoint.EnablePcapAll("w_results/pcaps/test1", true);

  Simulator::Destroy();
  return 0;
}
