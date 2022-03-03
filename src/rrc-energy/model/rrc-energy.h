/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef RRC_ENERGY_H
#define RRC_ENERGY_H
#include "ns3/li-ion-energy-source.h"
#include "ns3/nstime.h"
namespace ns3 {

/* ... */

class RrcEnergy
{
public:
	static TypeId GetTypeId (void);
	void SetNode (Ptr<Node> node);
	void SetEnergySource (Ptr<EnergySource> source);
    /**
     * Callback type for energy depletion handling.
     */
    typedef Callback<void> RrcEnergyDepletionCallback;

    /**
     * Callback type for energy recharged handling.
     */
    typedef Callback<void> RrcEnergyRechargedCallback;
    LiIonEnergySource LIES;
    RrcEnergy();
    ~RrcEnergy();
    int64_t m_time_last_tx; //Time since a transmission is observed
	int64_t m_time_last_rx; //Time since a reception is observed
	double m_total;

	//Only downlink is enabled
	void Only_downlink_rx(double idle_time_units);
	//Only uplink is enabled
	void Only_uplink_tx(double idle_time_units);
	//Decrease if the device is idle all time
	void Only_idle_decrease(double idle_time_units);
	//Decrease if the device is used simultaneous both uplink and downlink
	void both_downlink_and_uplink(double idle_time_units);

	void Only_deepsleep_decrease(double idle_time_units);
	void Only_drx_decrease(double idle_time_units);
	void Only_paging_decrease(double idle_time_units);
	void SetEnergyDepletionCallback (RrcEnergyDepletionCallback callback);

    /**
     * \param callback Callback function.
     *
     * Sets callback for energy recharged handling.
     */
    void SetEnergyRechargedCallback (RrcEnergyRechargedCallback callback);
	void rrcenergystatechange(int32_t oldstate, int32_t newstate);

private:
    RrcEnergyRechargedCallback m_energyRechargedCallback;
    RrcEnergyDepletionCallback m_energyDepletionCallback;	
	Ptr<Node> m_node;
	Ptr<EnergySource> m_source;
	
	TracedValue<double> m_totalEnergyConsumption;
    
};
}

#endif /* RRC_ENERGY_H */

