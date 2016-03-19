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



#include "PostProcessGain/PostProcessGainMessages.hpp"
#include "PostProcessGain/PostProcessGain.hpp"
#include "Addon/MessageSystem/Communication/ActorProtocol.h"
#include "Addon/MessageSystem/Sockets/TSocketMemcpy.hpp"
#include "PostProcessGain/PostProcessGainParameterIDs.hpp"
#include "ADSPModeInfos.h"


CPostProcessGainMessages::CPostProcessGainMessages() :
  CMessageDispatcher(new CActorProtocol(CADSPModeInfos::Strs[CADSPModeInfos::ADSP_MODE_ID_PORTPROCESS_GAIN]),
                     CADSPModeInfos::Strs[CADSPModeInfos::ADSP_MODE_ID_PORTPROCESS_GAIN])
{
}


CPostProcessGainMessages::~CPostProcessGainMessages()
{
}


bool CPostProcessGainMessages::Create(CPostProcessGain *Mode)
{
  SocketVector_t sockets;
  
  sockets.push_back(CreateTSocketMemcpy(float, &(Mode->m_MainGain), CPostProcessGainParameterIDs, POST_PROCESS_GAIN_MAIN_GAIN));

  return this->SetSockets(sockets);
}
