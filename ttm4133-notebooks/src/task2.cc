/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 */


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/config-store.h"
#include "ns3/radio-bearer-stats-calculator.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include <iomanip>
#include <string>
#include "ns3/flow-monitor-module.h"
#include "ns3/rectangle.h"
#include <ns3/buildings-module.h>
#include <ns3/spectrum-helper.h>



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Task-2");

uint32_t ByteCounter1 = 0, ByteCounter2 = 0, ByteCounter3 = 0;
uint32_t oldByteCounter1 = 0, oldByteCounter2 = 0, oldByteCounter3 = 0;
/**
 * This simulation script creates two eNodeBs and drops randomly several UEs in
 * a disc around them (same number on both). The number of UEs , the radius of
 * that disc and the distance between the eNodeBs can be configured.
 */
//#############################################Traces Source Overloads###############################################3
//#######################################################UE##########################################################3
void RsrpRsrqFromUes(std::string context, uint16_t rnti, uint16_t cellId, double rsrp, double rsrq, bool isServingCell, uint8_t componentCarrierId)
{
  //  if (!out_uephyrsrprsrqfile.is_open ())
  //    {
  //      std::cout << uephyrsrprsrqfile << "failed to open..." << std::endl;
  //    }
  //  out_uephyrsrprsrqfile << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  //  out_uephyrsrprsrqfile << std::setw(7) << cellId;
  //  out_uephyrsrprsrqfile << std::setw(7) << rnti;
  //  out_uephyrsrprsrqfile << std::setw(15) << rsrp;
  //  out_uephyrsrprsrqfile << std::setw(15) << rsrq << std::endl;

  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(15) << rsrp;
  std::cout << std::setw(15) << rsrq << std::endl;
}
void RsrpSinrFromUes (std::string context, uint16_t cellId, uint16_t rnti, double rsrp, double sinr, uint8_t componentCarrierId)
{
  //  if (!out_uephyrsrpsinrfile.is_open ())
  //    {
  //      std::cout << uephyrsrpsinrfile << "failed to open..." << std::endl;
  //    }
  //  out_uephyrsrpsinrfile << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  //  out_uephyrsrpsinrfile << std::setw(7) << cellId;
  //  out_uephyrsrpsinrfile << std::setw(7) << rnti;
  //  out_uephyrsrpsinrfile << std::setw(15) << rsrp;
  //  out_uephyrsrpsinrfile << std::setw(15) << sinr << std::endl;

  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(15) << rsrp;
  std::cout << std::setw(15) << 10 * std::log10(sinr) << std::endl;
}
//#######################################################ENB##########################################################3
void EnbSinrFromUes(std::string context, uint16_t cellId, uint16_t rnti, double sinrLinear, uint8_t componentCarrierId)
{
  //  if (!out_enbphysinrfile.is_open ())
  //    {
  //      std::cout << enbphysinrfile << "failed to open..." << std::endl;
  //    }
  //  out_enbphysinrfile << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  //  out_enbphysinrfile << std::setw(7) << cellId;
  //  out_enbphysinrfile << std::setw(7) << rnti;
  //  out_enbphysinrfile << std::setw(15) << sinrLinear << std::endl;

  std::cout << std::fixed << std::setw (10) << Simulator::Now ().GetSeconds ();
  std::cout << std::setw(7) << cellId;
  std::cout << std::setw(7) << rnti;
  std::cout << std::setw(15) << 10 * std::log10(sinrLinear) << std::endl;
}

void
PrintGnuplottableBuildingListToFile (std::string filename)
{
  std::ofstream outFile;
  outFile.open (filename.c_str (), std::ios_base::out | std::ios_base::trunc);
  if (!outFile.is_open ())
    {
      return;
    }
  uint32_t index = 0;
  for (BuildingList::Iterator it = BuildingList::Begin (); it != BuildingList::End (); ++it)
    {
      ++index;
      Box box = (*it)->GetBoundaries ();
      outFile << "set object " << index
              << " rect from " << box.xMin  << "," << box.yMin
              << " to "   << box.xMax  << "," << box.yMax
              << std::endl;
    }
}

