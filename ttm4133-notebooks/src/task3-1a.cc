/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2018 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Jaume Nin <jaume.nin@cttc.cat>
 *          Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/lte-module.h"
#include "ns3/flow-monitor-module.h"
#include <iomanip>
#include <string>

//#include "ns3/gtk-config-store.h"

using namespace ns3;

/**
 * Sample simulation script for LTE+EPC. It instantiates several eNodeBs,
 * attaches one UE per eNodeB starts a flow for each UE to and from a remote host.
 * It also starts another flow between each UE pair.
 */

NS_LOG_COMPONENT_DEFINE ("Task-2-1-Int");
uint32_t ByteCounter1 = 0, ByteCounter2 = 0, ByteCounter3 = 0;
uint32_t oldByteCounter1 = 0, oldByteCounter2 = 0, oldByteCounter3 = 0;
uint16_t cellID = 1;

///#############################################LTE States From Different Prespectives#####################
/// Map each of UE RRC states to its string representation.
static const std::string ueRrcStateName[LteUeRrc::NUM_STATES] =
{
  "IDLE_START",
  "IDLE_CELL_SEARCH",
  "IDLE_WAIT_MIB_SIB1",
  "IDLE_WAIT_MIB",
  "IDLE_WAIT_SIB1",
  "IDLE_CAMPED_NORMALLY",
  "IDLE_WAIT_SIB2",
  "IDLE_RANDOM_ACCESS",
  "IDLE_CONNECTING",
  "CONNECTED_NORMALLY",
  "CONNECTED_HANDOVER",
  "CONNECTED_PHY_PROBLEM",
  "CONNECTED_REESTABLISHING"
};

static const std::string enbRrcStateName[UeManager::NUM_STATES] =
{
  "INITIAL_RANDOM_ACCESS",
  "CONNECTION_SETUP",
  "CONNECTION_REJECTED",
  "ATTACH_REQUEST",
  "CONNECTED_NORMALLY",
  "CONNECTION_RECONFIGURATION",
  "CONNECTION_REESTABLISHMENT",
  "HANDOVER_PREPARATION",
  "HANDOVER_JOINING",
  "HANDOVER_PATH_SWITCH",
  "HANDOVER_LEAVING",
};

static const std::string uePhyStateName[LteUePhy::NUM_STATES] =
{
  "CELL_SEARCH",
  "SYNCHRONIZED"
};

static const std::string ueNasStateName[EpcUeNas::NUM_STATES] =
{
  "OFF",
  "ATTACHING",
  "IDLE_REGISTERED",
  "CONNECTING_TO_EPC",
  "ACTIVE"
};
//#################################################################################################################


void UePhyStateChange(std::string context, uint16_t cellId, uint16_t rnti, LteUePhy::State oldState, LteUePhy::State newState)
{
  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(20) << uePhyStateName[oldState];
  std::cout << std::setw(20) << uePhyStateName[newState] << std::endl;
}
void UeRrcStateChange(std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti, ns3::LteUeRrc::State  oldState, ns3::LteUeRrc::State newState)
{
  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << imsi;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(25) << ueRrcStateName[oldState];
  std::cout << std::setw(25) << ueRrcStateName[newState] << std::endl;
}
void EpcUeNasStateChange(std::string context, const ns3::EpcUeNas::State oldState, const ns3::EpcUeNas::State newState)
{
  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(20) << ueNasStateName[oldState];
  std::cout << std::setw(20) << ueNasStateName[newState] << std::endl;
}
void EnbRrcStateChange(std::string context, const uint64_t imsi, const uint16_t cellId, const uint16_t rnti, const ns3::UeManager::State  oldState, const UeManager::State newState)
{
  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << imsi;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(25) << enbRrcStateName[oldState];
  std::cout << std::setw(25) << enbRrcStateName[newState] << std::endl;
}

