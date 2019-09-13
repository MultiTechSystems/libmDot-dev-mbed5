/**********************************************************************
* COPYRIGHT 2016 MULTI-TECH SYSTEMS, INC.
*
* ALL RIGHTS RESERVED BY AND FOR THE EXCLUSIVE BENEFIT OF
* MULTI-TECH SYSTEMS, INC.
*
* MULTI-TECH SYSTEMS, INC. - CONFIDENTIAL AND PROPRIETARY
* INFORMATION AND/OR TRADE SECRET.
*
* NOTICE: ALL CODE, PROGRAM, INFORMATION, SCRIPT, INSTRUCTION,
* DATA, AND COMMENT HEREIN IS AND SHALL REMAIN THE CONFIDENTIAL
* INFORMATION AND PROPERTY OF MULTI-TECH SYSTEMS, INC.
* USE AND DISCLOSURE THEREOF, EXCEPT AS STRICTLY AUTHORIZED IN A
* WRITTEN AGREEMENT SIGNED BY MULTI-TECH SYSTEMS, INC. IS PROHIBITED.
*
***********************************************************************/

#include "ChannelPlan_AS923_Japan.h"

using namespace lora;

ChannelPlan_AS923_Japan::ChannelPlan_AS923_Japan()
:
    ChannelPlan_AS923(NULL, NULL)
{

}

ChannelPlan_AS923_Japan::ChannelPlan_AS923_Japan(Settings* settings)
:
    ChannelPlan_AS923(NULL, settings)
{

}

ChannelPlan_AS923_Japan::ChannelPlan_AS923_Japan(SxRadio* radio, Settings* settings)
:
    ChannelPlan_AS923(radio, settings)
{

}

ChannelPlan_AS923_Japan::~ChannelPlan_AS923_Japan() {

}

void ChannelPlan_AS923_Japan::Init() {
    ChannelPlan_AS923::Init();

    _plan = AS923_JAPAN;
    _planName = "AS923-JAPAN";
    _minFrequency = 920600000;
    _maxFrequency = 928000000;

    _minDatarate = 2;
    _maxDatarate = 5;

    _minRx2Datarate = DR_2;
    _maxRx2Datarate = DR_5;

    _minDatarateOffset = 2;
    _maxDatarateOffset = 5;

    GetSettings()->Session.UplinkDwelltime = 1;
    GetSettings()->Session.Max_EIRP  = 13;

    DefaultLBT();
}

void ChannelPlan_AS923_Japan::DefaultLBT() {
    _LBT_TimeUs = 5000;
    _LBT_Threshold = -80;
}

