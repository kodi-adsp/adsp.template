/*
 *      Copyright (C) 2005-2016 Team Kodi
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */


#include "FactoryADSPModes.hpp"
#include <string>

using namespace std;


// static member variables
CFactoryADSPModes::ADSPModeNameMap_t CFactoryADSPModes::m_ModeNameMappingTable;

AE_DSP_ERROR CFactoryADSPModes::Create(int ModeID, AE_DSP_MODE_TYPE ModeType, IADSPMode *&InterfacePtr)
{
  ADSPModeKey_t ADSPModeKey(ModeID, ModeType);
  ADSPModeMap_t::iterator iter = GetADSPModeMap().find(ADSPModeKey);
  if (iter == GetADSPModeMap().end())
  {
    InterfacePtr = NULL;
    return AE_DSP_ERROR_INVALID_PARAMETERS;
  }

  InterfacePtr = (*iter).second.CreateADSPMode();
  InterfacePtr->m_ModeID   = ModeID;
  InterfacePtr->m_ModeType = ModeType;
  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CFactoryADSPModes::Destroy(IADSPMode *&ADSPMode)
{
  if (ADSPMode)
  {
    delete ADSPMode;
    ADSPMode = NULL;
  }

  return AE_DSP_ERROR_NO_ERROR;
}


int CFactoryADSPModes::RegisterADSPMode(const std::string ModeName, AE_DSP_MODE_TYPE ModeType, ADSPModeCallbacks_t Callbacks)
{
  static int modeID=0;
  ADSPModeKey_t ADSPModeKey(modeID, ModeType);
  ADSPModeMap_t::iterator iter = GetADSPModeMap().find(ADSPModeKey);
  if (iter == GetADSPModeMap().end())
  {// only register not yet known products
    GetADSPModeMap()[ADSPModeKey] = Callbacks;

    const AE_DSP_MODES::AE_DSP_MODE& modeSettings = Callbacks.GetADSPModeSettings();
    ((AE_DSP_MODES::AE_DSP_MODE*)&modeSettings)->iUniqueDBModeId  = -1;
    ((AE_DSP_MODES::AE_DSP_MODE*)&modeSettings)->iModeType = ModeType;
    ((AE_DSP_MODES::AE_DSP_MODE*)&modeSettings)->iModeNumber      = modeID;
  }

  m_ModeNameMappingTable[ModeName] = ADSPModeKey;

  return modeID++;
}


AE_DSP_ERROR CFactoryADSPModes::GetAvailableModes(ADSPModeInfoVector_t &ModeInfos)
{
  ModeInfos.clear();

  if (m_ModeNameMappingTable.size() <= 0)
  {
    return AE_DSP_ERROR_FAILED;
  }

  for (ADSPModeNameMap_t::iterator iter = m_ModeNameMappingTable.begin(); iter != m_ModeNameMappingTable.end(); ++iter)
  {
    ADSPModeInfos modeInfo;
    modeInfo.ModeName           = iter->first;
    modeInfo.ModeInfo.ModeID    = iter->second.ModeID;
    modeInfo.ModeInfo.ModeType  = iter->second.ModeType;

    ModeInfos.push_back(modeInfo);
  }

  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CFactoryADSPModes::GetADSPModeSettings(int ModeID, AE_DSP_MODE_TYPE ModeType, AE_DSP_MODES::AE_DSP_MODE &ModeSettings)
{
  ADSPModeKey_t ADSPModeKey(ModeID, ModeType);
  ADSPModeMap_t::iterator iter = GetADSPModeMap().find(ADSPModeKey);
  if (iter == GetADSPModeMap().end())
  {
    return AE_DSP_ERROR_INVALID_PARAMETERS;
  }

  const AE_DSP_MODES::AE_DSP_MODE& modeSettings = (*iter).second.GetADSPModeSettings();
  memcpy(&ModeSettings, &modeSettings, sizeof(AE_DSP_MODES::AE_DSP_MODE));

  return AE_DSP_ERROR_NO_ERROR;
}


int CFactoryADSPModes::GetActiveADSPMode(std::string &ModeName)
{
  ADSPModeNameMap_t::iterator iterModeKey = m_ModeNameMappingTable.find(ModeName);
  if (iterModeKey == m_ModeNameMappingTable.end())
  {
    return -1;
  }

  ADSPModeMap_t::iterator iterCB = GetADSPModeMap().find((*iterModeKey).second);
  if (iterCB == GetADSPModeMap().end())
  {
    return -1;
  }

  return (*iterCB).second.GetActiveADSPMode();
}


int CFactoryADSPModes::GetCreatedADSPMode(std::string &ModeName)
{
  ADSPModeNameMap_t::iterator iterModeKey = m_ModeNameMappingTable.find(ModeName);
  if (iterModeKey == m_ModeNameMappingTable.end())
  {
    return -1;
  }

  ADSPModeMap_t::iterator iterCB = GetADSPModeMap().find((*iterModeKey).second);
  if (iterCB == GetADSPModeMap().end())
  {
    return -1;
  }

  return (*iterCB).second.GetCreatedADSPMode();
}

int CFactoryADSPModes::GetDestroyedADSPMode(std::string &ModeName)
{
  ADSPModeNameMap_t::iterator iterModeKey = m_ModeNameMappingTable.find(ModeName);
  if (iterModeKey == m_ModeNameMappingTable.end())
  {
    return -1;
  }

  ADSPModeMap_t::iterator iterCB = GetADSPModeMap().find((*iterModeKey).second);
  if (iterCB == GetADSPModeMap().end())
  {
    return -1;
  }

  return (*iterCB).second.GetDestroyedADSPMode();
}