void
enbRrcTracing()
{
  std::cout << "#";
  std::cout << std::setw(9) << "time";
  std::cout << std::setw(8) << "cellID";
  std::cout << std::setw(8) << "imsi";
  std::cout << std::setw(8) << "rnti";
  std::cout << std::setw(20) << "OldState" ;
  std::cout << std::setw(20) << "NewState" << std::endl;
  Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteEnbNetDevice/LteEnbRrc/UeMap/*/StateTransition"
      , MakeCallback (&EnbRrcStateChange));
}

void
ReceivePacket1 (Ptr<const Packet> packet, const Address &)
{
  ByteCounter1 += packet->GetSize ();
}

void
ReceivePacket2 (Ptr<const Packet> packet, const Address &)
{
  ByteCounter2 += packet->GetSize ();
}

void
ReceivePacket3 (Ptr<const Packet> packet, const Address &)
{
  ByteCounter3 += packet->GetSize ();
}

void
Throughput (Time binSize)
{
  double  throughput1 = (ByteCounter1 - oldByteCounter1) * 8 / binSize.GetSeconds () / 1024 / 1024;
  double  throughput2 = (ByteCounter2 - oldByteCounter2) * 8 / binSize.GetSeconds () / 1024 / 1024;
  double  throughput3 = (ByteCounter3 - oldByteCounter3) * 8 / binSize.GetSeconds () / 1024 / 1024;

  std::cout << std::setw(9) << Simulator::Now ().GetSeconds ()<< std::setw(9) << cellID << std::setw(11) << throughput1 << std::setw(11) << throughput2 << std::setw(11) << throughput3 << std::endl;
  oldByteCounter1 = ByteCounter1;
  oldByteCounter2 = ByteCounter2;
  oldByteCounter3 = ByteCounter3;

  Simulator::Schedule (binSize, &Throughput, binSize);
}

void RsrpRsrqFromUes(std::string context, uint16_t rnti, uint16_t cellId, double rsrp, double rsrq, bool isServingCell, uint8_t componentCarrierId)
{
  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(15) << rsrp;
  std::cout << std::setw(15) << rsrq << std::endl;
}
void RsrpSinrFromUes (std::string context, uint16_t cellId, uint16_t rnti, double rsrp, double sinr, uint8_t componentCarrierId)
{
  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(15) << rsrp;
  std::cout << std::setw(15) << 10 * std::log10(sinr) << std::endl;
}
//#######################################################ENB##########################################################3
void EnbSinrFromUes(std::string context, uint16_t cellId, uint16_t rnti, double sinrLinear, uint8_t componentCarrierId)
{
  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(15) << 10 * std::log10(sinrLinear) << std::endl;
}

void
NotifyHandoverEndOkEnb (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti)
{
  cellID = cellid;
}

int
main (int argc, char *argv[])
{
  uint16_t numEnb = 3;
//  uint16_t numUe = 2;
  double simTime = 20; //Seconds
  double d1 = 500, enbH = 0;
//  double ueH = 0;
  Time interPacketInterval = MicroSeconds(500);
  double eNBTxPowerDbm = 10;
  double ueTxPowerDbm = 10;
  uint16_t eNBDlEarfcn = 100;
  uint16_t eNBBandwidth = 25;

  bool disableDl = false;
  bool disableUl = false;
  bool enablersrp = false, enablesinrenb = false, enablesinrue = false;
  bool enableflowstats = false, enableRem = false;
  bool enableTraces = true, enableUePhyStates = false, enableUeRrcStates = false,
      enableUeNasStates = false, enableEnbRrcStates = false, enableInstTput = false,
      enableFading = false;
  bool a2a4Ho = false, a3Ho = false;
  uint8_t a2a4SCellThreshold = 30, a2a4neighborCellOffset = 1;

//  LogComponentEnable ("EpcMmeApplication", LOG_LEVEL_ALL);



  // Command line arguments
  CommandLine cmd (__FILE__);
//  cmd.AddValue ("simTime", "Total duration of the simulation", simTime);
//  cmd.AddValue ("interPacketInterval", "Inter packet interval", interPacketInterval);
  cmd.AddValue ("d1", "UE positioning distance", d1);
  cmd.AddValue ("disableDl", "Disable downlink data flows", disableDl);
  cmd.AddValue ("disableUl", "Disable uplink data flows", disableUl);
  cmd.AddValue ("enableUePhyStates", "Enable PHY level UE states", enableUePhyStates);
  cmd.AddValue ("enableUeRrcStates", "Enable RRC level UE states", enableUeRrcStates);
  cmd.AddValue ("enableUeNasStates", "Enable NAS level UE states", enableUeNasStates);
  cmd.AddValue ("enableEnbRrcStates", "Enable RRC level ENB states", enableEnbRrcStates);
  cmd.AddValue ("enablersrp", "get rsrp rsrq stats", enablersrp);
  cmd.AddValue ("enablesinrue", "get ue sinr stats", enablesinrue);
  cmd.AddValue ("enablesinrenb", "get enb sinr stats", enablesinrenb);
  cmd.AddValue ("enableflowstats", "Enable flow level stats", enableflowstats);
  cmd.AddValue ("enableInstTput", "Enable instantenous throughput stats", enableInstTput);
  cmd.AddValue ("enableRem", "Enable radio environment map", enableRem);
  cmd.AddValue ("a2a4Ho", "Enable A2A4 Handover Algorithm", a2a4Ho);
  cmd.AddValue ("a2a4SCellThreshold", "Serving Cell Threshold for A2A4", a2a4SCellThreshold);
  cmd.AddValue ("a2a4neighborCellOffset", "Neighbor Cell Offset for A2A4", a2a4neighborCellOffset);
  cmd.AddValue ("a3Ho", "Enable A3 Handover Algorithm", a3Ho);


  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  // parse again so you can override default values from the command line
  cmd.Parse(argc, argv);

  Ptr<UniformRandomVariable> enbNoise = CreateObject<UniformRandomVariable> ();
  enbNoise->SetAttribute ("Min", DoubleValue (1));
  enbNoise->SetAttribute ("Max", DoubleValue (5));

  Ptr<UniformRandomVariable> ueNoise = CreateObject<UniformRandomVariable> ();
  ueNoise->SetAttribute ("Min", DoubleValue (1));
  ueNoise->SetAttribute ("Max", DoubleValue (5));

  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (eNBTxPowerDbm));
  Config::SetDefault ("ns3::LteUePhy::TxPower", DoubleValue (ueTxPowerDbm));
  Config::SetDefault ("ns3::LteUePhy::NoiseFigure", DoubleValue (ueNoise->GetValue()));
  Config::SetDefault ("ns3::LteEnbPhy::NoiseFigure", DoubleValue (enbNoise->GetValue()));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));



  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (eNBDlEarfcn));
  lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (eNBDlEarfcn + 18000));
  lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (eNBBandwidth));
  lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (eNBBandwidth));
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::ThreeLogDistancePropagationLossModel"));
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");

  if(a2a4Ho)
    {
      lteHelper->SetHandoverAlgorithmType ("ns3::A2A4RsrqHandoverAlgorithm");
      lteHelper->SetHandoverAlgorithmAttribute ("ServingCellThreshold", UintegerValue (a2a4SCellThreshold));
      lteHelper->SetHandoverAlgorithmAttribute ("NeighbourCellOffset", UintegerValue (a2a4neighborCellOffset));
    }
  if(a3Ho)
    {
      lteHelper->SetHandoverAlgorithmType ("ns3::A3RsrpHandoverAlgorithm");
      lteHelper->SetHandoverAlgorithmAttribute ("Hysteresis", DoubleValue (3));
      lteHelper->SetHandoverAlgorithmAttribute ("TimeToTrigger", TimeValue (MilliSeconds (256)));
    }

  //  epcHelper->SetAttribute("S1uLinkEnablePcap", BooleanValue(true));
//  epcHelper->SetAttribute("X2LinkEnablePcap", BooleanValue(true));


  if(enableFading)
    {
      lteHelper->SetAttribute ("FadingModel", StringValue ("ns3::TraceFadingLossModel"));

      std::ifstream ifTraceFile;
      ifTraceFile.open ("../../src/lte/model/fading-traces/fading_trace_EPA_3kmph.fad", std::ifstream::in);
      if (ifTraceFile.good ())
        {
          // script launched by test.py
          lteHelper->SetFadingModelAttribute ("TraceFilename", StringValue ("../../src/lte/model/fading-traces/fading_trace_EPA_3kmph.fad"));
        }
      else
        {
          // script launched as an example
          lteHelper->SetFadingModelAttribute ("TraceFilename", StringValue ("src/lte/model/fading-traces/fading_trace_EPA_3kmph.fad"));
        }

      // these parameters have to be set only in case of the trace format
      // differs from the standard one, that is
      // - 10 seconds length trace
      // - 10,000 samples
      // - 0.5 seconds for window size
      // - 100 RB
      lteHelper->SetFadingModelAttribute ("TraceLength", TimeValue (Seconds (10.0)));
      lteHelper->SetFadingModelAttribute ("SamplesNum", UintegerValue (10000));
      lteHelper->SetFadingModelAttribute ("WindowSize", TimeValue (Seconds (0.5)));
      lteHelper->SetFadingModelAttribute ("RbNum", UintegerValue (100));
    }

  Ptr<Node> pgw = epcHelper->GetPgwNode ();

   // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (10)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  // interface 0 is localhost, 1 is the p2p device
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);
//
//  NodeContainer ueNodes;
  NodeContainer movingueNodes;
  NodeContainer enbNodes;
  enbNodes.Create (numEnb);
//  ueNodes.Create (numUe);
  movingueNodes.Create(1);

  // Install Mobility Model
  Ptr<ListPositionAllocator> enbPa = CreateObject<ListPositionAllocator> ();
  enbPa->Add (Vector (0, 0, enbH));
  enbPa->Add (Vector (d1, 0, enbH));
  enbPa->Add (Vector (2*d1, 0, enbH));


//  Ptr<ListPositionAllocator> uePa = CreateObject<ListPositionAllocator> ();
//  uePa->Add (Vector (d1/4 , d1/2, ueH));
//  uePa->Add (Vector (-d1/4 , -d1/2, ueH));

  MobilityHelper mobilityEnb;
  mobilityEnb.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobilityEnb.SetPositionAllocator(enbPa);
  mobilityEnb.Install(enbNodes);

//  MobilityHelper mobilityUe;
//  mobilityUe.SetMobilityModel("ns3::ConstantPositionMobilityModel");
//  mobilityUe.SetPositionAllocator(uePa);
//  mobilityUe.Install(ueNodes);

  MobilityHelper movingueMobility;
  Ptr<ListPositionAllocator> positionAllocmovingue = CreateObject<ListPositionAllocator> ();

  positionAllocmovingue->Add (Vector (0, d1/4, 0));
  movingueMobility.SetPositionAllocator(positionAllocmovingue);
  movingueMobility.SetMobilityModel("ns3::WaypointMobilityModel");
  movingueMobility.Install (movingueNodes);

  Ptr<WaypointMobilityModel> wayMobility;

  wayMobility = movingueNodes.Get(0)->GetObject<WaypointMobilityModel>();

  Waypoint waypointStart(Seconds(0), Vector3D(0, d1/4, 0));
  Waypoint waypointEnd(Seconds(simTime/2-0.01), Vector3D(d1, d1/4, 0));
  Waypoint waypointStart1(Seconds(simTime/2), Vector3D(d1, d1/4, 0));
  Waypoint waypointEnd1(Seconds(simTime), Vector3D(2*d1, d1/4, 0));

  wayMobility->AddWaypoint(waypointStart);
  wayMobility->AddWaypoint(waypointEnd);
  wayMobility->AddWaypoint(waypointStart1);
  wayMobility->AddWaypoint(waypointEnd1);


  // Install LTE Devices to the nodes
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
//  NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);
  NetDeviceContainer movingueDevs = lteHelper->InstallUeDevice (movingueNodes);


  NodeContainer ueNodes;
//  ueNodes.Add(ueNodes1);
  ueNodes.Add(movingueNodes);

  NetDeviceContainer ueLteDevs;
//  ueDevs.Add(ueDevs1);
  ueLteDevs.Add(movingueDevs);

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIface;
  ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));
  // Assign IP address to UEs, and install applications
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }

//  // Attach one UE per eNodeB
//  for (uint16_t i = 0; i < numNodePairs; i++)
//    {
//      lteHelper->Attach (ueLteDevs.Get(i), enbLteDevs.Get(i));
//      // side effect: the default EPS bearer will be activated
//    }

  lteHelper->Attach(ueLteDevs);

  // Install and start applications on UEs and remote host
  uint16_t dlPort = 1100;
  uint16_t ulPort = 2000;
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;

  Ptr<UniformRandomVariable> startTimeSeconds = CreateObject<UniformRandomVariable> ();
  startTimeSeconds->SetAttribute ("Min", DoubleValue (0));
  startTimeSeconds->SetAttribute ("Max", DoubleValue (0.10));
  Time startTime = Seconds (startTimeSeconds->GetValue ());
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      if (!disableDl)
        {
          PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get (u)));

          UdpClientHelper dlClient (ueIpIface.GetAddress (u), dlPort);
          dlClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
          dlClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
          clientApps.Add (dlClient.Install (remoteHost));
        }

      if (!disableUl)
        {
          ++ulPort;
          PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          serverApps.Add (ulPacketSinkHelper.Install (remoteHost));

          UdpClientHelper ulClient (remoteHostAddr, ulPort);
          ulClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
          ulClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
          clientApps.Add (ulClient.Install (ueNodes.Get(u)));
        }

    }

  serverApps.Start (startTime);
  clientApps.Start (startTime);

  // Add X2 interface
  lteHelper->AddX2Interface (enbNodes);

  if(enableTraces)
    {
      if(enableUePhyStates)
        {
          std::cout << "#";
          std::cout << std::setw(9) << "time";
          std::cout << std::setw(8) << "cellID";
          std::cout << std::setw(8) << "rnti";
          std::cout << std::setw(20) << "OldState" ;
          std::cout << std::setw(20) << "NewState" << std::endl;
          Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteUeNetDevice/ComponentCarrierMapUe/*/LteUePhy/StateTransition"
              , MakeCallback (&UePhyStateChange));
        }
      if(enableUeRrcStates)
        {
          std::cout << "#";
          std::cout << std::setw(9) << "time";
          std::cout << std::setw(8) << "cellID";
          std::cout << std::setw(8) << "imsi";
          std::cout << std::setw(8) << "rnti";
          std::cout << std::setw(20) << "OldState" ;
          std::cout << std::setw(20) << "NewState" << std::endl;
          Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteUeNetDevice/LteUeRrc/StateTransition"
              , MakeCallback (&UeRrcStateChange));
        }
      if(enableUeNasStates)
        {
          std::cout << "#";
          std::cout << std::setw(9) << "time";
          std::cout << std::setw(20) << "OldState" ;
          std::cout << std::setw(20) << "NewState" << std::endl;
          Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteUeNetDevice/EpcUeNas/StateTransition"
              , MakeCallback (&EpcUeNasStateChange));
        }

      if(enablersrp)
        {
          std::cout << "#";
          std::cout << std::setw(9) << "time";
          std::cout << std::setw(8) << "cellID";
          std::cout << std::setw(8) << "rnti" ;
          std::cout << std::setw(12) << "rsrp" ;
          std::cout << std::setw(12) << "rsrq" << std::endl;

          Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteUeNetDevice/ComponentCarrierMapUe/*/LteUePhy/ReportUeMeasurements"
              , MakeCallback (&RsrpRsrqFromUes));
        }

      if(enablesinrenb)
        {
          std::cout << "#";
          std::cout << std::setw(9) << "time";
          std::cout << std::setw(8) << "cellID";
          std::cout << std::setw(8) << "rnti" ;
          std::cout << std::setw(12) << "sinr" << std::endl;

          Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteEnbNetDevice/ComponentCarrierMap/*/LteEnbPhy/ReportUeSinr"
              , MakeCallback (&EnbSinrFromUes));
        }

      if(enablesinrue)
        {
          std::cout << "#";
          std::cout << std::setw(9) << "time";
          std::cout << std::setw(8) << "cellID";
          std::cout << std::setw(8) << "rnti" ;
          std::cout << std::setw(12) << "rsrp" ;
          std::cout << std::setw(12) << "sinr" << std::endl;

          Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteUeNetDevice/ComponentCarrierMapUe/*/LteUePhy/ReportCurrentCellRsrpSinr"
              , MakeCallback (&RsrpSinrFromUes));
        }
    }
  // Uncomment to enable PCAP tracing
  //p2ph.EnablePcapAll("lena-simple-epc");

  Ptr<RadioEnvironmentMapHelper> remHelper;
  if(enableRem)
    {
      //    PrintGnuplottableBuildingListToFile ("buildings.txt");
      //    PrintGnuplottableEnbListToFile ("enbs.txt");
      //    PrintGnuplottableUeListToFile ("ues.txt");
      // Configure Radio Environment Map (REM) output
      // for LTE-only simulations always use /ChannelList/0 which is the downlink channel
      remHelper = CreateObject<RadioEnvironmentMapHelper> ();
      //  remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/0"));
      remHelper->SetAttribute ("Channel", PointerValue (lteHelper->GetDownlinkSpectrumChannel ()));
      remHelper->SetAttribute ("OutputFile", StringValue ("task3-1a-rem.out"));
      remHelper->SetAttribute ("XMin", DoubleValue (-1.5*d1));
      remHelper->SetAttribute ("XMax", DoubleValue (numEnb*d1));
      remHelper->SetAttribute ("YMin", DoubleValue (-1.5*d1));
      remHelper->SetAttribute ("YMax", DoubleValue (1.5*d1));
      remHelper->SetAttribute ("Z", DoubleValue (1.5));
      remHelper->Install ();
    }

  FlowMonitorHelper flowmonHelper;
  Ptr<ns3::FlowMonitor> monitor;
  if(enableflowstats)
    {

      NodeContainer endpointNodes;
      endpointNodes.Add (ueNodes);
      endpointNodes.Add(remoteHost);

      monitor = flowmonHelper.Install (endpointNodes);
      monitor->SetAttribute ("DelayBinWidth", DoubleValue (0.001));
      monitor->SetAttribute ("JitterBinWidth", DoubleValue (0.001));
      monitor->SetAttribute ("PacketSizeBinWidth", DoubleValue (20));
      //
      //      monitor->SerializeToXmlFile("flowmon.xml", true, true);
      //
      //      std::string anim_filename = outputDir + "/" + "anim.xml";
      //      AnimationInterface anim (anim_filename);
      //      anim.SetMobilityPollInterval (MilliSeconds(5));
      //      anim.EnablePacketMetadata (true);
      //      anim.EnableIpv4L3ProtocolCounters(startTime, simTime, MilliSeconds(1));
      //      anim.UpdateNodeSize (6, 1.5, 1.5);
    }


  if(enableTraces)
    {
      if(enableEnbRrcStates)
        {
          Simulator::Schedule(Seconds(0.26), &enbRrcTracing);
        }
//      lteHelper->EnableTraces ();

    }
//  p2ph.EnablePcapAll("w_results/pcaps/task2-1", true);

  if(enableInstTput)
    {
      std::ostringstream oss1, oss2, oss3;
      oss1 << "/NodeList/" << ueNodes.Get (0)->GetId () << "/ApplicationList/0/$ns3::PacketSink/Rx";
//      oss2<< "/NodeList/" << ueNodes.Get (1)->GetId () << "/ApplicationList/0/$ns3::PacketSink/Rx";
//      oss3 << "/NodeList/" << ueNodes.Get (2)->GetId () << "/ApplicationList/0/$ns3::PacketSink/Rx";

      Config::ConnectWithoutContext (oss1.str (), MakeCallback (&ReceivePacket1));
//      Config::ConnectWithoutContext (oss2.str (), MakeCallback (&ReceivePacket2));
//      Config::ConnectWithoutContext (oss3.str (), MakeCallback (&ReceivePacket3));
      Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
                       MakeCallback (&NotifyHandoverEndOkEnb));

      std::cout << "#";
      std::cout << std::setw(9) << "time";
      std::cout << std::setw(9) << "cellID";
      std::cout << std::setw(11) << "Throughput-UE1 (Mbps)" << std::setw(11) << "Throughput-UE2 (Mbps)" << std::setw(11) << "Throughput-UE3 (Mbps)" << std::endl;
      Time binSize = Seconds (0.02);
      Simulator::Schedule (Seconds (0.1), &Throughput, binSize);
    }



  Simulator::Stop (Seconds(simTime));
  Simulator::Run ();



  if(enableflowstats)
    {
      // Print per-flow statistics
      monitor->CheckForLostPackets ();
      Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmonHelper.GetClassifier ());
      FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();

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

          if(t.sourceAddress.GetSubnetDirectedBroadcast("255.0.0.0") == "7.255.255.255")
            {
              channel = "UL";
            }
          else
            {
              channel = "DL";
            }
          std::cout  << "\n<" << channel << "> " << "Flow " << i->first << " (" << t.sourceAddress << ":" << t.sourcePort << " -> " << t.destinationAddress << ":" << t.destinationPort << ") proto " << protoStream.str () << "\n\n";
          //    std::cout  << "  Tx Packets: " << i->second.txPackets << "\n";
          //    std::cout  << "  Tx Bytes:   " << i->second.txBytes << "\n";
          //    std::cout  << "  TxOffered:  " << (((i->second.txBytes * 8.0) / (simTime.GetSeconds() - startTime.GetSeconds())) / 1000) / 1000  << " Mbps\n";
          //    std::cout  << "  Rx Bytes:   " << i->second.rxBytes << "\n";
                std::cout << "   Packet Lost: " << i->second.lostPackets << "\n";

          if (i->second.rxPackets > 0)
            {
              // Measure the duration of the flow from receiver's perspective
              //double rxDuration = i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ();
              double rxDuration = simTime - startTime.GetSeconds();

              averageFlowThroughput += (((i->second.rxBytes * 8.0) / rxDuration) / 1024) / 1024;
              averageFlowDelay += 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets;

              std::cout  << "  Throughput: " << (((i->second.rxBytes * 8.0) / rxDuration) / 1024) / 1024  << " Mbps\n";
              std::cout  << "  Mean delay:  " << 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets << " ms\n";
              //outFile << "  Mean upt:  " << i->second.uptSum / i->second.rxPackets / 1000/1000 << " Mbps \n";
              std::cout  << "  Mean jitter:  " << 1000 * i->second.jitterSum.GetSeconds () / i->second.rxPackets  << " ms\n";
            }
          else
            {
              std::cout << "  Throughput:  0 Mbps\n";
              std::cout  << "  Mean delay:  0 ms\n";
              std::cout  << "  Mean jitter: 0 ms\n";
            }
        }
    }

  /*GtkConfigStore config;
  config.ConfigureAttributes();*/

  Simulator::Destroy ();
  return 0;
}
