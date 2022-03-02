/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "rrc-energy-helper.h"

#include "ns3/device-energy-model.h"
#include "ns3/energy-source.h"
#include "ns3/mmwave-ue-net-device.h"
#include "ns3/mmwave-enb-net-device.h"
#include "ns3/mc-ue-net-device.h"

namespace ns3 {

 RrcEnergyHelper::RrcEnergyHelper ()
{
    m_radioEnergy.SetTypeId ("ns3::RrcEnergy");
       // m_depletionCallback.Nullify ();
       // m_rechargedCallback.Nullify ();
}


 // func()
Ptr<DeviceEnergyModel>
RrcEnergyHelper::DoInstall(Ptr<NetDevice> device,
                                            Ptr<EnergySource> source) const
{
  
    NS_ASSERT (device != NULL);
    NS_ASSERT (source != NULL);
    std::string deviceName = device->GetInstanceTypeId ().GetName ();
    if (deviceName.compare ("ns3::MmWaveUeNetDevice") != 0)
    {

    }

    Ptr<Node> node = device->GetNode ();
    Ptr<RrcEnergy> model = m_radioEnergy.Create ()->GetObject<RrcEnergy> ();

    NS_ASSERT (model != NULL);
    model->SetNode (node);
    Ptr<mmwave::MmWaveUeNetDevice> mmwaveDevice;
    Ptr<mmwave::McUeNetDevice> mcDevice;
    Ptr<ns3::mmwave::MmWaveUePhy> mmwavePhy;
    Ptr<LteUeRrc> dev;
    
    if (deviceName == "ns3::MmWaveUeNetDevice")
    {
        mmwaveDevice = DynamicCast<mmwave::MmWaveUeNetDevice> (device);
        dev = DynamicCast<ns3::LteUeRrc> (mmwaveDevice->GetRrc());
        //mmwavePhy = mmwaveDevice->GetPhy ();
        // mcDevice->DoDispose ();
    }
    else if (deviceName == "ns3::McUeNetDevice")
    {
        // mmwaveDevice->DoDispose ();
        mcDevice = DynamicCast<ns3::mmwave::McUeNetDevice> (device);
        //mmwavePhy = mcDevice->GetMmWavePhy ();
        dev = DynamicCast<ns3::LteUeRrc> (mcDevice->GetRrc());    
    }
    
    
    dev->TraceConnectWithoutContext("State",MakeCallback (&RrcEnergy::rrcenergystatechange, model));
    source->AppendDeviceEnergyModel (model);
    model->SetEnergySource (source);

    // create trace sourse for m_state

    // Ptr<ns3::mmwave::MmWaveSpectrumPhy> mmwaveDlSpectrumPhy = mmwavePhy->GetDlSpectrumPhy();
    // Ptr<ns3::mmwave::MmWaveSpectrumPhy> mmwaveUlSpectrumPhy = mmwavePhy->GetUlSpectrumPhy ();
    // mmwaveDlSpectrumPhy->TraceConnectWithoutContext ("State", MakeCallback (&MmWaveRadioEnergyModel::ChangeStateEvent, model));
    // mmwaveUlSpectrumPhy->TraceConnectWithoutContext ("State", MakeCallback (&MmWaveRadioEnergyModel::ChangeStateEvent, model));
    // model->SetEnergyDepletionCallback (m_depletionCallback);
    // model->SetEnergyRechargedCallback (m_rechargedCallback);


    return model;
}
/* ... */


}

