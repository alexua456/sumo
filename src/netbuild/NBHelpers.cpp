/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    NBHelpers.cpp
/// @author  Daniel Krajzewicz
/// @author  Sascha Krieg
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Tue, 20 Nov 2001
/// @version $Id$
///
// Some mathematical helper methods
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
//#include <iomanip>
#include <utils/common/StringUtils.h>
#include <utils/common/StringTokenizer.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/TplConvert.h>
#include <utils/geom/Position.h>
#include <utils/geom/GeomHelper.h>
#include "NBNode.h"
#include "NBHelpers.h"


// ===========================================================================
// method definitions
// ===========================================================================
double
NBHelpers::relAngle(double angle1, double angle2) {
    angle2 -= angle1;
    if (angle2 > 180) {
        angle2 = (360 - angle2) * -1;
    }
    while (angle2 < -180) {
        angle2 = 360 + angle2;
    }
    return angle2;
}


double
NBHelpers::normRelAngle(double angle1, double angle2) {
    double rel = relAngle(angle1, angle2);
    if (rel + NUMERICAL_EPS >= 180) {
        return -180;
    } else {
        return rel;
    }
}


std::string
NBHelpers::normalIDRepresentation(const std::string& id) {
    std::stringstream strm1(id);
    long numid;
    strm1 >> numid;
    std::stringstream strm2;
    strm2 << numid;
    return strm2.str();
}


double
NBHelpers::distance(NBNode* node1, NBNode* node2) {
    return node1->getPosition().distanceTo(node2->getPosition());
}


void
NBHelpers::loadEdgesFromFile(const std::string& file, std::set<std::string>& into) {
    std::ifstream strm(file.c_str());
    if (!strm.good()) {
        throw ProcessError("Could not load names of edges too keep from '" + file + "'.");
    }
    while (strm.good()) {
        std::string name;
        strm >> name;
        into.insert(name);
        // maybe we're loading an edge-selection
        if (StringUtils::startsWith(name, "edge:")) {
            into.insert(name.substr(5));
        }
    }
}


void
NBHelpers::loadPrefixedIDsFomFile(const std::string& file, const std::string prefix, std::set<std::string>& into) {
    std::ifstream strm(file.c_str());
    if (!strm.good()) {
        throw ProcessError("Could not load IDs from '" + file + "'.");
    }
    while (strm.good()) {
        std::string prefixedID;
        strm >> prefixedID;
        if (StringUtils::startsWith(prefixedID, prefix)) {
            into.insert(prefixedID.substr(prefix.size()));
        }
    }
}

void
NBHelpers::interpretLaneID(const std::string& lane_id, std::string& edge_id, int& index) {
    // assume lane_id = edge_id + '_' + index
    const std::string::size_type sep_index = lane_id.rfind('_');
    if (sep_index == std::string::npos) {
        WRITE_ERROR("Invalid lane id '" + lane_id + "' (missing '_').");
    }
    edge_id = lane_id.substr(0, sep_index);
    std::string index_string = lane_id.substr(sep_index + 1);
    try {
        index = TplConvert::_2int(index_string.c_str());
    } catch (NumberFormatException&) {
        WRITE_ERROR("Invalid lane index '" + index_string + "' for lane '" + lane_id + "'.");
    }
}

/****************************************************************************/
