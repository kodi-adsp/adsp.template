#include "PreProcessCpy.hpp"

#include "ADSPHelpers.h"
#include "include/client.h"

using namespace ADDON;


CPreProcessingCpy::CPreProcessingCpy()
{
}


CPreProcessingCpy::~CPreProcessingCpy()
{
}


AE_DSP_ERROR CPreProcessingCpy::ModeCreate(const AE_DSP_SETTINGS &Settings, const AE_DSP_STREAM_PROPERTIES &Properties)
{
  m_InChannels            = Settings.iInChannels;
  m_InChannelPresentFlags = Settings.lInChannelPresentFlags;

  // reset used channel mapping array
  for (int ch = 0; ch < AE_DSP_CH_MAX; ch++)
  {
    m_ChannelMappingIdx[ch] = -1;
    m_ChannelMappingFlags[ch] = 0;
  }

  // add used channels to channel mapping array
  int lastAudioChannel = 0;
  for (int ch = 0; ch < m_InChannels; ch++)
  {
    unsigned long tempChannelFlag = 1 << lastAudioChannel;
    m_ChannelMappingIdx[ch] = CADSPHelpers::GetNextChID(m_InChannelPresentFlags,
                                                        CADSPHelpers::Translate_ChFlag_TO_ChID((AE_DSP_CHANNEL_PRESENT)tempChannelFlag));
    m_ChannelMappingFlags[ch] = CADSPHelpers::Translate_ChID_TO_ChFlag((AE_DSP_CHANNEL)m_ChannelMappingIdx[ch]);
    if (m_ChannelMappingIdx[ch] == AE_DSP_CH_INVALID)
    {
      KODI->Log(LOG_ERROR, "%s line %i: Tried to create invalid channel mappings!", __FUNCTION__, __LINE__);
      return AE_DSP_ERROR_FAILED;
    }
    lastAudioChannel = m_ChannelMappingIdx[ch] + 1;
  }
  

  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CPreProcessingCpy::ModeDestroy()
{
  return AE_DSP_ERROR_NO_ERROR;
}


// Requiered Processing Methods
unsigned int CPreProcessingCpy::ModeProcess(float **ArrayIn, float **ArrayOut, unsigned int Samples)
{
  for (int ch = 0; ch < m_InChannels; ch++)
  { 
    memcpy(ArrayOut[m_ChannelMappingIdx[ch]], ArrayIn[m_ChannelMappingIdx[ch]], sizeof(float)*Samples);
  }

  return Samples;
}