//static void
//RecordMovement (Ptr<MobilityModel> rxMob)
//{
//  // print the SNR and pathloss values in the snr-trace.txt file
//  std::ofstream f;
//  f.open ("task2-output.txt", std::ios::out | std::ios::app);
//  f << Simulator::Now ().GetSeconds () << " " // time [s]
//    << rxMob->GetPosition ().x << " "
//    << rxMob->GetPosition ().y << " " << std::endl; // pathloss [dB]
//  f.close ();
//}

void
SaveUePosition (NodeContainer nodes, std::ostream *os, double interval)
{
  for (NodeContainer::Iterator node = nodes.Begin (); node != nodes.End (); ++node)
    {
      uint32_t nodeId = (*node)->GetId();
      uint64_t nodeImsi = (Ptr<NetDevice>((*node)->GetDevice(0)))->GetObject<LteUeNetDevice> ()->GetImsi ();
      Ptr<MobilityModel> mobModel = (*node)->GetObject<MobilityModel>();
      Vector3D pos = mobModel->GetPosition();

      // Prints position and velocities
      *os << Simulator::Now().GetSeconds() << " "
          << nodeId << " "
          << nodeImsi << " "
          << pos.x << " " << pos.y << " " << std::endl;
    }
  Simulator::Schedule(Seconds(interval), &SaveUePosition, nodes, os, interval);
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

  std::cout << std::setw(9) << Simulator::Now ().GetSeconds () << std::setw(11) << throughput1 << std::setw(11) << throughput2 << std::setw(11) << throughput3 << std::endl;
  oldByteCounter1 = ByteCounter1;
  oldByteCounter2 = ByteCounter2;
  oldByteCounter3 = ByteCounter3;

  Simulator::Schedule (binSize, &Throughput, binSize);
}

int main (int argc, char *argv[])
{
  double enbDist = 300.0;

  uint32_t numUes = 2;
  double simTime = 20; //  Time timeRes = MilliSeconds (10); // time resolution
  //  double speed = 500;       // m/s
  uint8_t numBearersPerUe = 1;
  double eNBTxPowerDbm = 40;
  uint16_t eNBDlEarfcn = 100;
  uint16_t eNBBandwidth = 25;
  double hEnb = 30.0;
  double hUe = 1.0;

    
  bool enablersrp = false, enablesinrenb = false, enablesinrue = false;
  bool enableRem = false, enableflowstats = false, enableTraces = true;
  bool enablebuilding = true, enableInstTput=false;
  CommandLine cmd (__FILE__);
  cmd.AddValue ("enbDist", "distance between the two eNBs", enbDist);
  cmd.AddValue ("numUes", "how many UEs are attached to each eNB", numUes);
  cmd.AddValue ("eNBTxPowerDbm", "base station power level", eNBTxPowerDbm);
  cmd.AddValue ("enableflowstats", "get flow stats", enableflowstats);
  cmd.AddValue ("enablersrp", "get rsrp rsrq stats", enablersrp);
  cmd.AddValue ("enablesinrue", "get ue sinr stats", enablesinrue);
  cmd.AddValue ("enablesinrenb", "get enb sinr stats", enablesinrenb);
  cmd.AddValue ("enablebuilding", "enable buildings in topology", enablebuilding);
  cmd.AddValue ("enableInstTput", "Enable instantenous throughput stats", enableInstTput);


  cmd.Parse (argc, argv);

    
  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();
    

  double radius = enbDist/1;

  Ptr<UniformRandomVariable> enbNoise = CreateObject<UniformRandomVariable> ();
  enbNoise->SetAttribute ("Min", DoubleValue (1));
  enbNoise->SetAttribute ("Max", DoubleValue (5));

  Ptr<UniformRandomVariable> ueNoise = CreateObject<UniformRandomVariable> ();
  ueNoise->SetAttribute ("Min", DoubleValue (1));
  ueNoise->SetAttribute ("Max", DoubleValue (5));

  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (eNBTxPowerDbm));
  Config::SetDefault ("ns3::LteUePhy::NoiseFigure", DoubleValue (ueNoise->GetValue()));
  Config::SetDefault ("ns3::LteEnbPhy::NoiseFigure", DoubleValue (enbNoise->GetValue()));
  Config::SetDefault("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue(80));



  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);


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
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);


  // Routing of the Internet Host (towards the LTE network)
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  // interface 0 is localhost, 1 is the p2p device
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes1, movingueNodes;
  enbNodes.Create (1);
  ueNodes1.Create (numUes);
  movingueNodes.Create(1);

  uint8_t floors = 10;
  uint8_t Xrooms = 5;
  uint8_t Yrooms = 5;

  if(enablebuilding)
    {
      Ptr<Building> b3, b1, b2, b4;
      b1 = CreateObject <Building> ();
      b2 = CreateObject <Building> ();
      b3 = CreateObject <Building> ();
      b4 = CreateObject <Building> ();

      //Building in quadrant 1
      b1->SetBoundaries (Box (radius/2, 2*radius/3, radius/2, 2*radius/3, 0, 5));
      b1->SetBuildingType (Building::Residential);
      b1->SetExtWallsType (Building::ConcreteWithWindows);
      b1->SetNFloors (floors);
      b1->SetNRoomsX (Xrooms);
      b1->SetNRoomsY (Yrooms);
      //Building in quadrant 2
      b2->SetBoundaries (Box (-radius/2, -2*radius/3, radius/2, 2*radius/3, 0, 7.5));
      b2->SetBuildingType (Building::Residential);
      b2->SetExtWallsType (Building::ConcreteWithWindows);
      b2->SetNFloors (floors);
      b2->SetNRoomsX (Xrooms);
      b2->SetNRoomsY (Yrooms);

      //Building in quadrant 3
      b3->SetBoundaries (Box (-radius/2, -2*radius/3, -radius/2, -2*radius/3, 0, 10));
      b3->SetBuildingType (Building::Residential);
      b3->SetExtWallsType (Building::ConcreteWithWindows);
      b3->SetNFloors (floors);
      b3->SetNRoomsX (Xrooms);
      b3->SetNRoomsY (Yrooms);

      //Building in quadrant 4
      b4->SetBoundaries (Box (radius/2, 2*radius/3, -radius/2, -2*radius/3, 0, 7.5));
      b4->SetBuildingType (Building::Residential);
      b4->SetExtWallsType (Building::ConcreteWithWindows);
      b4->SetNFloors (3);
      b4->SetNRoomsX (3);
      b4->SetNRoomsY (2);

    }

  // Position of eNBs
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, hEnb));
  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator (positionAlloc);
  enbMobility.Install (enbNodes);
  lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (eNBDlEarfcn));
  lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (eNBDlEarfcn + 18000));
  lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (eNBBandwidth));
  lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (eNBBandwidth));

  // Position of UEs attached to eNB 1
  Ptr<ListPositionAllocator> positionAllocue = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (radius/7, radius/7, hUe));
  positionAlloc->Add (Vector (-(6*radius)/7, -(6*radius)/7, hUe));
  MobilityHelper ue1mobility;
  ue1mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator (positionAllocue);
  ue1mobility.Install (ueNodes1);



