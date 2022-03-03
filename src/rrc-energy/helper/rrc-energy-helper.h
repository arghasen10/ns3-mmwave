/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef RRC_ENERGY_HELPER_H
#define RRC_ENERGY_HELPER_H

#include "ns3/rrc-energy.h"
#include "ns3/energy-model-helper.h"

namespace ns3 {

/* ... */

class RrcEnergyHelper : public DeviceEnergyModelHelper
{
public:
  /**
   * Construct a helper which is used to add a radio energy model to a node
   */
  RrcEnergyHelper ();

  /**
   * Destroy a RadioEnergy Helper
   */
  ~RrcEnergyHelper ();



  /**
   * \param callback Callback function for energy depletion handling.
   *
   * Sets the callback to be invoked when energy is depleted.
   */
  void SetDepletionCallback (RrcEnergy::RrcEnergyDepletionCallback callback);

  /**
   * \param callback Callback function for energy recharged handling.
   *
   * Sets the callback to be invoked when energy is recharged.
   */
  void SetRechargedCallback (RrcEnergy::RrcEnergyRechargedCallback callback);

  

private:
  /**
   * \param device Pointer to the NetDevice to install DeviceEnergyModel.
   * \param source Pointer to EnergySource to install.
   * \returns Ptr<DeviceEnergyModel>
   *
   * Implements DeviceEnergyModel::Install.
   */
  Ptr<DeviceEnergyModel> DoInstall (Ptr<NetDevice> device,
                                            Ptr<EnergySource> source) const;

private:
  ObjectFactory m_radioEnergy; ///< radio energy
  RrcEnergy::RrcEnergyDepletionCallback m_depletionCallback; ///< radio energy depletion callback
  RrcEnergy::RrcEnergyRechargedCallback m_rechargedCallback; ///< radio energy recharged callback

};                                     

}

#endif /* RRC_ENERGY_HELPER_H */

