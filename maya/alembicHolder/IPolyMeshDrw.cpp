//-*****************************************************************************
//
// Copyright (c) 2009-2011,
//  Sony Pictures Imageworks, Inc. and
//  Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Sony Pictures Imageworks, nor
// Industrial Light & Magic nor the names of their contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-*****************************************************************************

#include "IPolyMeshDrw.h"
#include <Alembic/AbcGeom/Visibility.h>

namespace SimpleAbcViewer {

    static MGLFunctionTable *gGLFT = NULL;

//-*****************************************************************************
IPolyMeshDrw::IPolyMeshDrw( IPolyMesh &iPmesh, std::vector<std::string> path )
  : IObjectDrw( iPmesh, false, path )
  , m_polyMesh( iPmesh )
{
    // Get out if problems.
    if ( !m_polyMesh.valid() )
    {
        return;
    }

    if ( m_polyMesh.getSchema().getNumSamples() > 0 )
    {
        m_polyMesh.getSchema().get( m_samp );
    }
    m_boundsProp = m_polyMesh.getSchema().getSelfBoundsProperty();

    // The object has already set up the min time and max time of
    // all the children.
    // if we have a non-constant time sampling, we should get times
    // out of it.
    TimeSamplingPtr iTsmp = m_polyMesh.getSchema().getTimeSampling();
    if ( !m_polyMesh.getSchema().isConstant() )
    {
        size_t numSamps =  m_polyMesh.getSchema().getNumSamples();
        if ( numSamps > 0 )
        {
            chrono_t minTime = iTsmp->getSampleTime( 0 );
            m_minTime = std::min( m_minTime, minTime );
            chrono_t maxTime = iTsmp->getSampleTime( numSamps-1 );
            m_maxTime = std::max( m_maxTime, maxTime );
        }
    }
    m_drwHelper.setName(m_object.getFullName());
}

//-*****************************************************************************
IPolyMeshDrw::~IPolyMeshDrw()
{
    // Nothing!
}

//-*****************************************************************************
bool IPolyMeshDrw::valid()
{
    return IObjectDrw::valid() && m_polyMesh.valid();
}

//-*****************************************************************************
void IPolyMeshDrw::setTime( chrono_t iSeconds )
{
    // Use nearest for now.
    m_ss =  ISampleSelector(iSeconds, ISampleSelector::kNearIndex );
    if ( IsAncestorInvisible(m_polyMesh,m_ss) ) {
        m_visible = false;
        return;
    }
    else
        m_visible = true;
    if (iSeconds != m_currentTime) {
        IObjectDrw::setTime( iSeconds );
        if ( !valid() )
        {
            m_drwHelper.makeInvalid();
            return;
        }
        //IPolyMeshSchema::Sample psamp;
        if ( m_polyMesh.getSchema().isConstant() )
        {
        }
        else if ( m_polyMesh.getSchema().getNumSamples() > 0 )
        {
            m_polyMesh.getSchema().get( m_samp, m_ss );
        }

        m_bounds.makeEmpty();
        m_needtoupdate = true;
    }
}

Box3d IPolyMeshDrw::getBounds()
{
    if(m_bounds.isEmpty())
        m_bounds = m_boundsProp.getValue( m_ss );

    return m_bounds;
}


void IPolyMeshDrw::updateData()
{
    // Get the stuff.
    P3fArraySamplePtr P = m_samp.getPositions();
    Int32ArraySamplePtr indices = m_samp.getFaceIndices();
    Int32ArraySamplePtr counts = m_samp.getFaceCounts();

    // update the mesh
    m_drwHelper.update( P, V3fArraySamplePtr(),
                            indices, counts, getBounds() );

    if ( !m_drwHelper.valid() )
    {
        m_polyMesh.reset();
        return;
    }
    m_needtoupdate = false;
}


//-*****************************************************************************
void IPolyMeshDrw::draw( const DrawContext &iCtx )
{
    if ( !valid() || !m_visible)
    {
        return;
    }

    if(iCtx.getSelection() != "")
    {
        std::string pathSel = iCtx.getSelection();
        if(pathSel.find("/") != std::string::npos)
        {
            if(m_polyMesh.getFullName().find(pathSel) == std::string::npos)
                return;
        }else
            return;
    }

    if (m_needtoupdate)
        updateData();

    m_drwHelper.draw( iCtx );

    IObjectDrw::draw( iCtx );
}

} // End namespace SimpleAbcViewer