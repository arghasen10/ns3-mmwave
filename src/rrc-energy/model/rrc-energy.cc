/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "rrc-energy.h"

#include "ns3/log.h"
#include "ns3/li-ion-energy-source.h"
#include <ns3/simulator.h>
#include <ns3/nstime.h>
#include <iomanip>
#include <limits>
#include "ns3/log.h"
#include "ns3/config-store-module.h"
#include "ns3/config.h"
namespace ns3 {

static const std::string g_ueRrcStateName[17] =
{
  "IDLE_START",
  "IDLE_CELL_SEARCH",
  "IDLE_WAIT_MIB_SIB1",
  "IDLE_WAIT_MIB",
  "IDLE_WAIT_SIB1",
  "IDLE_CAMPED_NORMALLY",
  "IDLE_WAIT_SIB2",
  "IDLE_PAGING",
  "IDLE_RANDOM_ACCESS",
  "IDLE_CONNECTING",
  "CONNECTED_NORMALLY",
  "CONNECTED_HANDOVER",
  "CONNECTED_PHY_PROBLEM",
  "CONNECTED_REESTABLISHING",
  "RRC_INACTIVE",
  "INACTIVE_PAGING",
  "IDLE_DS"
};
/* ... */
double idleTime = 0;
Time m_lastUpdatedTime ;


TypeId
RrcEnergy::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::RrcEnergy")
        .SetParent<DeviceEnergyModel> ()
        .AddConstructor<RrcEnergy> ()
        .AddTraceSource ("TotalEnergyConsumption",
                        "Total energy consumption of the radio device.",
                        MakeTraceSourceAccessor (&RrcEnergy::m_totalEnergyConsumption),
                        "ns3::TracedValueCallback::Double")
        
  ;
  return tid;
}


RrcEnergy::RrcEnergy()
:m_time_last_tx(0),
m_time_last_rx(0),
m_total(0)
{
    m_energyDepletionCallback.Nullify ();
}

RrcEnergy::~RrcEnergy()
{

}
void
RrcEnergy::SetNode (Ptr<Node> node)
{
  //NS_LOG_FUNCTION (this << node);
  NS_ASSERT (node != NULL);
  m_node = node;
}

void
RrcEnergy::SetEnergySource (Ptr<EnergySource> source)
{
    //NS_LOG_FUNCTION (this << source);
    NS_ASSERT (source != NULL);
    m_source = source;
}
void
RrcEnergy::SetEnergyDepletionCallback (
  RrcEnergyDepletionCallback callback
)
{
  //NS_LOG_FUNCTION (this);
//   if (callback.IsNull ())
//   {
//     NS_LOG_DEBUG ("RrcEnergy:Setting NULL energy depletion callback!");
//   }
  m_energyDepletionCallback = callback;
}

void
RrcEnergy::SetEnergyRechargedCallback (
  RrcEnergyRechargedCallback callback
)
{
//   NS_LOG_FUNCTION (this);
//   if (callback.IsNull ())
//   {
//     NS_LOG_DEBUG ("RrcEnergy:Setting NULL energy recharged callback!");
//   }
  m_energyRechargedCallback = callback;
}

void
RrcEnergy::Only_uplink_tx(double idletime)
{
    m_total += idletime;
    std::cout<<"Only_uplink_tx: 4 "<<idletime<<" m_total: "<< m_total<<std::endl;
	Callback<void, double> only_tx;
	only_tx = MakeCallback(&LiIonEnergySource::DecreaseRemainingEnergy, &LIES);
    double energyDecreasedIdle=1 * idletime;
	double energyDecreasedTx = 350* idletime;
	m_time_last_tx=Simulator::Now().GetMilliSeconds();

	if(m_time_last_rx==m_time_last_tx)
	{
		only_tx(energyDecreasedTx);
		both_downlink_and_uplink(idletime);

	}
	else
	{
		only_tx(energyDecreasedTx+energyDecreasedIdle);
		std::cout<<"Transmit chain is consuming power "<< (energyDecreasedTx+energyDecreasedIdle) <<" joules"<<std::endl;
	}
}

void RrcEnergy::Only_downlink_rx(double idleTime)
{
    m_total += idleTime;
    std::cout<<"Only_downlink_rx: 5 "<<idleTime<<" m_total: "<<m_total<<std::endl;
	Callback<void, double> only_tx;
	only_tx = MakeCallback(&LiIonEnergySource::DecreaseRemainingEnergy, &LIES);

    double energyDecreasedIdle=1 * idleTime;
	double energyDecreasedRx = 350*idleTime;
	if(m_time_last_rx == m_time_last_tx && m_time_last_tx != 0)
	{
	    std::cout<<"m_time_last_rx: "<<m_time_last_rx<< "m_time_last_tx "<<m_time_last_tx<<std::endl;
		only_tx(energyDecreasedRx);
		both_downlink_and_uplink(idleTime);

	}
	else
	{
		only_tx(energyDecreasedRx+energyDecreasedIdle);
		std::cout<<"Receive chain is consuming power "<< (energyDecreasedRx+energyDecreasedIdle) <<" joules"<<std::endl;
	}

	m_time_last_rx=Simulator::Now().GetMilliSeconds();


}

