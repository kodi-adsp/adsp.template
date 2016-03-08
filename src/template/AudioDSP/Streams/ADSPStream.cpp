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



#include "ADSPStream.hpp"
#include "AudioDSP/FactoryADSPModes/FactoryADSPModes.hpp"


CADSPStream::CADSPStream()
{
}


CADSPStream::~CADSPStream()
{
}


AE_DSP_ERROR CADSPStream::Create(const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties)
{
  CFactoryADSPModes::ADSPModeInfoVector_t adspModes;
  CFactoryADSPModes::GetAvailableModes(adspModes);

  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStream::Destroy()
{
  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStream::StreamIsModeSupported(AE_DSP_MODE_TYPE ModeType, unsigned int ModeID, int UniqueDBModeID)
{
  return AE_DSP_ERROR_NO_ERROR;
}


unsigned int CADSPStream::ProcessMode(unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
  return Samples;
}

unsigned int CADSPStream::ProcessMode(AE_DSP_MODE_TYPE ModeType, float **Array_in, float **Array_out, unsigned int Samples)
{
  return Samples;
}

unsigned int CADSPStream::ProcessMode(AE_DSP_MODE_TYPE ModeType, const float **Array_in, unsigned int Samples)
{
  return Samples;
}

unsigned int CADSPStream::NeededSamplesize(unsigned int ModeID)
{
  return 0;
}

unsigned int CADSPStream::NeededSamplesize(AE_DSP_MODE_TYPE ModeType)
{
  return 0;
}

float CADSPStream::GetDelay(unsigned int ModeID)
{
  return 0.0f;
}

float CADSPStream::GetDelay(AE_DSP_MODE_TYPE ModeType)
{
  return 0.0f;
}


// Optional Methods for Master Modes
int CADSPStream::MasterProcessGetOutChannels(unsigned long &OutChannelFlags)
{
  return 0;
}

const char* CADSPStream::MasterProcessGetStreamInfoString()
{
  return NULL;
}

AE_DSP_ERROR CADSPStream::MasterProcessSetMode(AE_DSP_STREAMTYPE StreamType, unsigned int ModeID, int UniqueDBModeID)
{
  return AE_DSP_ERROR_NO_ERROR;
}

// Optional Resampling Methods
int CADSPStream::ResamplingRate(AE_DSP_MODE_TYPE ModeType)
{
  return 0;
}