//  Ptr<MobilityModel> rxMob = CreateObject<RandomWalk2dOutdoorMobilityModel> ();
//  rxMob->GetObject<RandomWalk2dOutdoorMobilityModel>()->SetAttribute("Mode", StringValue ("Time"));
//  rxMob->GetObject<RandomWalk2dOutdoorMobilityModel>()->SetAttribute("Time", StringValue ("0.5s"));
//  rxMob->GetObject<RandomWalk2dOutdoorMobilityModel>()->SetAttribute("Speed", StringValue ("ns3::ConstantRandomVariable[Constant=2.0]"));
//  rxMob->GetObject<RandomWalk2dOutdoorMobilityModel>()->SetAttribute("Bounds", RectangleValue (Rectangle (-(6*radius)/7, (6*radius)/7, -(6*radius)/7, (6*radius)/7)));
  //  movingueNodes.Get(0)->AggregateObject(rxMob);

//   Install Mobility Model in UE
//  MobilityHelper movingueMobility;
//  movingueMobility.SetMobilityModel ("ns3::RandomWalk2dOutdoorMobilityModel",
//                                     "Mode", StringValue ("Time"),
//                                     "Time", StringValue ("0.5s"),
//                                     "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=2.0]"),
//                                     "Bounds", RectangleValue (Rectangle (-(6*radius)/7, (6*radius)/7, -(6*radius)/7, (6*radius)/7)));
//  movingueMobility.Install (movingueNodes);

  MobilityHelper movingueMobility;
  Ptr<ListPositionAllocator> positionAllocmovingue = CreateObject<ListPositionAllocator> ();

  positionAllocmovingue->Add (Vector (-(6*radius)/7, (6*radius)/7, hUe));
  movingueMobility.SetPositionAllocator(positionAllocmovingue);
  movingueMobility.SetMobilityModel("ns3::WaypointMobilityModel");
  movingueMobility.Install (movingueNodes);

  Ptr<WaypointMobilityModel> wayMobility;

  wayMobility = movingueNodes.Get(0)->GetObject<WaypointMobilityModel>();

  Waypoint waypointStart(Seconds(0), Vector3D(-(6*radius)/7, (6*radius)/7, 1.5));
  Waypoint waypointEnd(Seconds(simTime/4-0.01), Vector3D((radius)/10, (6*radius)/7, 1.5));

  Waypoint waypointStart1(Seconds(simTime/4), Vector3D((radius)/10, (6*radius)/7, 1.5));
  Waypoint waypointEnd1(Seconds(simTime/2), Vector3D((radius)/8, -(11*radius)/8, 1.5));

  Waypoint waypointStart2(Seconds(simTime/2 + 0.01), Vector3D((radius)/8, -(11*radius)/8, 1.5));
  Waypoint waypointEnd2(Seconds(3*simTime/4), Vector3D((11*radius)/7, -(6*radius)/7, 1.5));

  Waypoint waypointStart3(Seconds(3*simTime/4 + 0.01), Vector3D((11*radius)/7, -(6*radius)/7, 1.5));
  Waypoint waypointEnd3(Seconds(simTime), Vector3D((6*radius)/7, (6*radius)/7, 1.5));

  wayMobility->AddWaypoint(waypointStart);
  wayMobility->AddWaypoint(waypointEnd);
  wayMobility->AddWaypoint(waypointStart1);
  wayMobility->AddWaypoint(waypointEnd1);
  wayMobility->AddWaypoint(waypointStart2);
  wayMobility->AddWaypoint(waypointEnd2);
  wayMobility->AddWaypoint(waypointStart3);
  wayMobility->AddWaypoint(waypointEnd3);

  Ptr<ConstantPositionMobilityModel> mmEnb = CreateObject<ConstantPositionMobilityModel> ();
  Ptr<ConstantPositionMobilityModel> mmUe1 = CreateObject<ConstantPositionMobilityModel> ();
  Ptr<ConstantPositionMobilityModel> mmUe2 = CreateObject<ConstantPositionMobilityModel> ();


  mmEnb = enbNodes.Get(0)->GetObject<ConstantPositionMobilityModel> ();
  mmUe1 = ueNodes1.Get(0)->GetObject<ConstantPositionMobilityModel> ();
  mmUe2 = ueNodes1.Get(1)->GetObject<ConstantPositionMobilityModel> ();

  mmEnb->SetPosition (Vector (0.0, 0.0, hEnb));
  mmUe1->SetPosition (Vector (radius/7, radius/7, hUe));
  mmUe2->SetPosition (Vector (-(6*radius)/7, -(6*radius)/7, hUe));
  wayMobility->SetPosition(Vector (-(6*radius)/7, (6*radius)/7, hUe));


 if(enablebuilding)
   {
     // BuildingsHelper::Install (enbNodes);
     // BuildingsHelper::Install (ueNodes1);
     // BuildingsHelper::Install (movingueNodes);
   }



  Ptr<ThreeLogDistancePropagationLossModel> slowFadingLModel = CreateObject<ThreeLogDistancePropagationLossModel> ();

  //  Ptr<FriisPropagationLossModel> slowFadingLModel1 = CreateObject<Cost231PropagationLossModel> ();