void RrcEnergy::Only_idle_decrease(double idleTime)
{
    m_total += idleTime;
    std::cout << std::setprecision(16) << std::fixed;
    std::cout<<"Only_idle_decrease: 3 "<<idleTime<<" m_total: "<<m_total<<std::endl;

	double energyDecreasedIdle=1 * idleTime;

    Callback<void, double> only_tx;
    only_tx = MakeCallback(&LiIonEnergySource::DecreaseRemainingEnergy, &LIES);
	only_tx(energyDecreasedIdle);
}

void RrcEnergy::Only_deepsleep_decrease(double idleTime)
{
    m_total += idleTime;
    std::cout<<"Only_deep_sleep_decrease: 1 "<<idleTime<<" m_total: "<<m_total<<std::endl;
    Callback<void, double> only_tx;
    only_tx = MakeCallback(&LiIonEnergySource::DecreaseRemainingEnergy, &LIES);
    double energyDecreaseddeepSleep = (1)*idleTime;
    only_tx(energyDecreaseddeepSleep);
}

void RrcEnergy::Only_drx_decrease(double idleTime)
{
    m_total += idleTime;
    std::cout<<"Only_drx_decrease: 2 "<<idleTime<<" m_total: "<<m_total<<std::endl;
    Callback<void, double> only_tx;
    only_tx = MakeCallback(&LiIonEnergySource::DecreaseRemainingEnergy, &LIES);

    double energyDecreasedDrx=20*idleTime;
    only_tx(energyDecreasedDrx);
}

void RrcEnergy::Only_paging_decrease(double idleTime)
{
    m_total += idleTime;
    std::cout<<"Only_paging_decrease: 7 "<<idleTime<<" m_total: "<<m_total<<std::endl;
    Callback<void, double> only_tx;
    only_tx = MakeCallback(&LiIonEnergySource::DecreaseRemainingEnergy, &LIES);

    double energyDecreasedPaging=40*idleTime;
    only_tx(energyDecreasedPaging);
}

void RrcEnergy::both_downlink_and_uplink(double idleTime)
{
    std::cout<<"both_downlink_and_uplink: 6 "<<idleTime<<" m_total: "<<m_total<<std::endl;
	Callback<void, double> only_tx;
	only_tx = MakeCallback(&LiIonEnergySource::DecreaseRemainingEnergy, &LIES);
	double energyDecreasedIdle= 350*idleTime;
	only_tx(energyDecreasedIdle);
	std::cout<<"Both Transmit and Receive chain is consuming power "<<std::endl;
}

// void RrcEnergy::rrcenergystatechange(int32_t oldstate, int32_t newstate)
// {
// 	// //add switch states
// 	// idleTime= ((Simulator::Now()-m_lastUpdatedTime).GetNanoSeconds());
// 	// RrcEnergy::Only_idle_decrease(idleTime);
    
//     // m_lastUpdatedTime=Simulator::Now();
//     // std::cout<<Simulator::Now().GetSeconds()<<" "<<GetRnti() <<" ";
//     // two(idleTime);
// 	switch (newstate)
//   {
//       case 0:
//       case 1:
//       case 2:
//       case 3:
//       case 4:
//       case 5:
//       case 6:
//       case 7:
//       case 8:
//       case 9:
//           break;
//       case 10:
// 	      idleTime= ((Simulator::Now()-m_lastUpdatedTime).GetNanoSeconds());
// 		  m_lastUpdatedTime=Simulator::Now();
//           Only_idle_decrease(idleTime);
//           break;
//       case 11:
//       case 12:
//       case 13:
//           break;
//       case 14:
// 	      idleTime= ((Simulator::Now()-m_lastUpdatedTime).GetNanoSeconds());
//           m_lastUpdatedTime=Simulator::Now();
//           Only_paging_decrease(idleTime);
//           break;

//       case 15:
          
//           idleTime= ((Simulator::Now()-m_lastUpdatedTime).GetNanoSeconds());
//           m_lastUpdatedTime=Simulator::Now();
// 		  Only_drx_decrease(idleTime);
//           break;

//       case 16:
          
//           idleTime= ((Simulator::Now()-m_lastUpdatedTime).GetNanoSeconds());
//           m_lastUpdatedTime=Simulator::Now();
//           Only_deepsleep_decrease(idleTime);
//           break;

//       default:
//           break;
//     }

// }

}

