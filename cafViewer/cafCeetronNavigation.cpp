//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2012 Ceetron AS
//    
//   This library is free software: you can redistribute it and/or modify 
//   it under the terms of the GNU General Public License as published by 
//   the Free Software Foundation, either version 3 of the License, or 
//   (at your option) any later version. 
//    
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY 
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
//   FITNESS FOR A PARTICULAR PURPOSE.   
//    
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>> 
//   for more details. 
//
//##################################################################################################

#include "cafCeetronNavigation.h"
#include "cafViewer.h"
#include "cvfCamera.h"
#include "cvfScene.h"
#include "cvfModel.h"
#include "cvfViewport.h"
#include "cvfHitItemCollection.h"
#include "cvfRay.h"

#include <QInputEvent>

using cvf::ManipulatorTrackball;

//==================================================================================================
///
/// \class CeetronNavigationPolicy
/// \ingroup Caf
///
/// 
///
//==================================================================================================

//--------------------------------------------------------------------------------------------------
/// Repositions and orients the camera to view the rotation point along the 
/// direction "alongDirection". The distance to the rotation point is maintained.
///
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::setView( const cvf::Vec3d& alongDirection, const cvf::Vec3d& upDirection )
{
    m_trackball->setView(alongDirection, upDirection);
    /*
    if (m_camera.isNull()) return;

    Vec3d dir = alongDirection;
    if (!dir.normalize()) return;
    Vec3d up = upDirection;
    if(!up.normalize()) up = Vec3d::Z_AXIS;

    if((up * dir) < 1e-2) up = dir.perpendicularVector();

    Vec3d cToE = m_camera->position() - m_rotationPoint;
    Vec3d newEye = m_rotationPoint - cToE.length() * dir;

    m_camera->setFromLookAt(newEye, m_rotationPoint, upDirection);
    */
}



//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::init()
{
    m_trackball = new cvf::ManipulatorTrackball;
    m_trackball->setCamera(m_viewer->mainCamera());
    m_isRotCenterInitialized = false;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool caf::CeetronNavigation::handleInputEvent(QInputEvent* inputEvent)
{
    if (! inputEvent) return false;

    switch (inputEvent->type())
    {
    case QEvent::MouseButtonPress:
        mouseMoveEvent(static_cast<QMouseEvent*>( inputEvent));
        break;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>( inputEvent));
        break;
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent*>( inputEvent));
        break;
    case QEvent::Wheel:
        wheelEvent(static_cast<QWheelEvent*> ( inputEvent));
        break;
    }

    return false;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_viewer->canRender()) return;
    initializeRotationCenter();
    int posX = event->x();
    int posY = event->y();

    ManipulatorTrackball::NavigationType navType = getNavigationTypeFromMouseButtons(event->buttons());
    if (navType != m_trackball->activeNavigation())
    {
        m_trackball->startNavigation(navType, posX, posY);
    }

    bool needRedraw = m_trackball->updateNavigation(posX, posY);
    if (needRedraw)
    {
        m_viewer->update();
    }

    setCursorFromCurrentState();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::mousePressEvent(QMouseEvent* event)
{
    if (!m_viewer->canRender()) return;
    initializeRotationCenter();
    int posX = event->x();
    int posY = event->y();

    ManipulatorTrackball::NavigationType navType = getNavigationTypeFromMouseButtons(event->buttons());
    m_trackball->startNavigation(navType, posX, posY);

    setCursorFromCurrentState();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::mouseReleaseEvent(QMouseEvent* event)
{

    if (!m_viewer->canRender()) return;
    initializeRotationCenter();
    ManipulatorTrackball::NavigationType navType = getNavigationTypeFromMouseButtons(event->buttons());
    m_trackball->startNavigation(navType, event->x(), event->y());

    setCursorFromCurrentState();
}



//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::wheelEvent(QWheelEvent* event)
{
    if (!m_viewer->canRender()) return;
    if (!m_viewer->mainCamera()) return;
    initializeRotationCenter();
    int vpHeight = m_viewer->mainCamera()->viewport()->height();
    if (vpHeight <= 0) return;

    int navDelta = vpHeight/5;
    if (event->delta() < 0) navDelta *= -1;

    if (m_viewer->mainCamera()->projection() == cvf::Camera::PERSPECTIVE)
    {
        m_trackball->startNavigation(ManipulatorTrackball::WALK, event->x(), event->y());
    }
    else
    {
        m_trackball->startNavigation(ManipulatorTrackball::ZOOM, event->x(), event->y());
    }

    m_trackball->updateNavigation(event->x(), event->y() - navDelta);
    m_trackball->endNavigation();

    m_viewer->update();

    event->accept();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
ManipulatorTrackball::NavigationType caf::CeetronNavigation::getNavigationTypeFromMouseButtons(Qt::MouseButtons mouseButtons)
{
    if (mouseButtons == Qt::LeftButton)
    {
        return ManipulatorTrackball::PAN;
    }
    else if (mouseButtons == Qt::RightButton)
    {
        return ManipulatorTrackball::ROTATE;
    }
    else if (mouseButtons == Qt::MidButton || mouseButtons == (Qt::LeftButton | Qt::RightButton))
    {
        if (m_viewer->mainCamera()->projection() == cvf::Camera::PERSPECTIVE)
        {
            return ManipulatorTrackball::WALK;
        }
        else
        {
            return ManipulatorTrackball::ZOOM;
        }
    }
    else
    {
        return ManipulatorTrackball::NONE;
    }
}



//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::setCursorFromCurrentState()
{
    ManipulatorTrackball::NavigationType navType = m_trackball->activeNavigation();
    switch (navType)
    {
    case ManipulatorTrackball::PAN:	     
        //m_viewer->setCursor(RICursors::get(RICursors::PAN));
        return;
    case ManipulatorTrackball::WALK:	 
        //m_viewer->setCursor(RICursors::get(RICursors::WALK));	    
        return;
    case ManipulatorTrackball::ROTATE:	 
        //m_viewer->setCursor(RICursors::get(RICursors::ROTATE));	
        return;
    default:
        break;
    }

    // m_viewer->setCursor(RICursors::get(RICursors::PICK));
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::initializeRotationCenter()
{
    if(m_isRotCenterInitialized || m_trackball.isNull() || !m_viewer->mainScene()) return;

    cvf::BoundingBox bb = m_viewer->mainScene()->boundingBox();
    if(bb.isValid())
    {
        m_pointOfInterest = bb.center();
        m_trackball->setRotationPoint(m_pointOfInterest);
        m_isRotCenterInitialized = true;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
cvf::Vec3d caf::CeetronNavigation::pointOfInterest()
{
    initializeRotationCenter();
    return m_pointOfInterest;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void caf::CeetronNavigation::setPointOfInterest(cvf::Vec3d poi)
{
    m_pointOfInterest = poi;
    m_trackball->setRotationPoint(poi);
    m_isRotCenterInitialized = true;
}

