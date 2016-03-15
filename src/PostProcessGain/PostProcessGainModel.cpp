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



#include "Addon/MVC/Model/TParameter.hpp"
#include "PostProcessGain/PostProcessGainModel.hpp"
#include "PostProcessGain/PostProcessGainParameterIDs.hpp"

#include "ADSPModeInfos.h"

#include "include/client.h"

using namespace ADDON;


CPostProcessGainModel::CPostProcessGainModel() :
  IModel(CADSPModeInfos::Strs[CADSPModeInfos::ADSP_MODE_ID_PORTPROCESS_GAIN],
         CADSPModeInfos::ADSP_MODE_ID_PORTPROCESS_GAIN)
{
}


CPostProcessGainModel::~CPostProcessGainModel()
{
}


int CPostProcessGainModel::Create()
{
  IModel::ParameterVector_t paramVector;

  paramVector.push_back(CreateTParameter(float, CPostProcessGainParameterIDs, POST_PROCESS_GAIN_MAIN_GAIN));

  return this->SetParameterVector(paramVector);
}

void CPostProcessGainModel::Destroy()
{
}

