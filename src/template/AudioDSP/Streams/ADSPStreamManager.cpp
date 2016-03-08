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



#include "KodiThreads/threads/SingleLock.h"

#include "ADSPStreamManager.hpp"
#include "AudioDSP/FactoryADSPModes/FactoryADSPModes.hpp"
#include "include/client.h"

using namespace ADDON;


CADSPStreamManager::CADSPStreamManager()
{
  m_ADSPStreamBuilder = &m_ADSPStreamBuilderAll;
  m_HasProcesses = 0;

  CFactoryADSPModes::ADSPModeInfoVector_t adspModes;
  AE_DSP_ERROR err = CFactoryADSPModes::GetAvailableModes(adspModes);

  if (adspModes.size() <= 0)
  {
    // TODO: use ADDON Exception
    throw std::exception();
  }
    
  KODI->Log(LOG_DEBUG, "Available ADSP Modes:");

  for (int ii = 0; ii < adspModes.size(); ii++)
  {
    KODI->Log(LOG_DEBUG, "Name: %s, Internal ID: %i, ADSP Type: %i", adspModes[ii].ModeName.c_str(), adspModes[ii].ModeInfo.ModeID, adspModes[ii].ModeInfo.ModeType);
    if (adspModes[ii].ModeInfo.ModeType >= 0)
    {
      m_HasProcesses |= 1 << adspModes[ii].ModeInfo.ModeType;
    }
    else
    {
      // TODO: Log Warning!
    }

    AE_DSP_MODES::AE_DSP_MODE adspModeSettings;
    AE_DSP_ERROR err = CFactoryADSPModes::GetADSPModeSettings(adspModes[ii].ModeInfo.ModeID, adspModes[ii].ModeInfo.ModeType, adspModeSettings);
    if (err == AE_DSP_ERROR_NO_ERROR)
    {
      KODI->Log(LOG_DEBUG, "%s, Try to register ADSP Mode: %s", __FUNCTION__, adspModes[ii].ModeName.c_str());
      ADSP->RegisterMode(&adspModeSettings);
    }
    else
    {
      KODI->Log(LOG_ERROR, "%s, Failed to retrieve ADSP Mode settings for %s", __FUNCTION__, adspModes[ii].ModeName.c_str());
    }
  }

  // set all streams to NULL
  m_ADSPStreams[0] = NULL;
  m_ADSPStreams[1] = NULL;
  m_ADSPStreams[2] = NULL;
  m_ADSPStreams[3] = NULL;
  m_ADSPStreams[4] = NULL;
  m_ADSPStreams[5] = NULL;
  m_ADSPStreams[6] = NULL;
  m_ADSPStreams[7] = NULL;

  //memset(m_ADSPStreams, 0, sizeof(CADSPStream)*AE_DSP_STREAM_MAX_STREAMS);
}


CADSPStreamManager::~CADSPStreamManager()
{
  for (int ii = 0; ii < AE_DSP_STREAM_MAX_STREAMS; ii++)
  {
    StreamDestroy(ii);
  }
}

AE_DSP_ERROR CADSPStreamManager::StreamIsModeSupported(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType, unsigned int ModeID, int UniqueDBModeID)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return AE_DSP_ERROR_UNKNOWN;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return AE_DSP_ERROR_UNKNOWN;
  }

  return m_ADSPStreams[streamID]->StreamIsModeSupported(ModeType, ModeID, UniqueDBModeID);
}

unsigned int CADSPStreamManager::ProcessMode(const ADDON_HANDLE Handle, unsigned int ModeID, float **Array_in, float **Array_out, unsigned int Samples)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return 0;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->ProcessMode(ModeID, Array_in, Array_out, Samples);
}

unsigned int CADSPStreamManager::ProcessMode(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType, float **Array_in, float **Array_out, unsigned int Samples)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return 0;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->ProcessMode(ModeType, Array_in, Array_out, Samples);
}

unsigned int CADSPStreamManager::ProcessMode(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType, const float **Array_in, unsigned int Samples)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return 0;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->ProcessMode(ModeType, Array_in, Samples);
}

unsigned int CADSPStreamManager::NeededSamplesize(const ADDON_HANDLE Handle, unsigned int ModeID)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return 0;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->NeededSamplesize(ModeID);
}

unsigned int CADSPStreamManager::NeededSamplesize(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return 0;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->NeededSamplesize(ModeType);
}


float CADSPStreamManager::GetDelay(const ADDON_HANDLE Handle, unsigned int ModeID)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return -1.0;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->GetDelay(ModeID);
}

float CADSPStreamManager::GetDelay(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return -1.0;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->GetDelay(ModeType);
}

