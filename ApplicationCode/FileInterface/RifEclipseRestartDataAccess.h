/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cvfBase.h"
#include "cvfObject.h"
#include "cvfLibCore.h"

#include <QStringList>
#include <QDateTime>

#include <vector>

#include "well_info.h"

#include "RifReaderInterface.h"

//==================================================================================================
//
// Abstract class for results access
//
//==================================================================================================
class RifEclipseRestartDataAccess : public cvf::Object
{
public:
    RifEclipseRestartDataAccess();
    virtual ~RifEclipseRestartDataAccess();

    virtual bool                open(const QStringList& fileSet) = 0;
    virtual void                close() = 0;

    virtual size_t              timeStepCount() = 0;
    virtual QList<QDateTime>    timeSteps() = 0;

    virtual void                resultNames(QStringList* resultNames, std::vector<size_t>* resultDataItemCounts) = 0;
    virtual bool                results(const QString& resultName, size_t timeStep, size_t gridCount, std::vector<double>* values) = 0;

    virtual void                readWellData(well_info_type * well_info) = 0;
};
