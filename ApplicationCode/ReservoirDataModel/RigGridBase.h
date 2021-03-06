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

#include "cvfVector3.h"

#include "cvfStructGrid.h"
#include "cvfStructGridGeometryGenerator.h"

#include <vector>
#include <string>
#include "cvfStructGridScalarDataAccess.h"
#include "RifReaderInterface.h"


class RigMainGrid;
class RigCell;
class RigGridScalarDataAccess;

class RigGridBase : public cvf::StructGridInterface
{
public:
    RigGridBase(RigMainGrid* mainGrid);
    virtual ~RigGridBase(void);

    void                        setGridPointDimensions(const cvf::Vec3st& gridDimensions)   { m_gridPointDimensions = gridDimensions;}
    cvf::Vec3st                 gridPointDimensions()                                       { return m_gridPointDimensions; }

    size_t                      cellCount() const { return cellCountI() * cellCountJ() * cellCountK(); }
    RigCell&                    cell(size_t gridCellIndex);
    const RigCell&              cell(size_t gridCellIndex) const;
    
    void                        setIndexToStartOfCells(size_t indexToStartOfCells) { m_indexToStartOfCells = indexToStartOfCells; }
    void                        setGridIndex(size_t index) { m_gridIndex = index; }
    size_t                      gridIndex() { return m_gridIndex; }

    double                      characteristicCellSize();

    std::string                 gridName() const;
    void                        setGridName(const std::string& gridName);

    void                        computeFaults();
    bool                        isMainGrid() const;
    RigMainGrid*                mainGrid() const { return m_mainGrid; }

    size_t                      matrixModelActiveCellCount() const;
    void                        setMatrixModelActiveCellCount(size_t activeMatrixModelCellCount);
    size_t                      fractureModelActiveCellCount() const ;
    void                        setFractureModelActiveCellCount(size_t activeFractureModelCellCount);

protected:
    friend class RigMainGrid;//::initAllSubGridsParentGridPointer();
    void                        initSubGridParentPointer();
    void                        initSubCellsMainGridCellIndex();

    // Interface implementation
public:
    virtual size_t gridPointCountI() const;
    virtual size_t gridPointCountJ() const;
    virtual size_t gridPointCountK() const;

    virtual cvf::Vec3d minCoordinate() const;
    virtual cvf::Vec3d maxCoordinate() const;
    virtual cvf::Vec3d displayModelOffset() const;

    virtual size_t cellIndexFromIJK( size_t i, size_t j, size_t k ) const;
    virtual bool ijkFromCellIndex( size_t cellIndex, size_t* i, size_t* j, size_t* k ) const;

    virtual bool cellIJKFromCoordinate( const cvf::Vec3d& coord, size_t* i, size_t* j, size_t* k ) const;
    virtual void cellCornerVertices( size_t cellIndex, cvf::Vec3d vertices[8] ) const;
    virtual cvf::Vec3d cellCentroid( size_t cellIndex ) const;

    virtual void cellMinMaxCordinates( size_t cellIndex, cvf::Vec3d* minCoordinate, cvf::Vec3d* maxCoordinate ) const;

    virtual size_t gridPointIndexFromIJK( size_t i, size_t j, size_t k ) const;
    virtual cvf::Vec3d gridPointCoordinate( size_t i, size_t j, size_t k ) const;

    virtual bool isCellActive( size_t i, size_t j, size_t k ) const;
    virtual bool isCellValid( size_t i, size_t j, size_t k ) const;
    virtual bool cellIJKNeighbor(size_t i, size_t j, size_t k, FaceType face, size_t* neighborCellIndex ) const;

    cvf::ref<RigGridScalarDataAccess> dataAccessObject(RifReaderInterface::PorosityModelResultType porosityModel, size_t timeStepIndex, size_t scalarSetIndex) const;

private:
    std::string                 m_gridName;
    cvf::Vec3st                 m_gridPointDimensions;
    size_t                      m_indexToStartOfCells; ///< Index into the global cell array stored in main-grid where this grids cells starts.
    size_t                      m_gridIndex; ///< The LGR index of this grid. Starts with 1. Main grid has index 0.
    RigMainGrid*                m_mainGrid;

    size_t                      m_matrixModelActiveCellCount;
    size_t                      m_fractureModelActiveCellCount;

};


class RigGridCellFaceVisibilityFilter : public cvf::CellFaceVisibilityFilter
{
public:
    RigGridCellFaceVisibilityFilter(const RigGridBase* grid)
        :   m_grid(grid),
            m_showFaultFaces(true),
            m_showExternalFaces(true)
    {
    }

    virtual bool isFaceVisible( size_t i, size_t j, size_t k, cvf::StructGridInterface::FaceType face, const cvf::UByteArray* cellVisibility ) const;

public:
    bool m_showFaultFaces;
    bool m_showExternalFaces;

private:
    const RigGridBase* m_grid;
};