int CADSPStreamManager::MasterProcessGetOutChannels(const ADDON_HANDLE Handle, unsigned long &OutChannelFlags)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return -1;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->MasterProcessGetOutChannels(OutChannelFlags);
}

const char* CADSPStreamManager::MasterProcessGetStreamInfoString(const ADDON_HANDLE Handle)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return NULL;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return 0;
  }

  return m_ADSPStreams[streamID]->MasterProcessGetStreamInfoString();
}

AE_DSP_ERROR CADSPStreamManager::MasterProcessSetMode(const ADDON_HANDLE Handle, AE_DSP_STREAMTYPE ModeType, unsigned int ModeID, int UniqueDBModeID)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return AE_DSP_ERROR_UNKNOWN;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return AE_DSP_ERROR_UNKNOWN;
  }

  return m_ADSPStreams[streamID]->MasterProcessSetMode(ModeType, ModeID, UniqueDBModeID);
}

int CADSPStreamManager::ResamplingRate(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType)
{
  const unsigned int streamID = Handle->dataIdentifier;
  if (streamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return -1;
  }

  CSingleLock lock(m_Lock);
  if (!m_ADSPStreams[streamID])
  {
    KODI->Log(LOG_ERROR, "%s, Tried to process uncreated stream!", __FUNCTION__);
    return -1;
  }

  return m_ADSPStreams[streamID]->ResamplingRate(ModeType);
}


AE_DSP_ERROR CADSPStreamManager::StreamCreate(const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties, const ADDON_HANDLE handle)
{
  const unsigned int iStreamID = Settings->iStreamID;
  if (iStreamID >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "%s, Invalid input! StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!", __FUNCTION__);
    return AE_DSP_ERROR_UNKNOWN;
  }

  CADSPStream *adspStream = new CADSPStream;
  AE_DSP_ERROR err = m_ADSPStreamBuilder->ConstructStream(*adspStream, Settings, pProperties); //m_ADSPStreams[iStreamID]->Create(Settings, pProperties);
  if (err != AE_DSP_ERROR_NO_ERROR)
  {
    delete adspStream;

    return err;
  }

  CSingleLock lock(m_Lock);
  if (m_ADSPStreams[iStreamID])
  {
    delete m_ADSPStreams[iStreamID];
    m_ADSPStreams[iStreamID] = NULL;
  }

  m_ADSPStreams[iStreamID]  = adspStream;
  handle->dataIdentifier    = iStreamID;
  handle->dataAddress       = NULL;
  handle->callerAddress     = m_ADSPStreams[iStreamID];

  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStreamManager::StreamInitialize(const ADDON_HANDLE handle, const AE_DSP_SETTINGS *Settings)
{
  KODI->Log(LOG_DEBUG, "Called %s.", __FUNCTION__);
  
  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CADSPStreamManager::StreamDestroy(unsigned int Id)
{
  if (Id >= AE_DSP_STREAM_MAX_STREAMS)
  {
    KODI->Log(LOG_ERROR, "StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!");
    return AE_DSP_ERROR_UNKNOWN;
  }

  if (Id >= AE_DSP_STREAM_MAX_STREAMS)
  {
    return AE_DSP_ERROR_INVALID_PARAMETERS;
  }

  CSingleLock lock(m_Lock);
  if (m_ADSPStreams[Id])
  {
    m_ADSPStreams[Id]->Destroy();
    
    delete m_ADSPStreams[Id];
    m_ADSPStreams[Id] = NULL;
  }

  return AE_DSP_ERROR_NO_ERROR;
}


bool CADSPStreamManager::SupportsInputProcess()
{
  return (bool)(m_HasProcesses & (1 << AE_DSP_MODE_TYPE_MAX));
}

bool CADSPStreamManager::SupportsPreProcess()
{
  return (bool)(m_HasProcesses & (1 << AE_DSP_MODE_TYPE_PRE_PROCESS));
}

bool CADSPStreamManager::SupportsMasterProcess()
{
  return (bool)(m_HasProcesses & (1 << AE_DSP_MODE_TYPE_MASTER_PROCESS));
}

bool CADSPStreamManager::SupportsPostProcess()
{
  return (bool)(m_HasProcesses & (1 << AE_DSP_MODE_TYPE_POST_PROCESS));
}

bool CADSPStreamManager::SupportsInputResample()
{
  return (bool)(m_HasProcesses & (1 << AE_DSP_MODE_TYPE_INPUT_RESAMPLE));
}

bool CADSPStreamManager::SupportsOutputResample()
{
  return (bool)(m_HasProcesses & (1 << AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE));
}