//
//  SpectrumChannelHelper channelHelper;
//
//  Config::SetDefault ("ns3::NakagamiPropagationLossModel::m0", DoubleValue (1.0));
//  Config::SetDefault ("ns3::NakagamiPropagationLossModel::m1", DoubleValue (1.0));
//  Config::SetDefault ("ns3::NakagamiPropagationLossModel::m2", DoubleValue (1.0));


  if(enablebuilding)
    {

      Ptr<MobilityBuildingInfo> buildingInfoEnb = CreateObject<MobilityBuildingInfo> ();
      Ptr<MobilityBuildingInfo> buildingInfoUe1 = CreateObject<MobilityBuildingInfo> ();
      Ptr<MobilityBuildingInfo> buildingInfoUe2 = CreateObject<MobilityBuildingInfo> ();
      Ptr<MobilityBuildingInfo> buildingInfoMovingUe = CreateObject<MobilityBuildingInfo> ();

      mmEnb->AggregateObject (buildingInfoEnb); // operation usually done by BuildingsHelper::Install
      buildingInfoEnb->MakeConsistent (mmEnb);
      mmUe1->AggregateObject (buildingInfoUe1); // operation usually done by BuildingsHelper::Install
      buildingInfoUe1->MakeConsistent (mmUe1);
      mmUe2->AggregateObject (buildingInfoUe2); // operation usually done by BuildingsHelper::Install
      buildingInfoUe2->MakeConsistent (mmUe2);
      wayMobility->AggregateObject(buildingInfoMovingUe);
      buildingInfoMovingUe->MakeConsistent (wayMobility);

      lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
      lteHelper->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (2));
      lteHelper->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (2));
      lteHelper->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (2));

    }
  else
    {
      // lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::LogDistancePropagationLossModel"));
      // lteHelper->SetPathlossModelAttribute("Exponent", DoubleValue (3.0));
      // lteHelper->SetPathlossModelAttribute("ReferenceDistance", DoubleValue (10));
      // lteHelper->SetPathlossModelAttribute("Frequency", DoubleValue (2120000000));

      lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::ThreeLogDistancePropagationLossModel"));
      // lteHelper->SetPathlossModelAttribute("Exponent0", DoubleValue (1.0));
      // lteHelper->SetPathlossModelAttribute("Exponent1", DoubleValue (3.0));
      // lteHelper->SetPathlossModelAttribute("Exponent2", DoubleValue (10.0));
      // lteHelper->SetPathlossModelAttribute("Distance0", DoubleValue (100));
      // lteHelper->SetPathlossModelAttribute("Distance1", DoubleValue (200));
      // lteHelper->SetPathlossModelAttribute("Distance2", DoubleValue (400));
//      lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::NakagamiPropagationLossModel"));
//      lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::NakagamiPropagationLossModel"));
//      channelHelper.AddPropagationLoss(fastFadingLModel);
//      channelHelper.AddPropagationLoss(slowFadingLModel1);
    }


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


  //  movingueMobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  //  movingueMobility.Install (movingueNodes);
  //  movingueNodes.Get (0)->GetObject<MobilityModel> ()->SetPosition (Vector ((-6*radius)/7, (6*radius)/7, 0));
  //  movingueNodes.Get (0)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (speed, speed, 0));


  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs1, ueDevs2, ueDevs3, movingueDevs;
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs1 = lteHelper->InstallUeDevice (ueNodes1);
  movingueDevs = lteHelper->InstallUeDevice (movingueNodes);


  NodeContainer ueNodes;
  ueNodes.Add(ueNodes1);
  ueNodes.Add(movingueNodes);

  NetDeviceContainer ueDevs;
  ueDevs.Add(ueDevs1);
  ueDevs.Add(movingueDevs);




  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIfaces;
  ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ue = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }


  // Attach UEs to a eNB
  lteHelper->Attach (ueDevs1, enbDevs.Get (0));
  lteHelper->Attach (movingueDevs, enbDevs.Get (0));

  // Install and start applications on UEs and remote host
  uint16_t dlPort = 10000;
  uint16_t ulPort = 20000;

  // randomize a bit start times to avoid simulation artifacts
  // (e.g., buffer overflows due to packet transmissions happening
  // exactly at the same time)
  Ptr<UniformRandomVariable> startTimeSeconds = CreateObject<UniformRandomVariable> ();
  startTimeSeconds->SetAttribute ("Min", DoubleValue (0));
  startTimeSeconds->SetAttribute ("Max", DoubleValue (0.010));
  Time startTime = Seconds (startTimeSeconds->GetValue ());
  //
  //  for (uint32_t i = 0; i < numUes; ++i)
  //    {
  //
  //    }

  //  std::cout << ueNodes.GetN() <<" "<< ueIpIfaces.GetN() <<std::endl;

  for (uint32_t u = 0; u < ueNodes.GetN(); ++u)
    {
      Ptr<Node> ue = ueNodes.Get (u);
      //std::cout <<"UE IP: "<< ueIpIfaces.GetAddress (u) <<std::endl;

      // Set the default gateway for the UE
      //      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
      //      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);

      for (uint32_t b = 0; b < numBearersPerUe; ++b)
        {
          ++dlPort;
          ++ulPort;

          ApplicationContainer clientApps;
          ApplicationContainer serverApps;

          //      UdpClientHelper dlClientHelper (ueIpIfaces.GetAddress (u), dlPort);
          //      clientApps.Add (dlClientHelper.Install (remoteHost));
          //      PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory",
          //                                           InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          //      serverApps.Add (dlPacketSinkHelper.Install (ue));
          //
          //      UdpClientHelper ulClientHelper (remoteHostAddr, ulPort);
          //      clientApps.Add (ulClientHelper.Install (ue));
          //      PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory",
          //                                           InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          //      serverApps.Add (ulPacketSinkHelper.Install (remoteHost));

          //DL
          UdpClientHelper dlClientHelper (ueIpIfaces.GetAddress (u), dlPort);
          dlClientHelper.SetAttribute ("MaxPackets", UintegerValue (0xFFFFFFFF));
          dlClientHelper.SetAttribute ("Interval", TimeValue (Seconds (1.0 / 1000)));
          dlClientHelper.SetAttribute ("PacketSize", UintegerValue (1024));
          clientApps.Add (dlClientHelper.Install (remoteHost));
          PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory",
                                               InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          serverApps.Add (dlPacketSinkHelper.Install (ue));

          //UL
          UdpClientHelper ulClientHelper (remoteHostAddr, ulPort);
          ulClientHelper.SetAttribute ("MaxPackets", UintegerValue (0xFFFFFFFF));
          ulClientHelper.SetAttribute ("Interval", TimeValue (Seconds (1.0 / 1000)));
          ulClientHelper.SetAttribute ("PacketSize", UintegerValue (1024));
          clientApps.Add (ulClientHelper.Install (ue));
          PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory",
                                               InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          serverApps.Add (ulPacketSinkHelper.Install (remoteHost));


          Ptr<EpcTft> tft = Create<EpcTft> ();
          EpcTft::PacketFilter dlpf;
          dlpf.localPortStart = dlPort;
          dlpf.localPortEnd = dlPort;
          tft->Add (dlpf);
          EpcTft::PacketFilter ulpf;
          ulpf.remotePortStart = ulPort;
          ulpf.remotePortEnd = ulPort;
          tft->Add (ulpf);
          EpsBearer bearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT);
          lteHelper->ActivateDedicatedEpsBearer (ueDevs.Get (u), bearer, tft);

          serverApps.Start (startTime);
          clientApps.Start (startTime);
          serverApps.Stop (Seconds(simTime));
          clientApps.Stop (Seconds(simTime));

        } //end bearers


    } //end apps


  lteHelper->AddX2Interface (enbNodes);


  if(enableTraces)
    {
      //#####################################File Inits########################################################
      //#######################################UEs#############################################################
      if(enablersrp)
        {
          //          out_uephyrsrprsrqfile.open (uephyrsrprsrqfile.c_str(), std::ios_base::out | std::ios_base::trunc);
          //          if (!out_uephyrsrprsrqfile.is_open ())
          //            {
          //              std::cout << uephyrsrprsrqfile << " failed to open..." << std::endl;
          //            }
          //          out_uephyrsrprsrqfile << "#";
          //          out_uephyrsrprsrqfile << std::setw(9) << "time";
          //          out_uephyrsrprsrqfile << std::setw(8) << "cellID";
          //          out_uephyrsrprsrqfile << std::setw(8) << "rnti" ;
          //          out_uephyrsrprsrqfile << std::setw(12) << "rsrp" ;
          //          out_uephyrsrprsrqfile << std::setw(12) << "rsrq" << std::endl;

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
          //          out_enbphysinrfile.open (enbphysinrfile.c_str(), std::ios_base::out | std::ios_base::trunc);
          //          if (!out_enbphysinrfile.is_open ())
          //            {
          //              std::cout << enbphysinrfile << " failed to open..." << std::endl;
          //            }
          //          out_enbphysinrfile << "#";
          //          out_enbphysinrfile << std::setw(9) << "time";
          //          out_enbphysinrfile << std::setw(8) << "cellID";
          //          out_enbphysinrfile << std::setw(8) << "rnti" ;
          //          out_enbphysinrfile << std::setw(12) << "sinr" << std::endl;



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
          //          out_uephyrsrpsinrfile.open (uephyrsrpsinrfile.c_str(), std::ios_base::out | std::ios_base::trunc);
          //          if (!out_uephyrsrpsinrfile.is_open ())
          //            {
          //              std::cout << uephyrsrpsinrfile << " failed to open..." << std::endl;
          //            }
          //          out_uephyrsrpsinrfile << "#";
          //          out_uephyrsrpsinrfile << std::setw(9) << "time";
          //          out_uephyrsrpsinrfile << std::setw(8) << "cellID";
          //          out_uephyrsrpsinrfile << std::setw(8) << "rnti" ;
          //          out_uephyrsrpsinrfile << std::setw(12) << "rsrp" ;
          //          out_uephyrsrpsinrfile << std::setw(12) << "sinr" << std::endl;

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
      remHelper->SetAttribute ("OutputFile", StringValue ("test-rem.out"));
      remHelper->SetAttribute ("XMin", DoubleValue (-enbDist));
      remHelper->SetAttribute ("XMax", DoubleValue (enbDist));
      remHelper->SetAttribute ("YMin", DoubleValue (-enbDist));
      remHelper->SetAttribute ("YMax", DoubleValue (enbDist));
      remHelper->SetAttribute ("Z", DoubleValue (1.5));
      remHelper->Install ();
    }

  FlowMonitorHelper flowmonHelper;
  Ptr<ns3::FlowMonitor> monitor;
  if(enableflowstats)
    {

      NodeContainer endpointNodes;
      endpointNodes.Add (ueNodes1);
      endpointNodes.Add (movingueNodes);
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


  PrintGnuplottableBuildingListToFile ("buildings.txt");



//  std::string logFile = "ue-mob.log";
//  // open log file for output
//  std::ofstream os;
//  os.open (logFile.c_str ());
//  // Prints position and velocities
//  os << "<TIME>," << "<ID>," << "<IMSI>," <<"<POSITION>," << "<VELOCITY>" << std::endl;
//  Simulator::Schedule(Seconds(0.00), &SaveUePosition, movingueNodes, &os, 0.05);

//  for (int i = 0; i < simTime / timeRes.GetDouble(); i++)
//    {
//      Simulator::Schedule (timeRes * i, &RecordMovement, rxMob);
//    }

  if(enableInstTput)
    {
      std::ostringstream oss1, oss2, oss3;
      oss1 << "/NodeList/" << ueNodes.Get (0)->GetId () << "/ApplicationList/0/$ns3::PacketSink/Rx";
      oss2<< "/NodeList/" << ueNodes.Get (1)->GetId () << "/ApplicationList/0/$ns3::PacketSink/Rx";
      oss3 << "/NodeList/" << ueNodes.Get (2)->GetId () << "/ApplicationList/0/$ns3::PacketSink/Rx";

      Config::ConnectWithoutContext (oss1.str (), MakeCallback (&ReceivePacket1));
      Config::ConnectWithoutContext (oss2.str (), MakeCallback (&ReceivePacket2));
      Config::ConnectWithoutContext (oss3.str (), MakeCallback (&ReceivePacket3));


      std::cout << "#";
      std::cout << std::setw(9) << "time";
      std::cout << std::setw(11) << "Throughput-UE1 (Mbps)" << std::setw(11) << "Throughput-UE2 (Mbps)" << std::setw(11) << "Throughput-UE3 (Mbps)" << std::endl;
      Time binSize = Seconds (0.04);
      Simulator::Schedule (Seconds (0.1), &Throughput, binSize);
    }

  Simulator::Stop (Seconds (simTime));
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
          //      std::cout << "   Packet Lost: " << i->second.lostPackets << "\n";

          if (i->second.rxPackets > 0)
            {
              // Measure the duration of the flow from receiver's perspective
              //double rxDuration = i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ();
              double rxDuration = simTime - startTime.GetSeconds();

              averageFlowThroughput += (((i->second.rxBytes * 8.0) / rxDuration) / 1000) / 1000;
              averageFlowDelay += 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets;

              std::cout  << "  Throughput: " << (((i->second.rxBytes * 8.0) / rxDuration) / 1000) / 1000  << " Mbps\n";
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

  Simulator::Destroy ();
  return 0;
}
